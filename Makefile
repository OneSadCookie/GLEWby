glewby.bundle: GLEWby.o
	gcc -bundle GLEWby.o glew/lib/libGLEW.a -o glewby.bundle -framework OpenGL -undefined dynamic_lookup

GLEWby.o: GLEWby.c GLEWbyStart.h GLEWbyEnd.h
	gcc -I../Thirdparty/Root/i386/include/ruby -Iglew/include -O2 -c GLEWby.c -Wall -W -Wno-unused-parameter -Werror

GLEWby.c: glewby.rb
	ruby glewby.rb

preprocess: GLEWby.c GLEWbyStart.h GLEWbyEnd.h
	gcc -E -I../Thirdparty/Root/i386/include/ruby -Iglew/include GLEWby.c

clean:
	rm -f GLEWby.o glewby.bundle
