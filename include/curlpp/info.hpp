#pragma once

#include <curl/curl.h>

namespace curl {
    namespace info {
        enum times {
            NAMELOOKUP_TIME         = CURLINFO_NAMELOOKUP_TIME_T,
            CONNECT_TIME            = CURLINFO_CONNECT_TIME_T,
            APPCONNECT_TIME         = CURLINFO_APPCONNECT_TIME_T,
            PRETRANSFER_TIME        = CURLINFO_PRETRANSFER_TIME_T,
            STARTTRANSFER_TIME      = CURLINFO_STARTTRANSFER_TIME_T,
            TOTAL_TIME              = CURLINFO_TOTAL_TIME_T,
            REDIRECT_TIME           = CURLINFO_REDIRECT_TIME_T,
        };
        enum strings {
            EFFECTIVE_URL           = CURLINFO_EFFECTIVE_URL,
            CONTENT_TYPE            = CURLINFO_CONTENT_TYPE,
            FTP_ENTRY_PATH          = CURLINFO_FTP_ENTRY_PATH,
            REDIRECT_URL            = CURLINFO_REDIRECT_URL,
            PRIMARY_IP              = CURLINFO_PRIMARY_IP,
            RTSP_SESSION_ID         = CURLINFO_RTSP_SESSION_ID,
            LOCAL_IP                = CURLINFO_LOCAL_IP,
            SCHEME                  = CURLINFO_SCHEME,
        };
        enum longs {
            RESPONSE_CODE           = CURLINFO_RESPONSE_CODE,
            HEADER_SIZE             = CURLINFO_HEADER_SIZE,
            REQUEST_SIZE            = CURLINFO_REQUEST_SIZE,
            SSL_VERIFYRESULT        = CURLINFO_SSL_VERIFYRESULT,
            REDIRECT_COUNT          = CURLINFO_REDIRECT_COUNT,
            HTTP_CONNECTCODE        = CURLINFO_HTTP_CONNECTCODE,
            OS_ERRNO                = CURLINFO_OS_ERRNO,
            NUM_CONNECTS            = CURLINFO_NUM_CONNECTS,
            RTSP_CLIENT_CSEQ        = CURLINFO_RTSP_CLIENT_CSEQ,
            RTSP_SERVER_CSEQ        = CURLINFO_RTSP_SERVER_CSEQ,
            RTSP_CSEQ_RECV          = CURLINFO_RTSP_CSEQ_RECV,
            PRIMARY_PORT            = CURLINFO_PRIMARY_PORT,
            LOCAL_PORT              = CURLINFO_LOCAL_PORT,
            PROXY_SSL_VERIFYRESULT  = CURLINFO_PROXY_SSL_VERIFYRESULT,

            HTTPAUTH_AVAIL          = CURLINFO_HTTPAUTH_AVAIL,  // TODO: bitmask
            PROXYAUTH_AVAIL         = CURLINFO_PROXYAUTH_AVAIL, // TODO: bitmask
        };
        enum bools {
            CONDITION_UNMET         = CURLINFO_CONDITION_UNMET,
        };
        enum protocols {
            PROTOCOL                = CURLINFO_PROTOCOL,
        };
        enum http_versions {
            HTTP_VERSION            = CURLINFO_HTTP_VERSION,
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
            SSL_ENGINES             = CURLINFO_SSL_ENGINES,
            CURLINFO_COOKIELIST     = CURLINFO_COOKIELIST,
        };
        enum ptrs {
            CERTINFO                = CURLINFO_CERTINFO,    // TODO
            TLS_SSL_PTR             = CURLINFO_TLS_SSL_PTR, // TODO
        };
    }
}