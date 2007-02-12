#ifndef glewby_constants_h
#define glewby_constants_h

#include "glewby.h"
#include "glewby-types.h"

#define RGL_ENUM(name)                           \
    rb_define_const(                             \
        mGLEW, "GL_"#name, c2r_GLenum(GL_##name))

extern void init_gl_constants(void);

#endif
