#include <iostream>
#include <array>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

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
    std::cout << "Help table\n\n";
    for (auto command : commands) {
        std::cout << command.name << ": " << command.description << "\n";
    }
}

std::vector<std::string> bringFiles() {
    std::vector<std::string> files;
    for (const auto& entry: fs::directory_iterator("compile_me")) {
        files.push_back(entry.path().string());
    }
    return files;
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
        auto files = bringFiles();

        for (const auto& file : files) {
            std::cout << file << "\n";
        }
    }

    return 0;
}
