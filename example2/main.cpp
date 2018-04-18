#include <iostream>
#include <time.h>

int InitWindow();
int main(int argn, char *argv[]) {

    //if (argn < 2) {
    //    std::cout << "usage: example2 [netmesh path] [obj path]" << std::endl;
    //    return 1;
    //}

    srand(unsigned int(time(0)));

    InitWindow();

    return 0;
}