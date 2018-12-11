CXX=g++
CXXFLAGS += -g -Wall -Wextra -pthread
CPPFLAGS += -isystem src -std=c++17

MKDIR_P = mkdir -p
OBJ_DIR = obj

all: main

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

obj/%.o: src/%.cpp ${OBJ_DIR}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

main: obj/main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@



clean:
	rm -f main
	rm -rf obj
	rm -f *~ obj/*.o obj/*.a
	
