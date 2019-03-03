#pragma once

#include <curl/curl.h>

#include <curlpp/info.hpp>
#include <curlpp/opt.hpp>
#include <curlpp/types.hpp>

#include <chrono>
#include <memory>
#include <stdexcept>

namespace curl {
    struct error : std::runtime_error {
        error(CURLcode c)   : std::runtime_error(curl_easy_strerror(c))  {}
        error(CURLMcode c)  : std::runtime_error(curl_multi_strerror(c)) {}
        error(CURLSHcode c) : std::runtime_error(curl_share_strerror(c)) {}
    };

    // TODO: Use system_error with curl category
    CURLcode check(CURLcode c) {
        if (c != CURLE_OK) {
            throw error(c);
        }
        return c;
    }

    const char* version() {
        return curl_version();
    }

    struct global_init {
        global_init(long flags = CURL_GLOBAL_DEFAULT) {
            check(curl_global_init(flags));
        }
        ~global_init() {
            curl_global_cleanup();
        }
    };

    class easy {
        private:
            CURL* curl_;

            template<typename T, typename InfoT>
            T getinfo(InfoT info) {
                T ret;
                check(curl_easy_getinfo(curl_, static_cast<CURLINFO>(info), &ret));
                return ret;
            }
        public:
            easy(CURL* c) : curl_(c) {}
            easy() : easy(curl_easy_init()) {}
            ~easy() { curl_easy_cleanup(curl_); }

            easy(easy&& o) : curl_(std::exchange(o.curl_, nullptr)) {}
            easy& operator=(easy&& o) { std::swap(curl_, o.curl_); return *this; }

            easy(const easy&) = delete;
            easy& operator=(const easy&) = delete;

            void perform() {
                check(curl_easy_perform(curl_));
            }
            
            void reset() {
                curl_easy_reset(curl_);
            }

            // curl_easy_duphandle
            easy duphandle() {
                CURL* new_c = curl_easy_duphandle(curl_);
                if (!new_c)
                    throw std::runtime_error("curl::easy::duphandle failed");
                return easy(new_c);
            }

            // curl_easy_setopt
            void setopt(opt::strings opt, const char* val) {
                check(curl_easy_setopt(curl_, static_cast<CURLoption>(opt), val));
            }
            void setopt(opt::longs opt, long val) {
                check(curl_easy_setopt(curl_, static_cast<CURLoption>(opt), val));
            }
            void setopt(opt::bools opt, boolean val) {
                check(curl_easy_setopt(curl_, static_cast<CURLoption>(opt), static_cast<long>(val)));
            }
            void setopt(opt::off_ts opt, curl_off_t val) {
                check(curl_easy_setopt(curl_, static_cast<CURLoption>(opt), val));
            }

            // curl_easy_getinfo
            long getinfo(info::longs info) {
                return getinfo<long>(info);
            }
            bool getinfo(info::bools info) {
                return getinfo<long>(info);
            }
            protocol getinfo(info::protocols info) {
                return static_cast<protocol>(getinfo<long>(info));
            }
            http_version getinfo(info::http_versions info) {
                return static_cast<http_version>(getinfo<long>(info));
            }
            curl_off_t getinfo(info::off_ts info) {
                return getinfo<curl_off_t>(info);
            }
            slist getinfo(info::slists info) {
                return slist(getinfo<curl_slist*>(info));
            }
            std::chrono::duration<curl_off_t, std::micro> getinfo(info::times info) {
                return std::chrono::duration<curl_off_t, std::micro>(getinfo<curl_off_t>(info));
            }
            const char* getinfo(info::strings info) {
                return getinfo<char*>(info);
            }
    };
}
