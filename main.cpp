#include <CLI/CLI.hpp>
#include <fstream> 

int main(int argc, char **argv) {
    CLI::App app;

    // Define options
    CLI::App *new_app = app.add_subcommand("new", "Create New App")->required();
    std::string name;
    new_app->add_option("app", name, "App name");

    CLI11_PARSE(app, argc, argv);

    if (new_app) {
        std::cout << "New App Name is " << name << std::endl;
    }

    return 0;
}