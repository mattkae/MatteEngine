FILENAME=flooids
EXE=$(FILENAME)

CXX = g++

LINK_FLAGS = -lglfw3 -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit

src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

$(EXE): $(obj)
	$(CXX) -o $@ $^ $(LINK_FLAGS)

.PHONY: clean
clean:
	rm -rf $(obj) fluids
