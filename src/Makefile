# $@でtarget名を取得できる
build-linux:clean-before
	@echo [run] $@;

	g++ \
		-std=c++17 \
		-I /usr/local/include/boost \
		-I /usr/local/include/tesseract \
		-I /usr/local/include/opencv4 \
		./main.cpp \
		/usr/local/lib/libboost_*.so \
		/usr/local/lib/libleptonica.so \
		/usr/local/lib/libtesseract.so \
		/usr/local/lib/libopencv_*.so \
		-o ./main

	make clean-after

	@echo;

debug-linux:clean-before
	@echo [run] $@;

	g++ \
		-g \
		-std=c++17 \
		-I /usr/local/include/boost \
		-I /usr/local/include/tesseract \
		-I /usr/local/include/opencv4 \
		./main.cpp \
		/usr/local/lib/libboost_*.so \
		/usr/local/lib/libleptonica.so \
		/usr/local/lib/libtesseract.so \
		/usr/local/lib/libopencv_*.so \
		-o ./main
		
	make clean-after

	@echo;

build-macos:clean-before
	@echo [run] $@;

	g++ \
		-std=c++17 \
		./main.cpp \
		./../submodules/boost/bin.v2/libs/serialization/build/clang-darwin-12.0/release/threading-multi/visibility-hidden/libboost_serialization.dylib \
		./../submodules/boost/bin.v2/libs/filesystem/build/clang-darwin-12.0/release/threading-multi/visibility-hidden/libboost_filesystem.dylib \
		./../submodules/opencv/build/lib/*.dylib \
		/usr/local/lib/libtesseract.dylib \
		/usr/local/Cellar/leptonica/1.80.0/lib/liblept.dylib \
		-o ./main

	make clean-after

	@echo;

debug-macos:clean-before
	@echo [run] $@;

	g++ \
		-g \
		-std=c++17 \
		./main.cpp \
		./../submodules/boost/bin.v2/libs/serialization/build/clang-darwin-12.0/release/threading-multi/visibility-hidden/libboost_serialization.dylib \
		./../submodules/boost/bin.v2/libs/filesystem/build/clang-darwin-12.0/release/threading-multi/visibility-hidden/libboost_filesystem.dylib \
		./../submodules/opencv/build/lib/*.dylib \
		/usr/local/lib/libtesseract.dylib \
		/usr/local/Cellar/leptonica/1.80.0/lib/liblept.dylib \
		-o ./main

	make clean-after

	@echo;

clean-before:
	@echo [run] $@

	@rm -rfv ./main
	@rm -rfv ./main-debug
	@rm -rfv ./main-debug.*
	@rm -rfv ./aws-lambda-*

	@echo;

clean-after:
	@echo [run] $@

	@rm -rfv ./main-debug.*

	@echo;
