include(FetchContent)

FetchContent_Declare(curl
    URL https://github.com/curl/curl/releases/download/curl-7_64_0/curl-7.64.0.tar.xz
)
FetchContent_GetProperties(curl)
if (NOT curl_POPULATED)
    FetchContent_Populate(curl)
    set(BUILD_CURL_EXE OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(HTTP_ONLY ON)
    add_subdirectory(${curl_SOURCE_DIR} ${curl_BINARY_DIR})
endif ()


FetchContent_Declare(catch
    URL https://github.com/catchorg/Catch2/archive/v2.6.1.tar.gz
)
FetchContent_GetProperties(catch)
if (NOT catch_POPULATED)
    FetchContent_Populate(catch)
    add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif ()

FetchContent_Declare(nlohmann
    URL https://github.com/nlohmann/json/archive/v3.5.0.tar.gz
)
FetchContent_GetProperties(nlohmann)
if (NOT nlohmann_POPULATED)
    FetchContent_Populate(nlohmann)
    set(JSON_BuildTests OFF)
    add_subdirectory(${nlohmann_SOURCE_DIR} ${nlohmann_BINARY_DIR})
endif ()

