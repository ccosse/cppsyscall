#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <memory>

std::vector<std::string> getCommandOutput(const std::string& command) {
    std::vector<std::string> output;
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to run the command passed as argument
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to run command: " + command);
    }

    // Read the output of the command
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result = buffer.data();
        output.push_back(result);
    }

    return output;
}

int main(int argc, char* argv[]) {
    std::string command = "nvidia-smi";
    if (argc > 1) {
        command = argv[1];
    }

    try {
        auto lines = getCommandOutput(command);
        for (const auto& line : lines) {
            std::cout << line;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
