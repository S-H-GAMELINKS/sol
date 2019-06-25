#include <CLI/CLI.hpp>
#include <regex>
#include <array>
#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

const std::string bin_path(){

    fs::path p = "/proc/self/exe";

    return std::regex_replace(fs::read_symlink(p).string<char>(), std::regex("bin/sol"), "");
}

CLI::App *create_subcommand(CLI::App& app, const std::string& command, std::string& args, const std::string& content) {

    CLI::App *sub_command = app.add_subcommand(command, content);
    sub_command->add_option(command, args, content);

    return sub_command;
}

CLI::App *create_subcommand(CLI::App& app, const std::string& command, const std::string& content) {

    CLI::App *server = app.add_subcommand(command, content);
    server->add_option(command, content);

    return server;
}

CLI::App *create_generate_command(CLI::App *generate, const std::string& command, std::vector<std::string>& args, const std::string& content) {

    CLI::App *generate_command = generate->add_subcommand(command, content);
    generate_command->add_option(command, args, content);

    return generate_command;
}

void create_new_app(CLI::App& app, CLI::App *new_app, const std::string& app_name, const std::string& path) {
    if (app.got_subcommand(new_app)) {
        std::cout << "Created New App " << app_name << std::endl;
        fs::copy(path + "src", app_name, fs::copy_options::recursive);
        fs::copy(path + "cpp-httplib", app_name + "/cpp-httplib");
        fs::copy(path + "sqlite_orm", app_name + "/sqlite_orm", fs::copy_options::recursive);
        std::string cmd = "cd " + app_name + " && cd static && yarn install";
        system(cmd.c_str());
    }    
}

void run_subcommand(CLI::App& app, CLI::App *sub_command, const std::string& content, const std::string& command) {
    if (app.got_subcommand(sub_command)) {
        std::cout << content << std::endl;
        std::string cmd = command;
        system(cmd.c_str());
    }
}

void run_subcommand(CLI::App *generate, CLI::App *subcommand, const std::string& path, const std::vector<std::string> args, std::array<std::string, 4> contents) {
    if (generate->got_subcommand(subcommand)) {
        for (auto&& arg : args) {
            std::cout << contents[0] << arg << std::endl;
            fs::copy(path + contents[1], contents[2] + arg + contents[3]);
        }
    }
}

void run_subcommand(CLI::App *generate, CLI::App *subcommand, const std::string& path, const std::vector<std::string> args, std::array<std::string, 5> contents) {
    if (generate->got_subcommand(subcommand)) {
        for (auto&& arg : args) {
            std::cout << contents[0] << arg << std::endl;
            if (!fs::exists(contents[1]))
                fs::create_directories(contents[1]);

            fs::copy(path + contents[2], contents[3] + arg + contents[4]);
        }
    }
}
