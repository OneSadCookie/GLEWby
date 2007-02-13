#include "glewby.h"
#include "glewby-extensions.h"
#include "glewby-constants.h"
#include "glewby-functions.h"
#include "glewby-pointer.h"

#if defined(_WIN32)
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT __attribute__((visibility("default")))
#endif

VALUE mGLEW = Qnil;

static VALUE rglew_Init(VALUE self) {
    return c2r_GLenum(glewInit());
}

DLLEXPORT void Init_glewby(void) {
    mGLEW = rb_define_module("GLEW");
    
    rb_define_module_function(
        mGLEW, "glewInit", rglew_Init, 0);
    
    init_gl_extensions();
    init_gl_constants();
    init_gl_functions();
    
    glewby_init_pointer();
}
