
#pragma once

#include <iostream>

# define RESET "\e[0m\002"
# define RED "\002\001\e[1m\e[31m"
# define GREEN "\002\001\e[1m\e[32m"
# define YELLOW "\002\001\e[1m\e[33m"
# define BLUE "\002\001\e[1m\e[34m"
# define MAGENTA "\002\001\e[1m\e[35m"
# define CYAN "\002\001\e[1m\e[36m"

// #define RESET "\e[0;37m"
// #define RED "\e[1;31m"
// #define GREEN "\e[1;32m"
// #define YELLOW "\e[1;33m"

#define MAX_PORTS 65536
#define PRIVILEGED_PORTS 1023
#define PRIVATE_PORTS 49152
