.PHONY: test

build: rm
	mkdir build && cd build && cmake .. && make

rm:
	rm -rf build

test:
	cd build/test && ctest --progress -V

