#ifndef glewby_pointer_h
#define glewby_pointer_h

extern VALUE glewby_wrap_pointer(void *pointer);
extern void *glewby_unwrap_pointer(VALUE wrapper);

extern void glewby_init_pointer(void);

#endif
