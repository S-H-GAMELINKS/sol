#include <CLI/CLI.hpp>
#include <fstream>
#include <experimental/filesystem>

int main(int argc, char **argv) {
    CLI::App app;

    // Define options
    CLI::App *new_app = app.add_subcommand("new", "Create New App")->required();
    std::string app_name;
    new_app->add_option("app", app_name, "App name");

    CLI11_PARSE(app, argc, argv);

    if (new_app) {
        std::cout << "Created New App " << app_name << std::endl;
        std::experimental::filesystem::copy(std::experimental::filesystem::absolute("sol/src"), app_name);
        std::experimental::filesystem::copy(std::experimental::filesystem::absolute("sol/cpp-httplib"), app_name + "/cpp-httplib");
    }

    return 0;
}