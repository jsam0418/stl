#!/bin/bash
echo "Building With g++"
g++-14 -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra main.cpp -o ./bin/UniquePtrG && ./bin/UniquePtrG
echo "Building With clang"
clang++-22 -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra main.cpp -o ./bin/UniquePtrC && ./bin/UniquePtrC
