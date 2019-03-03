#include <curlpp/curlpp.hpp>
#include <iostream>

int main() {
    try {
        curl::global_init cgi;
        std::cout << curl::version() << "\n";

        curl::slist headers;
        headers.append("FOO");

        curl::easy conn;

        {
            using namespace curl::opt;
            conn.setopt(URL, "https://httpbin.org/get");
            conn.setopt(USERAGENT, curl::version());
        }

        for (int i=0; i<2; ++i) {
            conn.perform();

            using namespace curl::info;
            std::cout << "Protocol:      " << (int)conn.getinfo(PROTOCOL) << "\n";
            std::cout << "HTTP Version:  " << (int)conn.getinfo(HTTP_VERSION) << "\n";
            std::cout << "URL:           " << conn.getinfo(EFFECTIVE_URL) << "\n";
            std::cout << "RESPONSE_CODE: " << conn.getinfo(RESPONSE_CODE) << "\n";
            std::cout << "TOTAL:         " << conn.getinfo(TOTAL_TIME).count() / 1000 << "\n";
            std::cout << "UP   SIZE:     " << conn.getinfo(SIZE_UPLOAD) << "\n";
            std::cout << "UP   SPEED:    " << conn.getinfo(SPEED_UPLOAD) << "\n";
            std::cout << "DOWN SIZE:     " << conn.getinfo(SIZE_DOWNLOAD) << "\n";
            std::cout << "DOWN SPEED:    " << conn.getinfo(SPEED_DOWNLOAD) << "\n";
        }
    }
    catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
}