#pragma once

#include <curl/curl.h>

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
    
    template <auto fn>
    using deleter_from_fn = std::integral_constant<decltype(fn), fn>;

    template <typename T, auto fn>
    using unique_ptr_deleter = std::unique_ptr<T, deleter_from_fn<fn>>;

    // TODO: Accessor, iteration, etc.
    class slist {
        private:
            using holder = unique_ptr_deleter<curl_slist, curl_slist_free_all>;
            holder list;

        public:
            slist() {}
            slist(curl_slist* s) : list(s) {}
            void append(const char* s) {
                if (holder new_list(curl_slist_append(list.get(), s)); new_list) {
                    list.release();
                    list = std::move(new_list);
                }
                else {
                    throw std::runtime_error("curl::slist::append failed");
                }
            }
            void append(const std::string& s) {
                append(s.c_str());
            }
    };

    namespace opt {
        enum strings {
            URL = CURLOPT_URL,
            PROXY = CURLOPT_PROXY,
            USERPWD = CURLOPT_USERPWD,
            PROXYUSERPWD = CURLOPT_PROXYUSERPWD,
            RANGE = CURLOPT_RANGE,
            REFERER = CURLOPT_REFERER,
            FTPPORT = CURLOPT_FTPPORT,
            USERAGENT = CURLOPT_USERAGENT,
            COOKIE = CURLOPT_COOKIE,
            SSLCERT = CURLOPT_SSLCERT,
            KEYPASSWD = CURLOPT_KEYPASSWD,
            COOKIEFILE = CURLOPT_COOKIEFILE,
            CUSTOMREQUEST = CURLOPT_CUSTOMREQUEST,
            INTERFACE = CURLOPT_INTERFACE,
            KRBLEVEL = CURLOPT_KRBLEVEL,
            CAINFO = CURLOPT_CAINFO,
            RANDOM_FILE = CURLOPT_RANDOM_FILE,
            EGDSOCKET = CURLOPT_EGDSOCKET,
            COOKIEJAR = CURLOPT_COOKIEJAR,
            SSL_CIPHER_LIST = CURLOPT_SSL_CIPHER_LIST,
            SSLCERTTYPE = CURLOPT_SSLCERTTYPE,
            SSLKEY = CURLOPT_SSLKEY,
            SSLKEYTYPE = CURLOPT_SSLKEYTYPE,
            SSLENGINE = CURLOPT_SSLENGINE,
            CAPATH = CURLOPT_CAPATH,
            ACCEPT_ENCODING = CURLOPT_ACCEPT_ENCODING,
            NETRC_FILE = CURLOPT_NETRC_FILE,
            FTP_ACCOUNT = CURLOPT_FTP_ACCOUNT,
            COOKIELIST = CURLOPT_COOKIELIST,
            FTP_ALTERNATIVE_TO_USER = CURLOPT_FTP_ALTERNATIVE_TO_USER,
            SSH_PUBLIC_KEYFILE = CURLOPT_SSH_PUBLIC_KEYFILE,
            SSH_PRIVATE_KEYFILE = CURLOPT_SSH_PRIVATE_KEYFILE,
            SSH_HOST_PUBLIC_KEY_MD5 = CURLOPT_SSH_HOST_PUBLIC_KEY_MD5,
            CRLFILE = CURLOPT_CRLFILE,
            ISSUERCERT = CURLOPT_ISSUERCERT,
            USERNAME = CURLOPT_USERNAME,
            PASSWORD = CURLOPT_PASSWORD,
            PROXYUSERNAME = CURLOPT_PROXYUSERNAME,
            PROXYPASSWORD = CURLOPT_PROXYPASSWORD,
            NOPROXY = CURLOPT_NOPROXY,
            SSH_KNOWNHOSTS = CURLOPT_SSH_KNOWNHOSTS,
            MAIL_FROM = CURLOPT_MAIL_FROM,
            RTSP_SESSION_ID = CURLOPT_RTSP_SESSION_ID,
            RTSP_STREAM_URI = CURLOPT_RTSP_STREAM_URI,
            RTSP_TRANSPORT = CURLOPT_RTSP_TRANSPORT,
            TLSAUTH_USERNAME = CURLOPT_TLSAUTH_USERNAME,
            TLSAUTH_PASSWORD = CURLOPT_TLSAUTH_PASSWORD,
            TLSAUTH_TYPE = CURLOPT_TLSAUTH_TYPE,
            DNS_SERVERS = CURLOPT_DNS_SERVERS,
            MAIL_AUTH = CURLOPT_MAIL_AUTH,
            XOAUTH2_BEARER = CURLOPT_XOAUTH2_BEARER,
            DNS_INTERFACE = CURLOPT_DNS_INTERFACE,
            DNS_LOCAL_IP4 = CURLOPT_DNS_LOCAL_IP4,
            DNS_LOCAL_IP6 = CURLOPT_DNS_LOCAL_IP6,
            LOGIN_OPTIONS = CURLOPT_LOGIN_OPTIONS,
            PINNEDPUBLICKEY = CURLOPT_PINNEDPUBLICKEY,
            UNIX_SOCKET_PATH = CURLOPT_UNIX_SOCKET_PATH,
            PROXY_SERVICE_NAME = CURLOPT_PROXY_SERVICE_NAME,
            SERVICE_NAME = CURLOPT_SERVICE_NAME,
            DEFAULT_PROTOCOL = CURLOPT_DEFAULT_PROTOCOL,
            PROXY_CAINFO = CURLOPT_PROXY_CAINFO,
            PROXY_CAPATH = CURLOPT_PROXY_CAPATH,
            PROXY_TLSAUTH_USERNAME = CURLOPT_PROXY_TLSAUTH_USERNAME,
            PROXY_TLSAUTH_PASSWORD = CURLOPT_PROXY_TLSAUTH_PASSWORD,
            PROXY_TLSAUTH_TYPE = CURLOPT_PROXY_TLSAUTH_TYPE,
            PROXY_SSLCERT = CURLOPT_PROXY_SSLCERT,
            PROXY_SSLCERTTYPE = CURLOPT_PROXY_SSLCERTTYPE,
            PROXY_SSLKEY = CURLOPT_PROXY_SSLKEY,
            PROXY_SSLKEYTYPE = CURLOPT_PROXY_SSLKEYTYPE,
            PROXY_KEYPASSWD = CURLOPT_PROXY_KEYPASSWD,
            PROXY_SSL_CIPHER_LIST = CURLOPT_PROXY_SSL_CIPHER_LIST,
            PROXY_CRLFILE = CURLOPT_PROXY_CRLFILE,
            PRE_PROXY = CURLOPT_PRE_PROXY,
            PROXY_PINNEDPUBLICKEY = CURLOPT_PROXY_PINNEDPUBLICKEY,
            ABSTRACT_UNIX_SOCKET = CURLOPT_ABSTRACT_UNIX_SOCKET,
            REQUEST_TARGET = CURLOPT_REQUEST_TARGET,
            TLS13_CIPHERS = CURLOPT_TLS13_CIPHERS,
            PROXY_TLS13_CIPHERS = CURLOPT_PROXY_TLS13_CIPHERS,
            DOH_URL = CURLOPT_DOH_URL,
        };
        enum longs {
            PORT = CURLOPT_PORT,
            TIMEOUT = CURLOPT_TIMEOUT,
            INFILESIZE = CURLOPT_INFILESIZE,
            LOW_SPEED_LIMIT = CURLOPT_LOW_SPEED_LIMIT,
            LOW_SPEED_TIME = CURLOPT_LOW_SPEED_TIME,
            RESUME_FROM = CURLOPT_RESUME_FROM,
            SSLVERSION = CURLOPT_SSLVERSION,
            TIMECONDITION = CURLOPT_TIMECONDITION,
            TIMEVALUE = CURLOPT_TIMEVALUE,
            PROXYPORT = CURLOPT_PROXYPORT,
            POSTFIELDSIZE = CURLOPT_POSTFIELDSIZE,
            MAXREDIRS = CURLOPT_MAXREDIRS,
            FILETIME = CURLOPT_FILETIME,
            MAXCONNECTS = CURLOPT_MAXCONNECTS,
            CONNECTTIMEOUT = CURLOPT_CONNECTTIMEOUT,
            HTTP_VERSION = CURLOPT_HTTP_VERSION,
            SSLENGINE_DEFAULT = CURLOPT_SSLENGINE_DEFAULT,
            DNS_CACHE_TIMEOUT = CURLOPT_DNS_CACHE_TIMEOUT,
            BUFFERSIZE = CURLOPT_BUFFERSIZE,
            PROXYTYPE = CURLOPT_PROXYTYPE,
            HTTPAUTH = CURLOPT_HTTPAUTH, // bitmask
            PROXYAUTH = CURLOPT_PROXYAUTH, // bitmask
            FTP_RESPONSE_TIMEOUT = CURLOPT_FTP_RESPONSE_TIMEOUT,
            IPRESOLVE = CURLOPT_IPRESOLVE,
            MAXFILESIZE = CURLOPT_MAXFILESIZE,
            USE_SSL = CURLOPT_USE_SSL, // enum
            FTPSSLAUTH = CURLOPT_FTPSSLAUTH,
            FTP_FILEMETHOD = CURLOPT_FTP_FILEMETHOD,
            LOCALPORT = CURLOPT_LOCALPORT,
            LOCALPORTRANGE = CURLOPT_LOCALPORTRANGE,
            SSL_SESSIONID_CACHE = CURLOPT_SSL_SESSIONID_CACHE,
            SSH_AUTH_TYPES = CURLOPT_SSH_AUTH_TYPES,
            FTP_SSL_CCC = CURLOPT_FTP_SSL_CCC,
            TIMEOUT_MS = CURLOPT_TIMEOUT_MS,
            CONNECTTIMEOUT_MS = CURLOPT_CONNECTTIMEOUT_MS,
            NEW_FILE_PERMS = CURLOPT_NEW_FILE_PERMS,
            NEW_DIRECTORY_PERMS = CURLOPT_NEW_DIRECTORY_PERMS,
            // TODO: CHeck for more bools
            POSTREDIR = CURLOPT_POSTREDIR,
            PROXY_TRANSFER_MODE = CURLOPT_PROXY_TRANSFER_MODE,
            ADDRESS_SCOPE = CURLOPT_ADDRESS_SCOPE,
            CERTINFO = CURLOPT_CERTINFO,
            TFTP_BLKSIZE = CURLOPT_TFTP_BLKSIZE,
            SOCKS5_GSSAPI_NEC = CURLOPT_SOCKS5_GSSAPI_NEC,
            PROTOCOLS = CURLOPT_PROTOCOLS,
            REDIR_PROTOCOLS = CURLOPT_REDIR_PROTOCOLS,
            FTP_USE_PRET = CURLOPT_FTP_USE_PRET,
            RTSP_REQUEST = CURLOPT_RTSP_REQUEST,
            RTSP_CLIENT_CSEQ = CURLOPT_RTSP_CLIENT_CSEQ,
            RTSP_SERVER_CSEQ = CURLOPT_RTSP_SERVER_CSEQ,
            WILDCARDMATCH = CURLOPT_WILDCARDMATCH,
            TRANSFER_ENCODING = CURLOPT_TRANSFER_ENCODING,
            GSSAPI_DELEGATION = CURLOPT_GSSAPI_DELEGATION,
            ACCEPTTIMEOUT_MS = CURLOPT_ACCEPTTIMEOUT_MS,
            C_TCP_KEEPALIVE = CURLOPT_TCP_KEEPALIVE,
            C_TCP_KEEPIDLE = CURLOPT_TCP_KEEPIDLE,
            C_TCP_KEEPINTVL = CURLOPT_TCP_KEEPINTVL,
            SSL_OPTIONS = CURLOPT_SSL_OPTIONS,
            SASL_IR = CURLOPT_SASL_IR,
            SSL_ENABLE_NPN = CURLOPT_SSL_ENABLE_NPN,
            SSL_ENABLE_ALPN = CURLOPT_SSL_ENABLE_ALPN,
            EXPECT_100_TIMEOUT_MS = CURLOPT_EXPECT_100_TIMEOUT_MS,
            HEADEROPT = CURLOPT_HEADEROPT,
            SSL_VERIFYSTATUS = CURLOPT_SSL_VERIFYSTATUS,
            SSL_FALSESTART = CURLOPT_SSL_FALSESTART,
            PATH_AS_IS = CURLOPT_PATH_AS_IS,
            PIPEWAIT = CURLOPT_PIPEWAIT,
            STREAM_WEIGHT = CURLOPT_STREAM_WEIGHT,
            TFTP_NO_OPTIONS = CURLOPT_TFTP_NO_OPTIONS,
            C_TCP_FASTOPEN = CURLOPT_TCP_FASTOPEN,
            KEEP_SENDING_ON_ERROR = CURLOPT_KEEP_SENDING_ON_ERROR,
            PROXY_SSL_VERIFYPEER = CURLOPT_PROXY_SSL_VERIFYPEER,
            PROXY_SSL_VERIFYHOST = CURLOPT_PROXY_SSL_VERIFYHOST,
            PROXY_SSLVERSION = CURLOPT_PROXY_SSLVERSION,
            PROXY_SSL_OPTIONS = CURLOPT_PROXY_SSL_OPTIONS,
            SUPPRESS_CONNECT_HEADERS = CURLOPT_SUPPRESS_CONNECT_HEADERS,
            SOCKS5_AUTH = CURLOPT_SOCKS5_AUTH,
            SSH_COMPRESSION = CURLOPT_SSH_COMPRESSION,
            HAPPY_EYEBALLS_TIMEOUT_MS = CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS,
            HAPROXYPROTOCOL = CURLOPT_HAPROXYPROTOCOL,
            DNS_SHUFFLE_ADDRESSES = CURLOPT_DNS_SHUFFLE_ADDRESSES,
            DISALLOW_USERNAME_IN_URL = CURLOPT_DISALLOW_USERNAME_IN_URL,
            UPLOAD_BUFFERSIZE = CURLOPT_UPLOAD_BUFFERSIZE,
            UPKEEP_INTERVAL_MS = CURLOPT_UPKEEP_INTERVAL_MS,
            HTTP09_ALLOWED = CURLOPT_HTTP09_ALLOWED,
        };
        enum bools {
            CRLF                    = CURLOPT_CRLF,
            VERBOSE                 = CURLOPT_VERBOSE,
            HEADER                  = CURLOPT_HEADER,
            NOPROGRESS              = CURLOPT_NOPROGRESS,
            NOBODY                  = CURLOPT_NOBODY,
            FAILONERROR             = CURLOPT_FAILONERROR,
            UPLOAD                  = CURLOPT_UPLOAD,
            POST                    = CURLOPT_POST,
            DIRLISTONLY             = CURLOPT_DIRLISTONLY,
            APPEND                  = CURLOPT_APPEND,
            NETRC                   = CURLOPT_NETRC,
            FOLLOWLOCATION          = CURLOPT_FOLLOWLOCATION,
            TRANSFERTEXT            = CURLOPT_TRANSFERTEXT,
            PUT                     = CURLOPT_PUT,
            AUTOREFERER             = CURLOPT_AUTOREFERER,
            HTTPPROXYTUNNEL         = CURLOPT_HTTPPROXYTUNNEL,
            SSL_VERIFYPEER          = CURLOPT_SSL_VERIFYPEER,
            FRESH_CONNECT           = CURLOPT_FRESH_CONNECT,
            FORBID_REUSE            = CURLOPT_FORBID_REUSE,
            HTTPGET                 = CURLOPT_HTTPGET,
            SSL_VERIFYHOST          = CURLOPT_SSL_VERIFYHOST,
            FTP_USE_EPSV            = CURLOPT_FTP_USE_EPSV,
            COOKIESESSION           = CURLOPT_COOKIESESSION,
            NOSIGNAL                = CURLOPT_NOSIGNAL,
            UNRESTRICTED_AUTH       = CURLOPT_UNRESTRICTED_AUTH,
            FTP_USE_EPRT            = CURLOPT_FTP_USE_EPRT,
            FTP_CREATE_MISSING_DIRS = CURLOPT_FTP_CREATE_MISSING_DIRS,
            C_TCP_NODELAY           = CURLOPT_TCP_NODELAY,
            IGNORE_CONTENT_LENGTH   = CURLOPT_IGNORE_CONTENT_LENGTH,
            FTP_SKIP_PASV_IP        = CURLOPT_FTP_SKIP_PASV_IP,
            CONNECT_ONLY            = CURLOPT_CONNECT_ONLY,
            HTTP_TRANSFER_DECODING  = CURLOPT_HTTP_TRANSFER_DECODING,
            HTTP_CONTENT_DECODING   = CURLOPT_HTTP_CONTENT_DECODING,
        };
        enum off_ts {
            INFILESIZE_LARGE     = CURLOPT_INFILESIZE_LARGE,
            RESUME_FROM_LARGE    = CURLOPT_RESUME_FROM_LARGE,
            MAXFILESIZE_LARGE    = CURLOPT_MAXFILESIZE_LARGE,
            POSTFIELDSIZE_LARGE  = CURLOPT_POSTFIELDSIZE_LARGE,
            MAX_SEND_SPEED_LARGE = CURLOPT_MAX_SEND_SPEED_LARGE,
            MAX_RECV_SPEED_LARGE = CURLOPT_MAX_RECV_SPEED_LARGE,
            TIMEVALUE_LARGE      = CURLOPT_TIMEVALUE_LARGE,
        };

        enum functions {
            WRITEFUNCTION = CURLOPT_WRITEFUNCTION,
            READFUNCTION = CURLOPT_READFUNCTION,
            PROGRESSFUNCTION = CURLOPT_PROGRESSFUNCTION,
            HEADERFUNCTION = CURLOPT_HEADERFUNCTION,
            DEBUGFUNCTION = CURLOPT_DEBUGFUNCTION,
            SSL_CTX_FUNCTION = CURLOPT_SSL_CTX_FUNCTION,
            IOCTLFUNCTION = CURLOPT_IOCTLFUNCTION,
            CONV_FROM_NETWORK_FUNCTION = CURLOPT_CONV_FROM_NETWORK_FUNCTION,
            CONV_TO_NETWORK_FUNCTION = CURLOPT_CONV_TO_NETWORK_FUNCTION,
            CONV_FROM_UTF8_FUNCTION = CURLOPT_CONV_FROM_UTF8_FUNCTION,
            SOCKOPTFUNCTION = CURLOPT_SOCKOPTFUNCTION,
            OPENSOCKETFUNCTION = CURLOPT_OPENSOCKETFUNCTION,
            SEEKFUNCTION = CURLOPT_SEEKFUNCTION,
            SSH_KEYFUNCTION = CURLOPT_SSH_KEYFUNCTION,
            INTERLEAVEFUNCTION = CURLOPT_INTERLEAVEFUNCTION,
            CHUNK_BGN_FUNCTION = CURLOPT_CHUNK_BGN_FUNCTION,
            CHUNK_END_FUNCTION = CURLOPT_CHUNK_END_FUNCTION,
            FNMATCH_FUNCTION = CURLOPT_FNMATCH_FUNCTION,
            CLOSESOCKETFUNCTION = CURLOPT_CLOSESOCKETFUNCTION,
            XFERINFOFUNCTION = CURLOPT_XFERINFOFUNCTION,
            RESOLVER_START_FUNCTION = CURLOPT_RESOLVER_START_FUNCTION,
            TRAILERFUNCTION = CURLOPT_TRAILERFUNCTION,
        };
    }


#if 0
  CINIT(WRITEDATA, OBJECTPOINT, 1),
  CINIT(READDATA, OBJECTPOINT, 9),
  CINIT(HEADERDATA, OBJECTPOINT, 29),
  CINIT(PROGRESSDATA, OBJECTPOINT, 57),
  CINIT(DEBUGDATA, OBJECTPOINT, 95),
  CINIT(SSL_CTX_DATA, OBJECTPOINT, 109),
  CINIT(IOCTLDATA, OBJECTPOINT, 131),
  CINIT(SOCKOPTDATA, OBJECTPOINT, 149),
  CINIT(OPENSOCKETDATA, OBJECTPOINT, 164),
  CINIT(SEEKDATA, OBJECTPOINT, 168),
  CINIT(SSH_KEYDATA, OBJECTPOINT, 185),
  CINIT(INTERLEAVEDATA, OBJECTPOINT, 195),
  CINIT(CHUNK_DATA, OBJECTPOINT, 201),
  CINIT(FNMATCH_DATA, OBJECTPOINT, 202),
  CINIT(CLOSESOCKETDATA, OBJECTPOINT, 209),
  CINIT(RESOLVER_START_DATA, OBJECTPOINT, 273),
  CINIT(TRAILERDATA, OBJECTPOINT, 284),
  
  CINIT(ERRORBUFFER, OBJECTPOINT, 10),
  CINIT(POSTFIELDS, OBJECTPOINT, 15),
  CINIT(HTTPHEADER, OBJECTPOINT, 23),
  CINIT(HTTPPOST, OBJECTPOINT, 24),
  CINIT(QUOTE, OBJECTPOINT, 28),
  CINIT(STDERR, OBJECTPOINT, 37),
  CINIT(POSTQUOTE, OBJECTPOINT, 39),
  CINIT(TELNETOPTIONS, OBJECTPOINT, 70),
  CINIT(PREQUOTE, OBJECTPOINT, 93),
  CINIT(SHARE, OBJECTPOINT, 100),
  CINIT(PRIVATE, OBJECTPOINT, 103),
  CINIT(HTTP200ALIASES, OBJECTPOINT, 104),
  CINIT(COPYPOSTFIELDS, OBJECTPOINT, 165),
  CINIT(MAIL_RCPT, OBJECTPOINT, 187),
  CINIT(RESOLVE, OBJECTPOINT, 203),
  CINIT(PROXYHEADER, OBJECTPOINT, 228),
  CINIT(STREAM_DEPENDS, OBJECTPOINT, 240),
  CINIT(STREAM_DEPENDS_E, OBJECTPOINT, 241),
  CINIT(CONNECT_TO, OBJECTPOINT, 243),
  CINIT(MIMEPOST, OBJECTPOINT, 269),
  CINIT(CURLU, OBJECTPOINT, 282),
#endif

    namespace info {
        enum times {
            NAMELOOKUP_TIME    = CURLINFO_NAMELOOKUP_TIME_T,
            CONNECT_TIME       = CURLINFO_CONNECT_TIME_T,
            APPCONNECT_TIME    = CURLINFO_APPCONNECT_TIME_T,
            PRETRANSFER_TIME   = CURLINFO_PRETRANSFER_TIME_T,
            STARTTRANSFER_TIME = CURLINFO_STARTTRANSFER_TIME_T,
            TOTAL_TIME         = CURLINFO_TOTAL_TIME_T,
            REDIRECT_TIME      = CURLINFO_REDIRECT_TIME_T,
        };
        enum strings {
            EFFECTIVE_URL   = CURLINFO_EFFECTIVE_URL,
            CONTENT_TYPE    = CURLINFO_CONTENT_TYPE,
            FTP_ENTRY_PATH  = CURLINFO_FTP_ENTRY_PATH,
            REDIRECT_URL    = CURLINFO_REDIRECT_URL,
            PRIMARY_IP      = CURLINFO_PRIMARY_IP,
            RTSP_SESSION_ID = CURLINFO_RTSP_SESSION_ID,
            LOCAL_IP        = CURLINFO_LOCAL_IP,
            SCHEME          = CURLINFO_SCHEME,
        };
        enum longs {
            RESPONSE_CODE          = CURLINFO_RESPONSE_CODE,
            HEADER_SIZE            = CURLINFO_HEADER_SIZE,
            REQUEST_SIZE           = CURLINFO_REQUEST_SIZE,
            SSL_VERIFYRESULT       = CURLINFO_SSL_VERIFYRESULT,
            REDIRECT_COUNT         = CURLINFO_REDIRECT_COUNT,
            HTTP_CONNECTCODE       = CURLINFO_HTTP_CONNECTCODE,
            OS_ERRNO               = CURLINFO_OS_ERRNO,
            NUM_CONNECTS           = CURLINFO_NUM_CONNECTS,
            RTSP_CLIENT_CSEQ       = CURLINFO_RTSP_CLIENT_CSEQ,
            RTSP_SERVER_CSEQ       = CURLINFO_RTSP_SERVER_CSEQ,
            RTSP_CSEQ_RECV         = CURLINFO_RTSP_CSEQ_RECV,
            PRIMARY_PORT           = CURLINFO_PRIMARY_PORT,
            LOCAL_PORT             = CURLINFO_LOCAL_PORT,
            PROXY_SSL_VERIFYRESULT = CURLINFO_PROXY_SSL_VERIFYRESULT,

            HTTPAUTH_AVAIL         = CURLINFO_HTTPAUTH_AVAIL, // TODO: bitmask
            PROXYAUTH_AVAIL        = CURLINFO_PROXYAUTH_AVAIL, // TODO: bitmask
            HTTP_VERSION           = CURLINFO_HTTP_VERSION, // TODO: Enum
            PROTOCOL               = CURLINFO_PROTOCOL, // TODO: Enum
        };
        enum bools {
            CONDITION_UNMET        = CURLINFO_CONDITION_UNMET,
        };
        enum off_ts {
            SIZE_UPLOAD             = CURLINFO_SIZE_UPLOAD_T,
            SIZE_DOWNLOAD           = CURLINFO_SIZE_DOWNLOAD_T,
            SPEED_DOWNLOAD          = CURLINFO_SPEED_DOWNLOAD_T,
            SPEED_UPLOAD            = CURLINFO_SPEED_UPLOAD_T,
            FILETIME                = CURLINFO_FILETIME_T,
            CONTENT_LENGTH_DOWNLOAD = CURLINFO_CONTENT_LENGTH_DOWNLOAD_T,
            CONTENT_LENGTH_UPLOAD   = CURLINFO_CONTENT_LENGTH_UPLOAD_T,
        };
        enum slists {
            SSL_ENGINES         = CURLINFO_SSL_ENGINES,
            CURLINFO_COOKIELIST = CURLINFO_COOKIELIST,
        };
        enum ptrs {
            CERTINFO    = CURLINFO_CERTINFO, // TODO
            TLS_SSL_PTR = CURLINFO_TLS_SSL_PTR, // TODO
        };
    }

    class easy {
        private:
            using holder = unique_ptr_deleter<CURL, curl_easy_cleanup>;
            holder handle_;

            easy(holder&& handle) : handle_(std::move(handle)) {}

        public:
            easy() : handle_(curl_easy_init()) {}

            void perform() {
                check(curl_easy_perform(handle_.get()));
            }
            
            void reset() {
                curl_easy_reset(handle_.get());
            }

            easy duphandle() {
                if (holder new_handle(curl_easy_duphandle(handle_.get())); new_handle) {
                    return easy(std::move(new_handle));
                }
                throw std::runtime_error("curl::easy::duphandle failed");
            }

            void setopt(opt::strings opt, const char* val) {
                check(curl_easy_setopt(handle_.get(), (CURLoption)opt, val));
            }
            void setopt(opt::longs opt, long val) {
                check(curl_easy_setopt(handle_.get(), (CURLoption)opt, val));
            }
            void setopt(opt::bools opt, boolean val) {
                check(curl_easy_setopt(handle_.get(), (CURLoption)opt, static_cast<long>(val)));
            }
            void setopt(opt::off_ts opt, curl_off_t val) {
                check(curl_easy_setopt(handle_.get(), (CURLoption)opt, val));
            }

            long getinfo(info::longs info) {
                long ret;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return ret;
            }
            bool getinfo(info::bools info) {
                long ret;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return ret;
            }
            curl_off_t getinfo(info::off_ts info) {
                curl_off_t ret;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return ret;
            }
            slist getinfo(info::slists info) {
                curl_slist* ret = nullptr;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return slist(ret);
            }
            std::chrono::duration<curl_off_t, std::micro> getinfo(info::times info) {
                curl_off_t ret;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return std::chrono::duration<curl_off_t, std::micro>(ret);
            }
            const char* getinfo(info::strings info) {
                char* ret;
                check(curl_easy_getinfo(handle_.get(), (CURLINFO)info, &ret));
                return ret;
            }
    };
}
