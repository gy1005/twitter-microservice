cmake_minimum_required(VERSION 3.9)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.28.tar.gz"
        SHA1 "6c79b36253a8e8bcba48927e9f3be65a3a81e6ec"
        LOCAL
)

project(twitter_microservice)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++-8)

hunter_add_package(thrift)
find_package(thrift CONFIG REQUIRED)

find_package(libmongoc-1.0 1.13 REQUIRED)

include("cmake/Findlibmemcached.cmake")
INCLUDE_DIRECTORIES(${LIBMEMCACHED_INCLUDE_DIR})

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(UserService
        src/UserService.cc
        gen-cpp/UserService.cpp
        gen-cpp/twitter_types.cpp
        )
target_link_libraries(UserService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        )


target_include_directories(UserService PUBLIC
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(UserService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(UserService PRIVATE
        "${MONGOC_DEFINITIONS}"
        )

add_executable(FileService
        src/FileService.cc
        gen-cpp/FileService.cpp
        gen-cpp/twitter_types.cpp
        )
target_link_libraries(FileService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        )
target_include_directories(FileService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(FileService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(FileService PRIVATE
        "${MONGOC_DEFINITIONS}"
        )

add_executable(TweetService
        src/TweetService.cc
        gen-cpp/TweetService.cpp
        gen-cpp/twitter_types.cpp
        )
target_link_libraries(TweetService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        )

target_include_directories(TweetService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(TweetService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        )
target_compile_definitions(TweetService PRIVATE
        "${MONGOC_DEFINITIONS}"
        )


add_executable(ComposeService
        src/ComposeService.cc
        gen-cpp/ComposeService.cpp
        gen-cpp/FileService.cpp
        gen-cpp/TweetService.cpp
        gen-cpp/UserService.cpp
        gen-cpp/twitter_types.cpp
        )
target_link_libraries(ComposeService PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        nlohmann_json::nlohmann_json
        )

add_executable(Client
        src/Client.cc
        gen-cpp/ComposeService.cpp
        gen-cpp/twitter_types.cpp
        )
target_link_libraries(Client PUBLIC
        thrift::thrift_static
        thrift::thriftnb_static
        nlohmann_json::nlohmann_json
        )
