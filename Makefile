build:
	g++ -std=c++14 main.cpp -o raytracer.out

gen:
	./raytracer.out > image.ppm && open image.ppm

build_gen: build gen
