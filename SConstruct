import os, re

base_env = Environment(ENV = os.environ)

def union(a, b):
    x = a.copy()
    x.update(b)
    return x

class GCC:
    def environment(self, base_env):
        return base_env.Copy(
            CCFLAGS = [ '-Wall', '-W', '-Wno-unused-parameter', '-Werror' ],
            CPPPATH = [ 'glew/include', 'Source' ],
            LIBPATH = [ 'glew/lib' ],
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
        env.Append(
            tools = [ 'mingw' ])
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
        env.Append(CCFLAGS = [
            '-arch', self.arch,
            '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk' ])
        env.Append(LINKFLAGS = [
            '-arch', self.arch,
            '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk' ])
        return env
    
    def glewby_extension(self, env, *args, **kwargs):
        return env.LoadableModule(
            *args,
            **union(kwargs, {
                'LIBS': [ 'GLEW', 'ruby' ],
                'FRAMEWORKS': [ 'OpenGL' ]
            }))

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
    source = ['glewby.rb'],
    action = 'ruby glewby.rb glew/include/GL/glew.h Build/Generated')

if os.name == 'nt':
    glewby = build_glewby(GLEWbyWin32())
    base_env.Install(dir = '.', source = glewby)
else:
    # TODO handle Linux
    glewby_ppc = build_glewby(GLEWbyMacOSX('ppc'))
    glewby_i386 = build_glewby(GLEWbyMacOSX('i386'))
    base_env.Command(
        target = 'glewby.bundle',
        source = [ glewby_ppc, glewby_i386 ],
        action = 'lipo $SOURCES -create -output $TARGET')
