CXX?=c++
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
OPENCV2FLAGS=$(shell pkg-config opencv --cflags --libs)
CXXFLAGS?=-std=c++14 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow

.PHONY: all msg clean fullclean

all: msg main

msg:
	@echo '--- C++14 ---'

main: SLAM.cpp frame.cpp helper.cpp point.cpp
	${CXX} ${CXXFLAGS} -O2 -o $@ $< ${SDL2FLAGS} ${OPENCV2FLAGS}

small: SLAM.cpp
	${CXX} ${CXXFLAGS} -Os -o main $< ${SDL2FLAGS}
	-strip main
	-sstrip main

debug: SLAM.cpp
	${CXX} ${CXXFLAGS} -O0 -g -o main $< ${SDL2FLAGS}

asm: main.asm

main.asm: SLAM.cpp
	${CXX} ${CFLAGS} -S -o main.asm $< ${SDL2FLAGS}

run: msg main
	time ./main

clean:
	rm -f main *.o main.asm

fullclean: clean
