#!/bin/bash
echo "Building With g++"
g++-14 -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra tests/testmain.cpp -I ./ -o bin/UniquePtrG_tests && ./bin/UniquePtrG_tests
echo "Building With clang"
clang++-22 -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra tests/testmain.cpp -I ./ -o bin/UniquePtrC_tests && ./bin/UniquePtrC_tests
