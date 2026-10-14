#include <iostream>
#include <array>
#include <string_view>

enum class CommandId { Run, Help };

struct Command {
    CommandId id;
    std::string_view name;
    std::string_view description;
};

constexpr std::array commands{
    Command{CommandId::Run, "run", "build and run the project"},
    Command{CommandId::Help, "help", "show this help"},
};


void showHelp() {
    for (auto command : commands) {
        std::cout << command.name << ": " << command.description << "\n";
    }
}

int main(int argc, char *argv[]) {
    if (argc - 1 == 0) {
        showHelp();

        std::cerr << "at least one argument\n";
        return 1;
    }

    std::string_view arg = argv[1];

    if (arg == "help") {
        showHelp();
    }

    if (arg == "run") {
        //   // looks for src, list the files and collect them in a vec, sort
        //   them, compile them
    }

    return 0;
}
