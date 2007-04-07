#include "glewby-types.h"

/***********************************************************/
/* Ruby To C type conversion                               */
/***********************************************************/

#define R2C_ARRAY_RAW(type, name)                         \
    type *r2c_##name##Star(VALUE value) {                 \
        if (value == Qnil)                                \
        {                                                 \
            return NULL;                                  \
        }                                                 \
                                                          \
        long n = RARRAY(value)->len;                      \
        type *array = malloc(n * sizeof(type));           \
        long i;                                           \
        for (i = 0; i < n; ++i) {                         \
            array[i] = r2c_##name(RARRAY(value)->ptr[i]); \
        }                                                 \
        return array;                                     \
    }
#define R2C_ARRAY(type) R2C_ARRAY_RAW(type, type)

#define SIMPLE_TYPE(type) R2C_ARRAY(type)
#include "glewby-simple-types.h"
#undef SIMPLE_TYPE

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

#define SIMPLE_TYPE(type) C2R_WRITEBACK_ARRAY(type)
#include "glewby-simple-types.h"
#undef SIMPLE_TYPE
