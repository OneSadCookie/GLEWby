import os, re

env = Environment(
    tools = ['mingw'],
    ENV = os.environ)
env.Append(CCFLAGS = [
    '-Wall', '-W', '-Wno-unused-parameter', '-Werror'])
env.Append(CPPPATH = [
    '../Thirdparty/Root/i386/include/ruby',
    'glew/include',
    'Source'])
env.Append(LIBPATH = [
    '../Thirdparty/Root/i386/lib',
    'glew/lib'])

GENERATED_SOURCE = [
    'Build/Generated/glewby-extensions.c',
    'Build/Generated/glewby-constants.c',
    'Build/Generated/glewby-functions.c'
]

SOURCE = GENERATED_SOURCE + [
    'Source/glewby-pointer.c',
    'Source/glewby-types.c',
    'Source/glewby.c']

def source_to_object(name):
    return re.sub('^Source', 'Build', name) + '$SHOBJSUFFIX'

OBJECTS = map(lambda f: env.SharedObject(
        target = source_to_object(f),
        source = f,
        CPPDEFINES = { 'GLEW_STATIC': 1 }),
    SOURCE)

env.Command(
    target = GENERATED_SOURCE,
    source = ['glewby.rb'],
    action = 'ruby glewby.rb glew/include/GL/glew.h Build/Generated')

env.SharedLibrary(
    target = 'glewby',
    source = OBJECTS,
    LIBS = [ 'glew32s', 'libmsvcrt-ruby18', 'opengl32' ])
