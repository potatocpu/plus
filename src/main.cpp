#include <iostream>
void showHel() {}

int main(int argc, char *argv[]) {
    if (argc - 1 == 0) {
        // show func help
        std::cerr << "at least one argument\n";
    }

    std::string arg = argv[1];

    if (arg == "help") {
        // show help func
    }

    if (arg == "run") {
        //   // looks for src, list the files and collect them in a vec, sort
        //   them, compile them
    }

    return 0;
}
