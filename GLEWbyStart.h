#include <ruby.h>
#include <GL/glew.h>

/***********************************************************/
/* Ruby To C type conversion                               */
/***********************************************************/

#define R2C_VIA(type, converter)                 \
    static inline type r2c_##type(VALUE value) { \
        return (type)converter(value);           \
    }

R2C_VIA(GLboolean, RTEST)

#define R2C_VIA_ULONG(type) R2C_VIA(type, NUM2ULONG)
R2C_VIA_ULONG(GLubyte)
R2C_VIA_ULONG(GLushort)
R2C_VIA_ULONG(GLuint)
R2C_VIA_ULONG(GLsizei)
R2C_VIA_ULONG(GLenum)
R2C_VIA_ULONG(GLbitfield)
R2C_VIA_ULONG(GLhandleARB)
R2C_VIA_ULONG(GLhalf) /* No ARB/NV/OES ? */

#define R2C_VIA_LONG(type) R2C_VIA(type, NUM2LONG)
R2C_VIA_LONG(GLbyte)
R2C_VIA_LONG(GLshort)
R2C_VIA_LONG(GLint)
R2C_VIA_LONG(GLintptr)
R2C_VIA_LONG(GLsizeiptr)
R2C_VIA_LONG(GLintptrARB)
R2C_VIA_LONG(GLsizeiptrARB)

/* wacky... */
#define r2c_constGLint r2c_GLint

#define R2C_VIA_DOUBLE(type) R2C_VIA(type, NUM2DBL)
R2C_VIA_DOUBLE(GLfloat)
R2C_VIA_DOUBLE(GLdouble)
R2C_VIA_DOUBLE(GLclampf)
R2C_VIA_DOUBLE(GLclampd)

/* wacky... */
#define r2c_constGLfloat r2c_GLfloat

/***********************************************************/
/* C to Ruby type conversion                               */
/***********************************************************/

static VALUE c2r_GLboolean(GLboolean value) {
    return value ? Qtrue : Qfalse;
}

#define C2R_VIA(type, converter)          \
    static VALUE c2r_##type(type value) { \
        return converter(value);          \
    }

#define C2R_VIA_ULONG(type) C2R_VIA(type, ULONG2NUM)
C2R_VIA_ULONG(GLuint)
C2R_VIA_ULONG(GLenum)
C2R_VIA_ULONG(GLhandleARB)

#define C2R_VIA_LONG(type) C2R_VIA(type, LONG2NUM)
C2R_VIA_LONG(GLint)
C2R_VIA_LONG(GLintptr)

/***********************************************************/
/* Meta                                                    */
/***********************************************************/

#define RGL_ENUM(name, value) \
    rb_define_const(mGLEW, name, c2r_GLenum(value))

#define RGL_FUNCTION(name, fn, args) \
    rb_define_module_function(mGLEW, name, fn, args);
