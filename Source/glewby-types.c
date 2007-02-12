#include "glewby-types.h"

/***********************************************************/
/* Ruby To C type conversion                               */
/***********************************************************/

#define R2C_ARRAY_RAW(type, name)                         \
    type *r2c_##name##Star(VALUE value) {                 \
        long n = RARRAY(value)->len;                      \
        type *array = malloc(n * sizeof(type));           \
        long i;                                           \
        for (i = 0; i < n; ++i) {                         \
            array[i] = r2c_##name(RARRAY(value)->ptr[i]); \
        }                                                 \
        return array;                                     \
    }
#define R2C_ARRAY(type) R2C_ARRAY_RAW(type, type)

R2C_ARRAY(GLboolean)
R2C_ARRAY(GLushort)
R2C_ARRAY(GLuint)
R2C_ARRAY(GLsizei)
R2C_ARRAY(GLenum)
R2C_ARRAY(GLhalf)
R2C_ARRAY(GLhandleARB)
R2C_ARRAY(GLuint64EXT)

R2C_ARRAY(GLshort)
R2C_ARRAY(GLint)
R2C_ARRAY(GLint64EXT)

R2C_ARRAY(GLfloat)
R2C_ARRAY(GLdouble)
R2C_ARRAY(GLclampf)

R2C_ARRAY_RAW(void *, voidStar)

R2C_ARRAY_RAW(GLchar *, GLcharStar)
R2C_ARRAY_RAW(GLcharARB *, GLcharARBStar)

R2C_ARRAY_RAW(GLboolean *, GLbooleanStar)

/***********************************************************/
/* Write-back to Ruby array out parameters                 */
/***********************************************************/

#define C2R_WRITEBACK_ARRAY(type)                         \
    void c2r_writeback_##type##Star(                      \
            const type *array, VALUE value) {             \
        long n = RARRAY(value)->len;                      \
        long i;                                           \
        for (i = 0; i < n; ++i) {                         \
            RARRAY(value)->ptr[i] = c2r_##type(array[i]); \
        }                                                 \
    }

C2R_WRITEBACK_ARRAY(GLboolean)
C2R_WRITEBACK_ARRAY(GLushort)
C2R_WRITEBACK_ARRAY(GLuint)
C2R_WRITEBACK_ARRAY(GLsizei)
C2R_WRITEBACK_ARRAY(GLenum)
C2R_WRITEBACK_ARRAY(GLhandleARB)
C2R_WRITEBACK_ARRAY(GLuint64EXT)

C2R_WRITEBACK_ARRAY(GLshort)
C2R_WRITEBACK_ARRAY(GLint)
C2R_WRITEBACK_ARRAY(GLint64EXT)

C2R_WRITEBACK_ARRAY(GLfloat)
C2R_WRITEBACK_ARRAY(GLdouble)
