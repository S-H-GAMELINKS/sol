#include "cpp-httplib/httplib.h"
#include <fstream>
#include <sstream>
#include <string>

const std::string load_static(const std::string& path) {

    std::ifstream static_file(path.c_str(), std::ios::in);

    std::stringstream stream;

    stream << static_file.rdbuf();
    static_file.close();

    return stream.str();
}

int main() {

    httplib::Server svr;

    const std::string html = load_static("static/index.html"); 

    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res){
        res.set_content(html, "text/html");
    });

    svr.listen("0.0.0.0", 8080);
}