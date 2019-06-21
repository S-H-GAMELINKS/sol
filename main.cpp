#include <CLI/CLI.hpp>
#include <fstream>
#include <regex>
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

void run_subcommand(CLI::App& app, CLI::App *sub_command, const std::string& content, const std::string& command) {
    if (app.got_subcommand(sub_command)) {
        std::cout << content << std::endl;
        std::string cmd = command;
        system(cmd.c_str());
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
    CLI::App *new_controller = create_generate_command(generate_command, "controller", controller_name, "Create New Controller");

    std::vector<std::string> component_name;
    CLI::App *new_component = create_generate_command(generate_command, "component", component_name, "Create New Component");

    std::vector<std::string> route_name;
    CLI::App *new_route = create_generate_command(generate_command, "route", route_name, "Create New Route");

    CLI11_PARSE(app, argc, argv);

    const std::string path = bin_path();

    if (app.got_subcommand(new_app)) {
        std::cout << "Created New App " << app_name << std::endl;
        fs::copy(path + "src", app_name, fs::copy_options::recursive);
        fs::copy(path + "cpp-httplib", app_name + "/cpp-httplib");
        std::string cmd = "cd " + app_name + " && cd static && yarn install";
        system(cmd.c_str());
    }

    run_subcommand(app, server, "Running Server", "g++ main.cpp cpp-httplib/httplib.h -pthread && ./a.out");

    run_subcommand(app, build, "Build App", "cd static && yarn build && cd .. && g++ main.cpp cpp-httplib/httplib.h -pthread");

    if (generate_command->got_subcommand(new_controller)) {
        for (auto&& controller : controller_name) {
            std::cout << "Created New Controller " << controller << std::endl;
            fs::copy(path + "templates/controllers/template_controller.js", "static/src/controllers/" + controller + "_controller.js");
        }
    }

    if (generate_command->got_subcommand(new_component)) {
        for (auto&& component : component_name) {
            std::cout << "Created New Component " << component << std::endl;
            if (!fs::exists("static/src/components"))
                fs::create_directories("static/src/components");
            
            fs::copy(path + "templates/components/template.svelte", "static/src/components/" + component + ".svelte");
        }
    }

    if (generate_command->got_subcommand(new_route)) {
        for (auto&& route : route_name) {
            std::cout << "Created New Route " << route << std::endl;
            if (!fs::exists("static/src/routes"))
                fs::create_directories("static/src/routes");
            
            fs::copy(path + "templates/routes/template.svelte", "static/src/routes/" + route + ".svelte");
        }
    }

    return 0;
}