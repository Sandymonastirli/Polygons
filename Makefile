#Chrysanthi Monastirli AM 1716, Miltiadis Vasileiadis AM 2944
#This sample Makefile allows you to make any SRGP application
#   whose source is exactly one .c file.
#
# To use this Makefile, you must type:
#
#        make PROG=xxxx
#                  ----
# where
#       xxxx.cpp is the name of the file you wish to compile.
#       ----
#$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) -c Camera3D.cpp $(LDFLAGS) $(LDLIBS) -o Camera3D.o
#$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) -c Polygon.cpp $(LDFLAGS) $(LDLIBS) -o Polygon.o
#$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) -c Triangulate.cpp $(LDFLAGS) $(LDLIBS) -o Triangulate.o
#$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) -c PolygonClipping.cpp $(LDFLAGS) $(LDLIBS) -o PolygonClipping.o
#$(CC) -o ask1.exe Camera3D.o Polygon.o Triangulate.o PolygonClipping.o Program.o
#$(PROG):
#	$(CC) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) -c Program.cpp $(LDFLAGS) $(LDLIBS) -o Program.o


#FILES specifies which files to compile as part of the project
FILES = Camera3D.cpp Polygon.cpp Triangulate.cpp PolygonClipping.cpp Program.cpp
#EXE_NAME specifies the name of our exectuable
EXE_NAME = ask1_1
CC = g++

GCCFLAGS = -fpcc-struct-return
CFLAGS = -g -std=c++11
INCLUDE = -I/usr/include -I./glm
LDFLAGS = -L/usr/lib/
LDLIBS =  -lGLU -lGL -lglut -lm


all:$(FILES)
	$(CC) $(FILES) $(GCCFLAGS) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(EXE_NAME)
#default:
#	example
