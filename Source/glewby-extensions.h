#ifndef glewby_extensions_h
#define glewby_extensions_h

#include "glewby.h"
#include "glewby-types.h"

#define RGL_EXT(name)                         \
    rb_define_module_function(                \
        mGLEW, "GLEW_"#name, rglext_##name, 0)

extern void init_gl_extensions(void);

#endif
