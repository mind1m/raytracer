cmake:
	cmake --build build

gen:
	build/raytracer > image.ppm && open image.ppm

build_gen: cmake gen
