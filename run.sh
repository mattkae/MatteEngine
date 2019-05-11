#!/bin/sh

cd ./build && cmake "-DCMAKE_TOOLCHAIN_FILE=/Users/MatthewKosarek/vcpkg/scripts/buildsystems/vcpkg.cmake" ../ && make && cd ../ && ./build/Matte 
		    
