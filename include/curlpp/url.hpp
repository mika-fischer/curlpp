#pragma once

#include <curl/curl.h>

namespace curl {
    const char* curl_url_strerror(CURLUcode code) {
        switch (code) {
            case CURLUE_OK:                  return "OK";
            case CURLUE_BAD_HANDLE:          return "bad handle";
            case CURLUE_BAD_PARTPOINTER:     return "bad part pointer";
            case CURLUE_MALFORMED_INPUT:     return "malformed input";
            case CURLUE_BAD_PORT_NUMBER:     return "bad port number";
            case CURLUE_UNSUPPORTED_SCHEME:  return "unsupported scheme";
            case CURLUE_URLDECODE:           return "urldecode";
            case CURLUE_OUT_OF_MEMORY:       return "out of memory";
            case CURLUE_USER_NOT_ALLOWED:    return "user not allowed";
            case CURLUE_UNKNOWN_PART:        return "unknown part";
            case CURLUE_NO_SCHEME:           return "no scheme";
            case CURLUE_NO_USER:             return "no user";
            case CURLUE_NO_PASSWORD:         return "no password";
            case CURLUE_NO_OPTIONS:          return "no options";
            case CURLUE_NO_HOST:             return "no host";
            case CURLUE_NO_PORT:             return "no port";
            case CURLUE_NO_QUERY:            return "no query";
            case CURLUE_NO_FRAGMENT:         return "no fragment";
        }
    };

    enum class url_part {
        URL,
        SCHEME,
        USER,
        PASSWORD,
        OPTIONS,
        HOST,
        PORT,
        PATH,
        QUERY,
        FRAGMENT,
    };

    class URL {
        private:
            CURLU* url_;
        public:
            URL(CURLU* u) : url_(u) {}
            URL() : URL(curl_url()) {}
            ~URL() { curl_urL_cleanup(url_); }
            
            URL(const URL& o) : url_(curl_url_dup(o.url_)) {}
            URL(URL&& o) : url_(std::exchange(o.url_, nullptr)) {}

            URL& operator=(const URL& o) {}



/*
 * curl_url_get() extracts a specific part of the URL from a CURLU
 * handle. Returns error code. The returned pointer MUST be freed with
 * curl_free() afterwards.
 */
CURL_EXTERN CURLUcode curl_url_get(CURLU *handle, CURLUPart what,
                                   char **part, unsigned int flags);


/*
 * curl_url_set() sets a specific part of the URL in a CURLU handle. Returns
 * error code. The passed in string will be copied. Passing a NULL instead of
 * a part string, clears that part.
 */
CURL_EXTERN CURLUcode curl_url_set(CURLU *handle, CURLUPart what,
                                   const char *part, unsigned int flags);
    };

#define CURLU_DEFAULT_PORT (1<<0)       /* return default port number */
#define CURLU_NO_DEFAULT_PORT (1<<1)    /* act as if no port number was set,
                                           if the port number matches the
                                           default for the scheme */
#define CURLU_DEFAULT_SCHEME (1<<2)     /* return default scheme if
                                           missing */
#define CURLU_NON_SUPPORT_SCHEME (1<<3) /* allow non-supported scheme */
#define CURLU_PATH_AS_IS (1<<4)         /* leave dot sequences */
#define CURLU_DISALLOW_USER (1<<5)      /* no user+password allowed */
#define CURLU_URLDECODE (1<<6)          /* URL decode on get */
#define CURLU_URLENCODE (1<<7)          /* URL encode on set */
#define CURLU_APPENDQUERY (1<<8)        /* append a form style part */
#define CURLU_GUESS_SCHEME (1<<9)       /* legacy curl-style guessing */

