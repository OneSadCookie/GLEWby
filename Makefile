GLEWby.o: GLEWby.c GLEWbyStart.h GLEWbyEnd.h
	gcc -I../Thirdparty/Root/i386/include/ruby -Iglew/include -c GLEWby.c -Wall -W -Wno-unused-parameter -Werror

GLEWby.c: glewby.rb
	ruby glewby.rb

preprocess: GLEWby.c GLEWbyStart.h GLEWbyEnd.h
	gcc -E -I../Thirdparty/Root/i386/include/ruby -Iglew/include GLEWby.c
