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

CLI::App *create_subcommand(CLI::App& app, const std::string& command, const std::string& content) {

    CLI::App *server = app.add_subcommand(command, content);
    server->add_option(command, content);

    return server;
}

int main(int argc, char **argv) {
    CLI::App app;

    // Define options
    CLI::App *new_app = app.add_subcommand("new", "Create New App");
    std::string app_name;
    new_app->add_option("app", app_name, "App name");

    CLI::App *server = create_subcommand(app, "server", "Running Server");
    CLI::App *build = create_subcommand(app, "build", "Build App");

    CLI::App *generate_command = app.add_subcommand("generate", "Generate New File");
    generate_command->require_subcommand(0,1);

    CLI::App *new_controller = generate_command->add_subcommand("controller", "Create New Controller");
    std::vector<std::string> controller_name;
    new_controller->add_option("controller", controller_name, "Controller name");

    CLI::App *new_component = generate_command->add_subcommand("component", "Create New Component");
    std::vector<std::string> component_name;
    new_component->add_option("component", component_name, "Component name");

    CLI::App *new_route = generate_command->add_subcommand("route", "Create New Route");
    std::vector<std::string> route_name;
    new_route->add_option("route", route_name, "Route name");

    CLI11_PARSE(app, argc, argv);

    const std::string path = bin_path();

    if (app.got_subcommand(new_app)) {
        std::cout << "Created New App " << app_name << std::endl;
        fs::copy(path + "src", app_name, fs::copy_options::recursive);
        fs::copy(path + "cpp-httplib", app_name + "/cpp-httplib");
        std::string cmd = "cd " + app_name + " && cd static && yarn install";
        system(cmd.c_str());
    }

    if (app.got_subcommand(server)) {
        std::cout << "Running Server" << std::endl;
        std::string cmd = "g++ main.cpp cpp-httplib/httplib.h -pthread && ./a.out";
        system(cmd.c_str());
    }

    if (app.got_subcommand(build)) {
        std::cout << "Build App" << std::endl;
        std::string cmd = "cd static && yarn build && cd .. && g++ main.cpp cpp-httplib/httplib.h -pthread";
        system(cmd.c_str());
    }

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