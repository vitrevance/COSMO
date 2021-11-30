include config.mk

BINARIES= cosmo
LIBDIRS= -LSOIL2/lib -LirrKlang-64bit-1.6.0/bin/linux-gcc-64 
INCLUDE_DIRS= -IirrKlang-64bit-1.6.0/include
LIBS= ./SOIL2/lib/libsoil2.a -lIrrKlang -lpthread -lGL -lGLEW -lglfw -lassimp -lfreeimage 

SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst  %.cpp, %.o, $(SOURCES))


all: $(BINARIES)

cosmo: $(OBJS) 
	$(CXX) -o $@ $(CXXFLAGS) $(LIBDIRS) $^ $(LIBS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $^

clean:
	rm $(BINARIES) $(OBJS)


