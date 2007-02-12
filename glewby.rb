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

class Type
    
    def initialize(const)
        @const = const
    end
    
    def to_s
        if @const then
            ' const'
        else
            ''
        end
    end
    
    def const?
        @const
    end
    
    def == (other)
        @const == other.const?
    end
    
end

class SimpleType < Type
    
    attr_reader :name
    
    def initialize(name, const)
        super(const)
        @name = name
    end
    
    def to_s
        @name + super
    end
    
    def freeable_copy
        self.class.new(@name, false)
    end
    
    def void?
        @name == 'void' || @name == 'GLvoid'
    end
    
    def pointer?
        false
    end
    
    def == (other)
        @name == other.name && super(other)
    end
    
end

class PointerType < Type
    
    attr_reader :type
    
    def initialize(type, const)
        super(const)
        @type = type
    end
    
    def to_s
        @type.to_s + ' *' + super
    end
    
    def freeable_copy
        self.class.new(@type.freeable_copy, const?)
    end
    
    def void?
        false
    end
    
    def pointer?
        true
    end
    
    def == (other)
        @type == other.type && super(other)
    end
    
end

def parse_type(type)
    bits = type.split(/\b/).collect do |token|
        s = token.strip
        if s.size == 0 then
            nil
        else
            s
        end
    end.compact
    
    # TODO there's gotta be a better way to do this!
    if bits.size == 1 then
        SimpleType.new(bits[0], false)
    elsif bits.size == 2 && bits[0] == 'const' then
        SimpleType.new(bits[1], true)
    elsif bits.size == 2 && bits[1] == '*' then
        PointerType.new(SimpleType.new(bits[0], false), false)
    elsif bits.size == 2 && bits[1] =~ /\*\s*\*/ then
        PointerType.new(PointerType.new(SimpleType.new(bits[0], false), false), false)
    elsif bits.size == 3 && bits[0] == 'const' && bits[2] == '*' then
        PointerType.new(SimpleType.new(bits[1], true), false)
    elsif bits.size == 3 && bits[0] == 'const' && bits[2] =~ /\*\s*\*/ then
        PointerType.new(PointerType.new(SimpleType.new(bits[1], true), false), false)
    elsif bits.size == 5 && bits[0] == 'const' && bits[2] == '*' && bits[3] == 'const' && bits[4] == '*' then
        PointerType.new(PointerType.new(SimpleType.new(bits[1], true), true), false)
    else
        p bits
        exit
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
        name = $2 || 'arg' + (extra_arg_index += 1).to_s
        raw_type = ($1 + $3).gsub(/\[\d*\]/, '*').strip
        type = parse_type(raw_type)
        Argument.new(name, type)
    end
end

def parse_header(path, rejects)
    extensions = {}
    constants = {}
    functions = {}
    functypes = {}
    File.open(path) do |header|
        header.each_line do |line|
            if line =~ /^#define\s+GL_([A-Z_0-9]+)\s+.*$/ then
                constants[$1] = true
            elsif line =~ Regexp.compile(FUNCTION) then
                functions[$2] = Function.new($2, parse_type($1), parse_args($3))
            elsif line =~ Regexp.compile(FUNCTYPE) then
                functypes[$2] = Function.new($2, parse_type($1), parse_args($3))
            elsif line =~ /^#define\s+gl([A-Z][A-Za-z0-9]+)\s+GLEW_GET_FUN.*$/ then
                fn = functypes[$1.upcase]
                if fn == nil then
                    puts("no type for #{$1}")
                else
                    functions[$1] = Function.new($1, fn.return_type, fn.args)
                end
            elsif line =~ /^#define\s+GLEW_([A-Z][A-Za-z0-9_]+)\s*GLEW_GET_VAR.*$/ then
                extensions[$1] = true
            else
                rejects.puts(line)
            end
        end
    end
    return extensions, constants, functions
end

def r_args(fn)
    (['VALUE r_GLEW'] + fn.args.collect do |arg|
        'VALUE r_' + arg.name
    end).join(', ')
end

def sanitize_type(type)
    type.to_s.
        gsub(/\s/, '').
        gsub(/\*/, 'Star').
        gsub(/const/, 'Const').
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

def needs_writeback(type)
    !type.type.const? &&
        !type.type.pointer? &&
        type.type.name !~ /void|byte|char/
end
    
def needs_free(type)
    !type.type.pointer? &&
        type.type.name !~ /void|byte|char/
end
    
def print_extension_function(io, name)
    io.puts <<EOE
static VALUE rglext_#{name}(VALUE r_GLEW) {
    return c2r_GLboolean(GLEW_#{name});
}

EOE
end

def print_function(io, fn)
    io.puts "static VALUE rgl_#{fn.name}(#{r_args(fn)}) {"
    fn.args.each do |arg|
        io.puts "    #{arg.type.freeable_copy} #{arg.name} = #{r2c(arg.type)}(r_#{arg.name});"
    end
    
    funcall = 'gl' + fn.name + '(' + fn.args.collect do |arg|
        if arg.type.freeable_copy != arg.type then
            "(#{arg.type})#{arg.name}"
        else
            arg.name
        end
    end.join(', ') + ')'
    
    if fn.return_type.void? then
        io.puts "    #{funcall};\n    VALUE result = Qnil;"
    else
        io.puts "    VALUE result = #{c2r(fn.return_type)}(#{funcall});"
    end
    
    fn.args.each do |arg|
        if arg.type.pointer? then
            if needs_writeback(arg.type) then
                io.puts "    #{c2r_writeback(arg.type)}(#{arg.name}, r_#{arg.name});"
            end
            if needs_free(arg.type) then
                io.puts "    free(#{arg.name});"
            end
        end
    end
    
    io.puts "    return result;\n}\n\n"
end

def print_extension_bindings(io, list)
    list.each do |extension|
        io.puts <<EOE
    RGL_EXT(#{extension});
EOE
    end
end

def print_constant_bindings(io, list)
    list.each do |constant|
        io.puts <<EOC
    RGL_ENUM(#{constant});
EOC
    end
end

def print_function_bindings(io, hash)
    hash.keys.sort.each do |name|
        fn = hash[name]
        io.puts <<EOF
    RGL_FUNCTION(#{fn.name}, #{fn.args.size});
EOF
    end
end

def write_extensions_file(io, extensions)
    io.puts("#include \"glewby-extensions.h\"\n\n")
    
    extensions.keys.sort.each do |name|
        print_extension_function(io, name)
    end
    
    io.puts "void init_gl_extensions(void) {"
    print_extension_bindings(io, extensions.keys.sort)
    io.puts "}"
end

def write_constants_file(io, constants)
    io.puts("#include \"glewby-constants.h\"\n\n")
    
    io.puts "void init_gl_constants(void) {"
    print_constant_bindings(io, constants.keys.sort)
    io.puts "}"
end

def write_functions_file(io, functions)
    io.puts("#include \"glewby-functions.h\"\n\n")
    
    functions.keys.sort.each do |name|
        fn = functions[name]
        print_function(io, fn)
    end
    
    io.puts "void init_gl_functions(void) {"
    print_function_bindings(io, functions)
    io.puts "}"
end

def main(args)
    glew_header = args[0]
    output_prefix = args[1]
    
    extensions, constants, functions =
        parse_header(glew_header, DevNull.new)
    
    File.open(output_prefix +
            '/glewby-extensions.c', 'wb') do |file|
        write_extensions_file(file, extensions)
    end
    
    File.open(output_prefix +
            '/glewby-constants.c', 'wb') do |file|
        write_constants_file(file, constants)
    end
    
    File.open(output_prefix +
            '/glewby-functions.c', 'wb') do |file|
        write_functions_file(file, functions)
    end
end

main(ARGV)
