#include <curlpp/curlpp.hpp>
#include <iostream>

int main() {
    try {
        curl::global_init cgi;
        std::cout << curl::version() << "\n";

        curl::slist headers;
        headers.append("FOO");

        curl::easy conn;
        conn.setopt(curl::opt::URL, "http://httpbin.org/get");
        conn.setopt(curl::opt::USERAGENT, curl::version());
        conn.perform();

        std::cout << "URL: " << conn.getinfo(curl::info::EFFECTIVE_URL) << "\n";
        std::cout << "RESPONSE_CODE: " << conn.getinfo(curl::info::RESPONSE_CODE) << "\n";
        std::cout << "TOTAL: " << conn.getinfo(curl::info::TOTAL_TIME).count() / 1000 << "\n";
        std::cout << "UP   SIZE:  " << conn.getinfo(curl::info::SIZE_UPLOAD) << "\n";
        std::cout << "UP   SPEED: " << conn.getinfo(curl::info::SPEED_UPLOAD) << "\n";
        std::cout << "DOWN SIZE:  " << conn.getinfo(curl::info::SIZE_DOWNLOAD) << "\n";
        std::cout << "DOWN SPEED: " << conn.getinfo(curl::info::SPEED_DOWNLOAD) << "\n";

        auto conn2 = conn.duphandle();
        conn2.perform();

        conn.reset();
        conn.perform();
    }
    catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
}