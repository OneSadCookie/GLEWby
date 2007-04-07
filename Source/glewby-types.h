#ifndef glewby_types_h
#define glewby_types_h

#include <GL/glew.h>
#include <ruby.h>

#include "glewby-pointer.h"

/***********************************************************/
/* Typedefs for basic types                                */
/***********************************************************/

typedef void *            voidStar;
typedef GLvoid *          GLvoidStar;
typedef GLcharARB *       GLcharARBStar;
typedef GLchar *          GLcharStar;
typedef GLbyte *          GLbyteStar;
typedef GLubyte *         GLubyteStar;


/***********************************************************/
/* Ruby To C type conversion                               */
/***********************************************************/

#define R2C_VIA(type, converter)                 \
    static inline type r2c_##type(VALUE value) { \
        return (type)converter(value);           \
    }

R2C_VIA(GLboolean, RTEST)

#if defined(GL_EXT_timer_query) /* && GL_EXT_timer_query ? */
/* long long is 64-bit everywhere I know of... */
R2C_VIA(GLuint64EXT, NUM2ULL)
R2C_VIA(GLint64EXT, NUM2LL)
#endif

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
R2C_VIA_LONG(GLchar)
R2C_VIA_LONG(GLcharARB)
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

#define R2C_VIA_STRING(name, type)               \
    static inline type r2c_##name(VALUE value) { \
        if (value == Qnil) {                     \
            return NULL;                         \
        }                                        \
        return (type)StringValuePtr(value);      \
    }

R2C_VIA_STRING(GLcharStar, GLchar *)
R2C_VIA_STRING(GLcharARBStar, GLcharARB *)
R2C_VIA_STRING(GLubyteStar, GLubyte *)
R2C_VIA_STRING(GLbyteStar, GLbyte *)

#define DECLARE_R2C_ARRAY_RAW(type, name)            \
    extern type *r2c_##name##Star(VALUE value);
#define DECLARE_R2C_ARRAY(type) DECLARE_R2C_ARRAY_RAW(type, type)

#define SIMPLE_TYPE(type) DECLARE_R2C_ARRAY(type)
#include "glewby-simple-types.h"
#undef SIMPLE_TYPE


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

#if defined(GL_EXT_timer_query) /* && GL_EXT_timer_query ? */
/* long long is 64-bit everywhere I know of... */
C2R_VIA(GLint64EXT, LL2NUM)
C2R_VIA(GLuint64EXT, ULL2NUM)
#endif

#define C2R_VIA_ULONG(type) C2R_VIA(type, ULONG2NUM)
C2R_VIA_ULONG(GLubyte)
C2R_VIA_ULONG(GLushort)
C2R_VIA_ULONG(GLuint)
C2R_VIA_ULONG(GLsizei)
C2R_VIA_ULONG(GLenum)
C2R_VIA_ULONG(GLbitfield)
C2R_VIA_ULONG(GLhandleARB)
C2R_VIA_ULONG(GLhalf)

#define C2R_VIA_LONG(type) C2R_VIA(type, LONG2NUM)
C2R_VIA_LONG(GLchar)
C2R_VIA_LONG(GLcharARB)
C2R_VIA_LONG(GLbyte)
C2R_VIA_LONG(GLshort)
C2R_VIA_LONG(GLint)
C2R_VIA_LONG(GLintptr)
C2R_VIA_LONG(GLsizeiptr)
C2R_VIA_LONG(GLintptrARB)
C2R_VIA_LONG(GLsizeiptrARB)

#define C2R_VIA_DOUBLE(type) C2R_VIA(type, rb_float_new)
C2R_VIA_DOUBLE(GLfloat)
C2R_VIA_DOUBLE(GLdouble)
C2R_VIA_DOUBLE(GLclampf)
C2R_VIA_DOUBLE(GLclampd)

#define C2R_VIA_STRING(type)                     \
    static inline VALUE c2r_##type(type value) { \
        return rb_str_new2((const char *)value); \
    }

C2R_VIA_STRING(GLcharStar)
C2R_VIA_STRING(GLcharARBStar)
C2R_VIA_STRING(GLbyteStar)
C2R_VIA_STRING(GLubyteStar)

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

#define SIMPLE_TYPE(type) DECLARE_C2R_WRITEBACK_ARRAY(type)
#include "glewby-simple-types.h"
#undef SIMPLE_TYPE

/***********************************************************/
/* Freeing of temporary storage                            */
/***********************************************************/

#define R2C_FREE(type)                                             \
    static inline void r2c_free_##type##Star(type *pointer) {      \
        free(pointer);                                             \
    }

#define SIMPLE_TYPE(type) R2C_FREE(type)
#include "glewby-simple-types.h"
#undef SIMPLE_TYPE

/***********************************************************/
/* Unusual extras                                          */
/***********************************************************/

/* only used by EdgeFlagPointerListIBM */
static inline GLboolean * * r2c_GLbooleanStarStar(VALUE value) {
    rb_notimplement();
}
static inline void c2r_writeback_GLbooleanStarStar(
        GLboolean * *array, VALUE value) {
    rb_notimplement();
}
static inline void r2c_free_GLbooleanStarStar(GLboolean * *pointer) {
    rb_notimplement();
}

/* only used by glGetString */
#define c2r_GLubyteConstStar(pointer) c2r_GLubyteStar((GLubyte *)pointer)

#endif
