#include <iostream>

int main(int argc, char* argv[]) {
    if (argc - 1 == 0) {
        // by standard write the help
        std::cout << "at least one argument\n";
    }
    if (argv[1] == "run") {
        // whatever
    } else {
        std::cout << "unknown argument\n";
    }
}
