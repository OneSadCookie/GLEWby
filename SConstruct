import os

env = Environment(
    tools = ['mingw'],
    ENV = os.environ)
env.Append(CCFLAGS = [
    '-Wall', '-W', '-Wno-unused-parameter', '-Werror'])
env.Append(CPPPATH = [
    '../Thirdparty/Root/i386/include/ruby',
    'glew/include'])
env.Append(LIBPATH = [
    '../Thirdparty/Root/i386/lib',
    'glew/lib'])

GENERATED_SOURCE = [
    'glewby-extensions.c',
    'glewby-constants.c',
    'glewby-functions.c'
]

SOURCE = GENERATED_SOURCE + [
    'glewby-types.c',
    'glewby.c']

env.Command(
    target = GENERATED_SOURCE,
    source = ['glewby.rb'],
    action = 'ruby glewby.rb glew/include/GL/glew.h')

env.SharedLibrary(
    target = 'glewby',
    source = SOURCE,
    CPPDEFINES = { 'GLEW_STATIC': 1 },
    LIBS = [ 'glew32s', 'libmsvcrt-ruby18', 'opengl32' ])
