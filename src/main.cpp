#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <string_view>

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

int main(int argc, char *argv[]) {
    if (argc - 1 == 0) {
        showHelp();

        std::cerr << "at least one argument\n";
        return 1;
    }
    std::string_view arg = argv[1];
    if (arg == "help") {
        showHelp();
        return 0;
    }

    bool found = false;

    for (const auto &command : commands) {
        if (command.name == arg) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Unknown command: " << arg << '\n';
        showHelp();
        return 1;
    }
    if (!fs::is_directory("src")) {
        std::cerr << "Error: src is not a directory\n";
        return 1;
    }
    if (arg == "build") {
        auto files = bringFiles();

        if (files.empty()) {
            std::cerr << "No .cpp files found\n";
            return 1;
        }

        fs::path output_dir = "target/debug";
        fs::create_directories(output_dir);

        std::string project_name = fs::current_path().filename().string();

        fs::path output = output_dir / project_name;

        std::string command = "clang++ -std=c++20 -Wall -Wextra "
                              "-fsanitize=address,undefined -g ";

        for (const auto &file : files) {
            command += "\"" + file.string() + "\" ";
        }

        command += "-o \"" + output.string() + "\"";

        std::cout << "Compiling project: " << project_name << '\n';

        int result = std::system(command.c_str());

        if (result != 0) {
            std::cerr << "Compilation failed\n";
            return 1;
        }
        std::cout << "Build successful: " << output.string() << '\n';
    }

    if (arg == "run") {
        auto files = bringFiles();

        if (files.empty()) {
            std::cerr << "No .cpp files found\n";
            return 1;
        }

        fs::path output_dir = "target/debug";
        fs::create_directories(output_dir);

        std::string project_name = fs::current_path().filename().string();

        fs::path output = output_dir / project_name;

        std::string command = "clang++ -std=c++20 -Wall -Wextra "
                              "-fsanitize=address,undefined -g ";

        for (const auto &file : files) {
            command += "\"" + file.string() + "\" ";
        }

        command += "-o \"" + output.string() + "\"";

        std::cout << "Compiling project: " << project_name << '\n';

        int result = std::system(command.c_str());

        if (result != 0) {
            std::cerr << "Compilation failed\n";
            return 1;
        }
        std::cout << "Build successful: " << output.string() << '\n';
        command = "\"./" + output.string() + "\"";
        result = std::system(command.c_str());

        // make this a helper later
        if (result != 0) {
            std::cerr << "Running error\n";
            return 1;
        }
    }
    return 0;
}
