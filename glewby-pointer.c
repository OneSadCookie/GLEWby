#include "glewby.h"
#include "glewby-pointer.h"

static VALUE cGLEWPointer = Qnil;

VALUE glewby_wrap_pointer(void *pointer)
{
    return Data_Wrap_Struct(
        cGLEWPointer, NULL, NULL, pointer);
}

void *glewby_unwrap_pointer(VALUE wrapper)
{
    void *pointer;
    Data_Get_Struct(wrapper, void, pointer);
    return pointer;
}

void glewby_init_pointer(void)
{
    cGLEWPointer = rb_define_class_under(
        mGLEW, "Pointer", rb_cObject);
}
