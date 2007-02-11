import os

env = Environment(
    tools = ['mingw'],
    ENV = os.environ)
env.Append(CPPPATH = [
    '../Thirdparty/Root/i386/include/ruby',
    'glew/include'])
env.Append(LIBPATH = [
    '../Thirdparty/Root/i386/lib',
    'glew/lib'])

env.Command(
    target = 'GLEWby.c',
    source = ['glewby.rb'],
    action = 'ruby glewby.rb')

env.SharedLibrary(
    target = 'glewby',
    source = ['GLEWby.c'],
    CPPDEFINES = { 'GLEW_STATIC': 1 },
    LIBS = [ 'glew32s', 'libmsvcrt-ruby18', 'opengl32' ])
