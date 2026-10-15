#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

enum class CommandId { Run, Help, Build };

struct Command {
    CommandId id;
    std::string_view name;
    std::string_view description;
};

constexpr std::array commands{
    Command{CommandId::Run, "run", "build and run the project"},
    Command{CommandId::Help, "help", "show this help"},
    Command{CommandId::Build, "build", "compile the files"},
};

void showHelp() {
    std::cout << "Help table\n\n";
    for (auto command : commands) {
        std::cout << command.name << ": " << command.description << "\n";
    }
}

std::vector<fs::path> bringFiles() {
    std::vector<fs::path> files;
    for (const auto &entry : fs::directory_iterator("src")) {
        if (entry.path().extension() == ".cpp") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

bool noCpp(const std::vector<fs::path> &files) {
    if (files.empty()) {
        std::cerr << "No .cpp files found\n";
        return true;
    }
    return false;
}

std::optional<Command> lookForCommand(std::string_view arg) {
    for (const auto &command : commands) {
        if (command.name == arg) {
            return command;
        }
    }
    return std::nullopt;
}

fs::path createOutput() {
    fs::path output_dir = "target/debug";
    fs::create_directories(output_dir);
    return output_dir / fs::current_path().filename();
}

std::string compileCommand(const std::vector<fs::path> &files,
                           const fs::path &output) {
    std::string command = "clang++ -std=c++20 -Wall -Wextra "
                          "-fsanitize=address,undefined -g ";
    for (const auto &file : files) {
        command += "\"" + file.string() + "\" ";
    }
    command += "-o \"" + output.string() + "\"";
    return command;
}

int build(const fs::path &output) {
    if (!fs::is_directory("src")) {
        std::cerr << "Error: src is not a directory\n";
        return 1;
    }

    auto files = bringFiles();
    if (noCpp(files)) {
        return 1;
    }

    std::cout << "Compiling project: " << output.filename().string() << '\n' << std::flush;

    if (std::system(compileCommand(files, output).c_str()) != 0) {
        std::cerr << "Compilation failed\n";
        return 1;
    }
    std::cout << "Build successful: " << output.string() << '\n' << std::flush;
    return 0;
}

int run(const fs::path &output) {
    if (build(output) != 0) {
        return 1;
    }

    std::string command = "\"./" + output.string() + "\"";
    if (std::system(command.c_str()) != 0) {
        std::cerr << "Running error\n";
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        showHelp();
        std::cerr << "at least one argument\n";
        return 1;
    }

    std::string_view arg = argv[1];
    auto command = lookForCommand(arg);

    if (!command) {
        std::cerr << "Unknown command: " << arg << '\n';
        showHelp();
        return 1;
    }

    switch (command->id) {
        case CommandId::Help:
            showHelp();
            return 0;
        case CommandId::Build:
            return build(createOutput());
        case CommandId::Run:
            return run(createOutput());
    }
    return 0;
}
