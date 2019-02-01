((nil . ((compile-command . "cd ../build && make && cd ../ && ./build/Matte && cd ./src"))))

((c++-mode
  (eval add-hook 'after-save-hook "cmake \"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON\" \"-DCMAKE_TOOLCHAIN_FILE=/Users/MatthewKosarek/vcpkg/scripts/buildsystems/vcpkg.cmake\" ../")))
