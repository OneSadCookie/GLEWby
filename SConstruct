import glob, os, re

HEADER_PATHS = [
    'glew/include/GL/glew.h',
    '/usr/include/GL/glew.h' ]    

class CantFindGLEWHeader:
    
    def __str__(self):
        raise IOError('Unable to find GLEW header - please pass GLEW_HEADER_PATH to SCons')

def glew_header():
    for path in HEADER_PATHS:
        try:
            os.stat(path)
            return path
        except:
            pass
    return CantFindGLEWHeader()

opts = Options()
opts.Add('FAT_ARCHS', 'Mac OS X universal binary architectures', 'ppc,i386')
opts.Add('GLEW_HEADER_PATH', 'Path to the GLEW header', glew_header())

base_env = Environment(ENV = os.environ, options=opts)
base_env['GLEW_HEADER'] = base_env.File(base_env['GLEW_HEADER_PATH'])

def union(a, b):
    x = a.copy()
    x.update(b)
    return x

class GCC:
    def environment(self, base_env):
        return base_env.Copy(
            CCFLAGS = [ '-Wall', '-W', '-Wno-unused-parameter', '-Werror', '-g', '-O2' ],
            CPPPATH = [ '${GLEW_HEADER.dir.dir}', 'Source' ],
            LIBPATH = [ '${GLEW_HEADER.dir.dir.dir}/lib' ],
            CPPDEFINES = { 'GLEW_STATIC': 1 })

class ThirdParty(GCC):
    def __init__(self, arch):
        self.arch = arch
    
    def environment(self, base_env):
        env = GCC.environment(self, base_env)
        env.Append(CPPPATH = [
            '../Thirdparty/Root/' + self.arch + '/include/ruby' ])
        env.Append(LIBPATH = [
            '../Thirdparty/Root/' + self.arch + '/lib' ])
        return env
    
    def build_dir(self):
        return 'Build/' + self.arch

class GLEWbyWin32(ThirdParty):
    def __init__(self):
        ThirdParty.__init__(self, 'i386')
    
    def environment(self, base_env):
        env = ThirdParty.environment(self, base_env)
        env = env.Copy(tools = [ 'mingw' ])
        return env
    
    def glewby_extension(self, env, *args, **kwargs):
        return env.SharedLibrary(
            *args,
            **union(kwargs, {
                'LIBS': [ 'glew32s', 'libmsvcrt-ruby18', 'opengl32' ]
            }))
    
    def build_dir(self):
        return 'Build'

class GLEWbyMacOSX(ThirdParty):
    def environment(self, base_env):
        env = ThirdParty.environment(self, base_env)
        env.Append(
            ENV = { 'MACOSX_DEPLOYMENT_TARGET': '10.4' })
        env.Append(CCFLAGS = [
            '-arch', self.arch,
            '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk',
            '-fvisibility=hidden' ])
        env.Append(LINKFLAGS = [
            '-arch', self.arch,
            '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk',
            '-undefined', 'dynamic_lookup' ])
        return env
    
    def glewby_extension(self, env, *args, **kwargs):
        return env.LoadableModule(
            *args,
            **union(kwargs, {
                'LIBS': [ 'GLEW' ],
                'FRAMEWORKS': [ 'OpenGL' ]
            }))

class GLEWbyLinux(GCC):
    def __init__(self, arch):
        self.arch = arch
    
    def environment(self, base_env):
        env = GCC.environment(self, base_env)
        env.Append(CCFLAGS = [
            '-fvisibility=hidden' ])
        env.Append(CPPPATH = [
            '/usr/lib/ruby/1.8/i486-linux'])
        return env
    
    def glewby_extension(self, env, *args, **kwargs):
        return env.SharedLibrary(
            *args,
            **union(kwargs, {
                'LIBS': [ 'GLEW', 'GL' ]
            }))
    
    def build_dir(self):
        return 'Build/' + self.arch

def generated(source):
    return 'Build/Generated/' + source

def source_to_object(policy, name):
    return re.sub('^(Source|Build)', policy.build_dir(), name) + '$SHOBJSUFFIX'

def build_glewby(policy):
    env = policy.environment(base_env)
    
    source = GENERATED_SOURCE + [
        'Source/glewby-pointer.c',
        'Source/glewby-types.c',
        'Source/glewby.c'
    ]
    
    objects = map(lambda f: env.SharedObject(
            target = source_to_object(policy, f),
            source = f),
        source)
    
    return policy.glewby_extension(env,
        target = policy.build_dir() + '/glewby',
        source = objects)

GENERATED_SOURCE = [
    generated('glewby-extensions.c'),
    generated('glewby-constants.c'),
    generated('glewby-functions.c')
]
    
base_env.Command(
    target = GENERATED_SOURCE,
    source = ['glewby.rb'] + glob.glob('Templates/Ruby/*.rc'),
    action = 'ruby glewby.rb --glew-header=$GLEW_HEADER --output-directory=Build/Generated --template-dir=Templates/Ruby')

if os.name == 'nt':
    build_glewby(GLEWbyWin32())
    base_env.Install(
        dir = '.',
        source = 'Build/glewby.dll')
elif os.name == 'sposix':
    kernel_name = os.popen('uname -s').read()
    if kernel_name == "Linux\n": # is that enough?
        glewby = build_glewby(GLEWbyLinux('i386'))
        base_env.InstallAs(
            target = 'glewby.so',
            source = glewby)
    elif kernel_name == "Darwin\n":
        fat_archs = base_env['FAT_ARCHS'].split(',')
        glewbys = map(lambda a: build_glewby(GLEWbyMacOSX(a)), fat_archs)
        base_env.Command(
            target = 'glewby.bundle',
            source = glewbys,
            action = 'lipo $SOURCES -create -output $TARGET')
else:
    raise EnvironmentError('Unknown OS')
