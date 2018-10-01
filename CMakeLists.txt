cmake_minimum_required(VERSION 3.12)

# Using hunter to resolve dependencies.
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

hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)

find_package(libmongoc-1.0 1.9 REQUIRED)

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
target_include_directories(UserService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(UserService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        )
target_compile_definitions(UserService PRIVATE
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
target_include_directories(FileService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(FileService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        )
target_compile_definitions(FileService PRIVATE
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
target_include_directories(TweetService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(TweetService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        )
target_compile_definitions(TweetService PRIVATE
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
target_include_directories(ComposeService PRIVATE
        "${MONGOC_INCLUDE_DIRS}"
        )
target_link_libraries(ComposeService PRIVATE
        "${MONGOC_LIBRARIES}"
        "${LIBMEMCACHED_LIBRARY}"
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        )
target_compile_definitions(ComposeService PRIVATE
        "${MONGOC_DEFINITIONS}"
        )