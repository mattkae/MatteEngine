FILENAME=flooids
EXE=$(FILENAME)

CXX = clang++
CXXFLAGS = -std=c++14 -I./includes
LINK_FLAGS = -lglfw3 -lglew -lassimp -lSOIL -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
.PHONY: depend clean all
DEPS = $(OBJS:.o=.d)

$(EXE): $(OBJS) 
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LINK_FLAGS)

-include $(DEPS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d)

clean:
	rm -rf $(OBJS) $(DEPS) $(EXE)

run:
	./$(FILENAME)
