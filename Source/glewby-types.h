#ifndef glewby_types_h
#define glewby_types_h

#include <GL/glew.h>
#include <ruby.h>

#include "glewby-pointer.h"

/***********************************************************/
/* Ruby To C type conversion                               */
/***********************************************************/

#define R2C_VIA(type, converter)                 \
    static inline type r2c_##type(VALUE value) { \
        return (type)converter(value);           \
    }

R2C_VIA(GLboolean, RTEST)

/* long long is 64-bit everywhere I know of... */
R2C_VIA(GLuint64EXT, NUM2ULL)
R2C_VIA(GLint64EXT, NUM2LL)

#define R2C_VIA_ULONG(type) R2C_VIA(type, NUM2ULONG)
R2C_VIA_ULONG(GLubyte)
R2C_VIA_ULONG(GLushort)
R2C_VIA_ULONG(GLuint)
R2C_VIA_ULONG(GLsizei)
R2C_VIA_ULONG(GLenum)
R2C_VIA_ULONG(GLbitfield)
R2C_VIA_ULONG(GLhandleARB)
R2C_VIA_ULONG(GLhalf)

#define R2C_VIA_LONG(type) R2C_VIA(type, NUM2LONG)
R2C_VIA_LONG(GLbyte)
R2C_VIA_LONG(GLshort)
R2C_VIA_LONG(GLint)
R2C_VIA_LONG(GLintptr)
R2C_VIA_LONG(GLsizeiptr)
R2C_VIA_LONG(GLintptrARB)
R2C_VIA_LONG(GLsizeiptrARB)

#define r2c_GLintConst r2c_GLint

#define R2C_VIA_DOUBLE(type) R2C_VIA(type, NUM2DBL)
R2C_VIA_DOUBLE(GLfloat)
R2C_VIA_DOUBLE(GLdouble)
R2C_VIA_DOUBLE(GLclampf)
R2C_VIA_DOUBLE(GLclampd)

#define r2c_GLfloatConst r2c_GLfloat

static inline void *r2c_voidStar(VALUE value) {
    /* for VBOs/PBOs */
    if (NIL_P(value)) {
        return NULL;
    } else if (rb_obj_is_kind_of(value, rb_cNumeric)) {
        return (void *)NUM2ULONG(value);
    } else {
        return StringValuePtr(value);
    }
}

#define r2c_GLvoidStar r2c_voidStar
#define r2c_GLvoidConstStar r2c_voidStar
#define r2c_voidConstStar r2c_voidStar

#define R2C_VIA_STRING(name, type)               \
    static inline type r2c_##name(VALUE value) { \
        return (type)StringValuePtr(value);      \
    }

R2C_VIA_STRING(GLcharStar, GLchar *)
R2C_VIA_STRING(GLcharARBStar, GLcharARB *)
R2C_VIA_STRING(GLubyteStar, GLubyte *)
R2C_VIA_STRING(GLbyteStar, GLbyte *)

#define r2c_GLcharConstStar r2c_GLcharStar
#define r2c_GLcharARBConstStar r2c_GLcharARBStar
#define r2c_GLubyteConstStar r2c_GLubyteStar
#define r2c_GLbyteConstStar r2c_GLbyteStar

#define DECLARE_R2C_ARRAY_RAW(type, name)       \
    extern type *r2c_##name##Star(VALUE value);
#define DECLARE_R2C_ARRAY(type) DECLARE_R2C_ARRAY_RAW(type, type)

DECLARE_R2C_ARRAY(GLboolean)
DECLARE_R2C_ARRAY(GLushort)
DECLARE_R2C_ARRAY(GLuint)
DECLARE_R2C_ARRAY(GLsizei)
DECLARE_R2C_ARRAY(GLenum)
DECLARE_R2C_ARRAY(GLhalf)
DECLARE_R2C_ARRAY(GLhandleARB)
DECLARE_R2C_ARRAY(GLuint64EXT)

DECLARE_R2C_ARRAY(GLshort)
DECLARE_R2C_ARRAY(GLint)
DECLARE_R2C_ARRAY(GLint64EXT)

DECLARE_R2C_ARRAY(GLfloat)
DECLARE_R2C_ARRAY(GLdouble)
DECLARE_R2C_ARRAY(GLclampf)

DECLARE_R2C_ARRAY_RAW(void *, voidStar)
#define r2c_GLvoidStarStar r2c_voidStarStar

DECLARE_R2C_ARRAY_RAW(GLchar *, GLcharStar)
DECLARE_R2C_ARRAY_RAW(GLcharARB *, GLcharARBStar)

#define r2c_GLcharConstStarStar r2c_GLcharStarStar
#define r2c_GLcharARBConstStarStar r2c_GLcharARBStarStar

#define r2c_GLbooleanConstStar r2c_GLbooleanStar
#define r2c_GLushortConstStar r2c_GLushortStar
#define r2c_GLuintConstStar r2c_GLuintStar
#define r2c_GLsizeiConstStar r2c_GLsizeiStar
#define r2c_GLenumConstStar r2c_GLenumStar
#define r2c_GLhalfConstStar r2c_GLhalfStar

#define r2c_GLshortConstStar r2c_GLshortStar
#define r2c_GLintConstStar r2c_GLintStar

#define r2c_GLfloatConstStar r2c_GLfloatStar
#define r2c_GLdoubleConstStar r2c_GLdoubleStar
#define r2c_GLclampfConstStar r2c_GLclampfStar

#define r2c_voidConstStarStar r2c_voidStarStar
#define r2c_GLvoidConstStarStar r2c_voidStarStar
#define r2c_GLvoidConstStarConstStar r2c_voidStarStar

DECLARE_R2C_ARRAY_RAW(GLboolean *, GLbooleanStar)

#define r2c_GLbooleanConstStarStar r2c_GLbooleanStarStar

/***********************************************************/
/* C to Ruby type conversion                               */
/***********************************************************/

static inline VALUE c2r_GLboolean(GLboolean value) {
    return value ? Qtrue : Qfalse;
}

#define C2R_VIA(type, converter)                 \
    static inline VALUE c2r_##type(type value) { \
        return converter(value);                 \
    }

/* long long is 64-bit everywhere I know of... */
C2R_VIA(GLint64EXT, LL2NUM)
C2R_VIA(GLuint64EXT, ULL2NUM)

#define C2R_VIA_ULONG(type) C2R_VIA(type, ULONG2NUM)
C2R_VIA_ULONG(GLushort)
C2R_VIA_ULONG(GLuint)
C2R_VIA_ULONG(GLsizei)
C2R_VIA_ULONG(GLenum)
C2R_VIA_ULONG(GLhandleARB)

#define C2R_VIA_LONG(type) C2R_VIA(type, LONG2NUM)
C2R_VIA_LONG(GLshort)
C2R_VIA_LONG(GLint)
C2R_VIA_LONG(GLintptr)

#define C2R_VIA_DOUBLE(type) C2R_VIA(type, rb_float_new)
C2R_VIA_DOUBLE(GLfloat)
C2R_VIA_DOUBLE(GLdouble)

static inline VALUE c2r_GLubyteConstStar(
        const GLubyte *value) {
    return rb_str_new2((const char *)value);
}

static inline VALUE c2r_voidStar(void *value) {
    return glewby_wrap_pointer(value);
}

#define c2r_GLvoidStar c2r_voidStar

/***********************************************************/
/* Write-back to Ruby array out parameters                 */
/***********************************************************/

#define DECLARE_C2R_WRITEBACK_ARRAY(type)    \
    extern void c2r_writeback_##type##Star(  \
            const type *array, VALUE value);

DECLARE_C2R_WRITEBACK_ARRAY(GLboolean)
DECLARE_C2R_WRITEBACK_ARRAY(GLushort)
DECLARE_C2R_WRITEBACK_ARRAY(GLuint)
DECLARE_C2R_WRITEBACK_ARRAY(GLsizei)
DECLARE_C2R_WRITEBACK_ARRAY(GLenum)
DECLARE_C2R_WRITEBACK_ARRAY(GLhandleARB)
DECLARE_C2R_WRITEBACK_ARRAY(GLuint64EXT)

DECLARE_C2R_WRITEBACK_ARRAY(GLshort)
DECLARE_C2R_WRITEBACK_ARRAY(GLint)
DECLARE_C2R_WRITEBACK_ARRAY(GLint64EXT)

DECLARE_C2R_WRITEBACK_ARRAY(GLfloat)
DECLARE_C2R_WRITEBACK_ARRAY(GLdouble)

#endif