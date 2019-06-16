#include "cpp-httplib/httplib.h"

int main() {

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res){
        res.set_content("Hello World!", "text/html");
    });

    svr.listen("0.0.0.0", 8080);
}