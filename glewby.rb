#!/usr/bin/env ruby

IDENTIFIER = '[A-Za-z_][A-Za-z_0-9]*'
TYPE = '(?:const\s+)?' + IDENTIFIER + '(?:\s*(?:const\s+)?\s*\*)*'
ARG = TYPE + '\s*(?:' + IDENTIFIER + ')?(?:\s*\[\d*\])*'
ARGS = '(?:' + ARG + ')?(?:\s*,\s*' + ARG + ')*'
FUNCTION = 'GLAPI\s+(' + TYPE + ')\s+GLAPIENTRY\s+gl(' + IDENTIFIER + ')\s*\(\s*(' + ARGS + ')\s*\)\s*;'
FUNCTYPE = 'typedef\s+(' + TYPE + ')\s+\(GLAPIENTRY\s*\*\s*PFNGL(' + IDENTIFIER + ')PROC\)\s*\(\s*(' + ARGS + ')\s*\);'
ARG_CAPTURE = '(' + TYPE + ')\s*(' + IDENTIFIER + ')?((?:\s*\[\d*\])*)'

Function = Struct.new('Function', :name, :return_type, :args)
Argument = Struct.new('Argument', :name, :type)

class DevNull
    
    def puts(*args)
    end
    
end

def parse_args(args)
    if args =~ /^\s*void\s*$/ then
        return []
    end
    
    extra_arg_index = -1
    args = args.split(/,/)
    args.collect do |arg|
        raise "Can't parse argument " + arg unless arg =~ Regexp.compile(ARG_CAPTURE)
        Argument.new($2 || 'arg' + (extra_arg_index += 1).to_s, ($1 + $3).gsub(/\[\d*\]/, '*'))
    end
end

def parse_header(constants, functions, rejects)
    functypes = {}
    File.open('glew/include/GL/glew.h') do |header|
        header.each_line do |line|
            if line =~ /^#define\s+GL_([A-Z_0-9]+)\s+.*$/ then
                constants[$1] = true
            elsif line =~ Regexp.compile(FUNCTION) then
                functions[$2] = Function.new($2, $1, parse_args($3))
            elsif line =~ Regexp.compile(FUNCTYPE) then
                functypes[$2] = Function.new($2, $1, parse_args($3))
            elsif line =~ /^#define\s+gl([A-Z][A-Za-z0-9]+)\s+GLEW_GET_FUN.*$/ then
                fn = functypes[$1.upcase]
                if fn == nil then
                    puts("no type for #{$1}")
                else
                    functions[$1] = Function.new($1, fn.return_type, fn.args)
                end
            else
                rejects.puts(line)
            end
        end
    end
end

def r_args(fn)
    fn.args.collect do |arg|
        'VALUE r_' + arg.name
    end.join(', ')
end

def sanitize_type(type)
    type.
        gsub(/\s/, '').
        gsub(/\*/, 'Star').
        gsub(/\W/, '_')
end

def r2c(type)
    'r2c_' + sanitize_type(type)
end

def c2r(type)
    'c2r_' + sanitize_type(type)
end

def c2r_writeback(type)
    'c2r_writeback_' + sanitize_type(type)
end

def local_type(type)
    type.gsub(/\s*const\s*/, '')
end

def is_pointer(type)
    (type =~ /\*/  && type !~ /void|char|byte/) ||
        (type =~ /\*.*\*/)
end

def pointer_needs_writeback(type)
    type !~ /const/
end

constants = {}
functions = {}
parse_header(constants, functions, DevNull.new)

$stdout = File.open('GLEWby.c', 'wb')

puts <<EOH
#include "GLEWbyStart.h"


EOH

functions.keys.sort.each do |name|
    fn = functions[name]
    
    puts "static VALUE rgl_#{fn.name}(#{r_args(fn)}) {"
    fn.args.each do |arg|
        puts "    #{local_type(arg.type)} #{arg.name} = #{r2c(arg.type)}(r_#{arg.name});"
    end
    
    funcall = 'gl' + fn.name + '(' + fn.args.collect do |arg|
        arg.name
    end.join(', ') + ')'
    
    if fn.return_type == 'void' then
        puts "    #{funcall};\n    VALUE result = Qnil;"
    else
        puts "    VALUE result = #{c2r(fn.return_type)}(#{funcall});"
    end
    
    fn.args.each do |arg|
        if is_pointer(arg.type) then
            if pointer_needs_writeback(arg.type) then
                puts "    #{c2r_writeback(arg.type)}(#{arg.name}, r_#{arg.name});"
            end
            puts "    free(#{arg.name});"
        end
    end
    
    puts "    return result;\n}\n\n"
end

puts <<EOI
static VALUE mGLEW = Qnil;

void Init_glewby(void) {
    mGLEW = rb_define_module("GLEW");
    
EOI

constants.keys.sort.each do |constant|
    puts <<EOC
    RGL_ENUM("GL_#{constant}", GL_#{constant});
EOC
end

puts "\n\n"

functions.keys.sort.each do |name|
    fn = functions[name]
    
    puts <<EOF
    RGL_FUNCTION("gl#{name}", rgl_#{name}, #{fn.args.size});
EOF
end

puts <<EOF
}

#include "GLEWbyEnd.h"
EOF
