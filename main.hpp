#include <CLI/CLI.hpp>
#include <regex>
#include <array>
#include <vector>
#include <experimental/filesystem>

// Replce namespace for fs
namespace fs = std::experimental::filesystem;

// Get .sol path
const std::string bin_path(){

    fs::path p = "/proc/self/exe";

    return std::regex_replace(fs::read_symlink(p).string<char>(), std::regex("bin/sol"), "");
}

// Create sub command func for args used
CLI::App *create_subcommand(CLI::App& app, const std::string& command, std::string& args, const std::string& content) {

    // Add sub command
    CLI::App *sub_command = app.add_subcommand(command, content);

    // Set option and args
    sub_command->add_option(command, args, content);

    return sub_command;
}

// Create sub command 
CLI::App *create_subcommand(CLI::App& app, const std::string& command, const std::string& content) {

    // Add sub command
    CLI::App *sub_command = app.add_subcommand(command, content);

    // Set command content
    sub_command->add_option(command, content);

    return sub_command;
}

// Add generate sub command func  
CLI::App *create_generate_command(CLI::App *generate, const std::string& command, std::vector<std::string>& args, const std::string& content) {

    // Add sub command
    CLI::App *generate_command = generate->add_subcommand(command, content);

    // Add option and args for sub command
    generate_command->add_option(command, args, content);

    return generate_command;
}

// Create new sol app 
void create_new_app(CLI::App& app, CLI::App *new_app, const std::string& app_name, const std::string& path) {

    // Got sub command? Create new sol app
    if (app.got_subcommand(new_app)) {
        std::cout << "Created New App " << app_name << std::endl;
        fs::copy(path + "src", app_name, fs::copy_options::recursive);
        fs::copy(path + "cpp-httplib", app_name + "/cpp-httplib");
        fs::copy(path + "sqlite_orm", app_name + "/sqlite_orm", fs::copy_options::recursive);
        std::string cmd = "cd " + app_name + " && cd static && yarn install";
        system(cmd.c_str());
    }    
}

// Run sub command 
void run_subcommand(CLI::App& app, CLI::App *sub_command, const std::string& content, const std::string& command) {

    // Run sub command 
    if (app.got_subcommand(sub_command)) {
        std::cout << content << std::endl;
        std::string cmd = command;
        system(cmd.c_str());
    }
}

// Run sub command for args 
void run_subcommand(CLI::App *generate, CLI::App *subcommand, const std::string& path, const std::vector<std::string> args, std::array<std::string, 4> contents) {

    // Run sub command for args 
    if (generate->got_subcommand(subcommand)) {
        // Create args file
        for (auto&& arg : args) {
            std::cout << contents[0] << arg << std::endl;
            fs::copy(path + contents[1], contents[2] + arg + contents[3]);
        }
    }
}

// Run sub command for args(directory exist check)
void run_subcommand(CLI::App *generate, CLI::App *subcommand, const std::string& path, const std::vector<std::string> args, std::array<std::string, 5> contents) {

    // Run sub command for args
    if (generate->got_subcommand(subcommand)) {
        // Create args file
        for (auto&& arg : args) {
            std::cout << contents[0] << arg << std::endl;

            // Directory exist?
            if (!fs::exists(contents[1]))
                fs::create_directories(contents[1]);

            fs::copy(path + contents[2], contents[3] + arg + contents[4]);
        }
    }
}
