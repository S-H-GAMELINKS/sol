#include <CLI/CLI.hpp>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char **argv) {
    CLI::App app;

    // Define options
    CLI::App *new_app = app.add_subcommand("new", "Create New App")->required();
    std::string app_name;
    new_app->add_option("app", app_name, "App name");

    CLI11_PARSE(app, argc, argv);

    if (new_app) {
        std::cout << "Created New App " << app_name << std::endl;
        fs::copy(fs::absolute("sol/src"), app_name);
        fs::copy(fs::absolute("sol/cpp-httplib"), app_name + "/cpp-httplib");
        fs::copy(fs::absolute("sol/src/static"), app_name + "/static");
    }

    return 0;
}