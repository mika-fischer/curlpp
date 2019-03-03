#pragma once

#include <curl/curl.h>
#include <iterator>
#include <cstddef>

namespace curl {
    enum class protocol {
        DICT    = CURLPROTO_DICT,
        FILE    = CURLPROTO_FILE,
        FTP     = CURLPROTO_FTP,
        FTPS    = CURLPROTO_FTPS,
        GOPHER  = CURLPROTO_GOPHER,
        HTTP    = CURLPROTO_HTTP,
        HTTPS   = CURLPROTO_HTTPS,
        IMAP    = CURLPROTO_IMAP,
        IMAPS   = CURLPROTO_IMAPS,
        LDAP    = CURLPROTO_LDAP,
        LDAPS   = CURLPROTO_LDAPS,
        POP3    = CURLPROTO_POP3,
        POP3S   = CURLPROTO_POP3S,
        RTMP    = CURLPROTO_RTMP,
        RTMPE   = CURLPROTO_RTMPE,
        RTMPS   = CURLPROTO_RTMPS,
        RTMPT   = CURLPROTO_RTMPT,
        RTMPTR  = CURLPROTO_RTMPTE,
        RTMPTS  = CURLPROTO_RTMPTS,
        RTSP    = CURLPROTO_RTSP,
        SCP     = CURLPROTO_SCP,
        SFTP    = CURLPROTO_SFTP,
        SMB     = CURLPROTO_SMB,
        SMBS    = CURLPROTO_SMBS,
        SMTP    = CURLPROTO_SMTP,
        SMTPS   = CURLPROTO_SMTPS,
        TELNET  = CURLPROTO_TELNET,
        TFTP    = CURLPROTO_TFTP,
    };
    enum class http_version {
        NONE                = CURL_HTTP_VERSION_NONE,
        V_1_0               = CURL_HTTP_VERSION_1_0,
        V_1_1               = CURL_HTTP_VERSION_1_1,
        V_2_0               = CURL_HTTP_VERSION_2_0,
        V_2TLS              = CURL_HTTP_VERSION_2TLS,
        V_2_PRIOR_KNOWLEDGE = CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE,
    };

    class slist_iterator {
        private:
            curl_slist* node = nullptr;

        public:
            using difference_type   = std::ptrdiff_t;
            using value_type        = const char*;
            using pointer           = const char* const*;
            using reference         = const char* const&;
            using iterator_category = std::forward_iterator_tag;

            slist_iterator() = default;
            explicit slist_iterator(curl_slist* list) : node(list) {}

            slist_iterator& operator++() {
                node = node->next;
                return *this;
            }
            slist_iterator& operator++(int) {
                slist_iterator copy(*this);
                node = node->next;
                return copy; 
            }
            bool operator==(const slist_iterator& rhs) const {
                return node == rhs.node;
            }
            bool operator!=(const slist_iterator& rhs) const {
                return node != rhs.node;
            }
            reference operator*() const {
                return node->data;
            }
            reference& operator->() const {
                return node->data;
            }
    };

    class slist_view {
        protected:
            curl_slist* list_ = nullptr;
        public:
            slist_view() = default;
            slist_view(curl_slist* list) : list_(list) {}
            slist_iterator begin() const { return slist_iterator(list_); }
            slist_iterator end()   const { return slist_iterator(); }
    };
   
    class slist : public slist_view {
        public:
            slist() = default;
            slist(curl_slist* list) : slist_view(list) {}
            ~slist() { curl_slist_free_all(list_); }

            slist(slist&& o) : slist_view(std::exchange(o.list_, nullptr)) {}
            slist& operator=(slist&& o) { std::swap(list_, o.list_); return *this; }

            slist(const slist&) = delete;
            slist& operator=(const slist&) = delete;

            void append(const char* s) {
                curl_slist* new_list = curl_slist_append(list_, s);
                if (!new_list)
                    throw std::runtime_error("curl::slist::append failed");
                list_ = new_list;
            }
            void append(const std::string& s) {
                append(s.c_str());
            }
    };

    struct curl_string_deleter {
        void operator()(char* ptr) {
            curl_free(ptr);
        }
    };
    using curl_string = std::unique_ptr<char, curl_string_deleter>;
}