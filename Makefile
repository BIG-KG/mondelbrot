COMPILLER := g++
CXXFLAGS := -O3 -march=native -Iinclude
LDFLAGS := -lSDL2
SIMD_FLAGS := -msse2 -msse4.1 -mavx

SRCS := src/mondelbrot_Base.cpp src/mondelbrot_inctr.cpp src/testsystem.cpp
OBJS := $(SRCS:.cpp=.o)

all: mandelbrot

src/%.o: src/%.cpp
	$(COMPILLER) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

src/mondelbrot_inctr.o: src/mondelbrot_inctr.cpp
	$(COMPILLER) $(CXXFLAGS) $(SIMD_FLAGS) -c $< -o $@ $(LDFLAGS)

mandelbrot: $(OBJS)
	$(COMPILLER) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@