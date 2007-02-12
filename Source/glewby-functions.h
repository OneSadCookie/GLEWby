#ifndef glewby_functions_h
#define glewby_functions_h

#include "glewby.h"
#include "glewby-types.h"

#define RGL_FUNCTION(name, args)           \
    rb_define_module_function(             \
        mGLEW, "gl"#name, rgl_##name, args)

extern void init_gl_functions(void);

#endif
