SIMPLE_TYPE(GLboolean)

/*SIMPLE_TYPE(GLcharARB)
SIMPLE_TYPE(GLchar)*/

SIMPLE_TYPE(GLhandleARB)

SIMPLE_TYPE(GLbitfield)

SIMPLE_TYPE(GLenum)

/*SIMPLE_TYPE(GLbyte)
SIMPLE_TYPE(GLubyte)*/
SIMPLE_TYPE(GLshort)
SIMPLE_TYPE(GLushort)
SIMPLE_TYPE(GLint)
SIMPLE_TYPE(GLuint)
SIMPLE_TYPE(GLsizei)
SIMPLE_TYPE(GLintptr)
SIMPLE_TYPE(GLintptrARB)
SIMPLE_TYPE(GLsizeiptr)
SIMPLE_TYPE(GLsizeiptrARB)

#if defined(GL_EXT_timer_query) /* && GL_EXT_timer_query ? */
/* see glewby-types.h for explanation */
SIMPLE_TYPE(GLint64EXT)
SIMPLE_TYPE(GLuint64EXT)
#endif

SIMPLE_TYPE(GLhalf)

SIMPLE_TYPE(GLfloat)
SIMPLE_TYPE(GLdouble)
SIMPLE_TYPE(GLclampf)
SIMPLE_TYPE(GLclampd)

SIMPLE_TYPE(voidStar)
SIMPLE_TYPE(GLvoidStar)
SIMPLE_TYPE(GLcharARBStar)
SIMPLE_TYPE(GLcharStar)
SIMPLE_TYPE(GLbyteStar)
SIMPLE_TYPE(GLubyteStar)

