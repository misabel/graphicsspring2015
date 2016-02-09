#
#  Makefile for fltk applications
#

# set local paths
FLTK =

INCLUDE = -I$(FLTK) -I$(FLTK)/png -I$(FLTK)/jpeg
LIBDIR = -L$(FLTK)/lib

CFLAGS = -O2 -g -fpermissive -std=c++11 -Dcimg_display_type=0 
LIBS = -lfltk -lfltk_gl -lm -lfltk_png -lfltk_jpeg -ldl -lpthread -lz -framework OpenGL -framework AGL -framework Cocoa



CC = g++

.SUFFIXES: .o .cpp .cxx

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.cxx.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = src/main.o src/getopt.o src/RayTracer.o \
	src/ui/CommandLineUI.o src/ui/GraphicalUI.o src/ui/TraceGLWindow.o \
	src/ui/debuggingView.o src/ui/glObjects.o src/ui/debuggingWindow.o \
	src/ui/ModelerCamera.o \
	src/fileio/imageio.o src/fileio/buffer.o \
	src/parser/Token.o src/parser/Tokenizer.o \
	src/parser/Parser.o src/parser/ParserException.o \
	src/scene/camera.o src/scene/light.o \
	src/scene/material.o src/scene/ray.o src/scene/scene.o \
	src/SceneObjects/Box.o src/SceneObjects/Cone.o \
	src/SceneObjects/Cylinder.o src/SceneObjects/trimesh.o \
	src/SceneObjects/Sphere.o src/SceneObjects/Square.o src/threads/ThreadPool.o

trace: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f $(ALL.O) trace
