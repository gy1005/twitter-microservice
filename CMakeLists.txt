cmake_minimum_required(VERSION 3.12)

# Using hunter to resolve dependencies.pigpass

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.28.tar.gz"
        SHA1 "6c79b36253a8e8bcba48927e9f3be65a3a81e6ec"
        LOCAL
)


project(twitter_microservice)
set(CMAKE_CXX_STANDARD 17)

hunter_add_package(thrift)
find_package(thrift CONFIG REQUIRED)

find_package(libmongoc-1.0 1.13 REQUIRED)

include("cmake/Findlibmemcached.cmake")
INCLUDE_DIRECTORIES(${LIBMEMCACHED_INCLUDE_DIR})

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB twitter_thrift_SRC
        "gen-cpp/*.cpp"
        )

add_library(twitter_thrift
        ${twitter_thrift_SRC}
        )


add_executable(UserService
        src/UserService.cc
        )
target_link_libraries(UserService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        twitter_thrift
        )
target_include_directories(UserService PUBLIC
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(UserService PUBLIC
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(UserService PUBLIC
        "${MONGOC_DEFINITIONS}"
        )

add_executable(FileService
        src/FileService.cc
        )
target_link_libraries(FileService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        twitter_thrift
        )
target_include_directories(FileService PUBLIC
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(FileService PUBLIC
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(FileService PUBLIC
        "${MONGOC_DEFINITIONS}"
        )

add_executable(TweetService
        src/TweetService.cc
        )

target_link_libraries(TweetService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        twitter_thrift
        )
target_include_directories(TweetService PUBLIC
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(TweetService PUBLIC
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(TweetService PUBLIC
        "${MONGOC_DEFINITIONS}"
        )

add_executable(ComposeService
        src/ComposeService.cc
        )
target_link_libraries(ComposeService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        twitter_thrift
        )
target_include_directories(ComposeService PUBLIC
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(ComposeService PUBLIC
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(ComposeService PUBLIC
        "${MONGOC_DEFINITIONS}"
        )