#!/bin/bash
g++ -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra main.cpp && ./a.out
#clang++ -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra main.cpp && ./a.out
