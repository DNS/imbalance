
CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
AR = i686-w64-mingw32-ar
STRIP = i686-w64-mingw32-strip

BIN = ms3d-loader.exe
INCS =  -I../libms3d/src
#INCS =  
LIBS =  -L../libms3d/src -lms3d -lopengl32 -lglu32 -lglaux -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lwinmm
#LIBS =  -lopengl32 -lglu32 -lglaux -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 

all:
	$(CC) $(INCS) -c ms3d-loader.c -o ms3d-loader.o
	$(CC) ms3d-loader.o $(LIBS) -o ms3d-loader.exe
	$(STRIP) ms3d-loader.exe
	

#-I../libms3d/src -L../libms3d/src 

clean:
	rm -f ms3d.o libms3d.a

build:


dist:
	cp ms3d.h ../include/ms3d.h
	cp libms3d.a ../lib/libms3d.a
	tar -cf  ../dist/libms3d.tar ../include/ms3d.h ../lib/libms3d.a
	gzip -9 ../dist/libms3d.tar
	
#tar -cf  ../test.tar && \

#gzip -9  $(RELEASE_DIR)/$(RELEASE_FILE).tar


