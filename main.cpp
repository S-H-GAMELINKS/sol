#include <CLI/CLI.hpp>
#include <regex>
#include <array>
#include <vector>
#include <experimental/filesystem>
#include "main.hpp"

namespace fs = std::experimental::filesystem;

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