#include <CLI/CLI.hpp>
#include <fstream>
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

int main(int argc, char **argv) {
    CLI::App app;

    // Define options
    std::string app_name;
    CLI::App *new_app = create_subcommand(app, "new", app_name, "Create New App");    

    CLI::App *server = create_subcommand(app, "server", "Running Server");
    CLI::App *build = create_subcommand(app, "build", "Build App");

    CLI::App *generate_command = app.add_subcommand("generate", "Generate New File");
    generate_command->require_subcommand(0,1);

    std::vector<std::string> controller_name;
    std::vector<std::string> component_name;
    std::vector<std::string> route_name;

    CLI::App *new_controller = create_generate_command(generate_command, "controller", controller_name, "Create New Controller");
    CLI::App *new_component = create_generate_command(generate_command, "component", component_name, "Create New Component");
    CLI::App *new_route = create_generate_command(generate_command, "route", route_name, "Create New Route");

    CLI11_PARSE(app, argc, argv);

    const std::string path = bin_path();

    create_new_app(app, new_app, app_name, path);

    run_subcommand(app, server, "Running Server", "g++ main.cpp cpp-httplib/httplib.h -pthread && ./a.out");
    run_subcommand(app, build, "Build App", "cd static && yarn build && cd .. && g++ main.cpp cpp-httplib/httplib.h -pthread");

    std::array<std::string, 4> controller_contents = {"Created New Controller ", "templates/controllers/template_controller.js", "static/src/controllers/", "_controller.js"};
    std::array<std::string, 5> component_contents = {"Created New Component ", "static/src/components", "templates/components/template.svelte", "static/src/components/", ".svelte"};
    std::array<std::string, 5> route_contents = {"Created New Route ", "static/src/routes", "templates/routes/template.svelte", "static/src/routes/", ".svelte"};

    run_subcommand(generate_command, new_controller, path, controller_name, controller_contents);
    run_subcommand(generate_command, new_component, path, component_name, component_contents);
    run_subcommand(generate_command, new_route, path, route_name, route_contents);

    return 0;
}