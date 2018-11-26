#!/usr/bin/env bash

DB_PATH=/filer-01/yg397/mongodb

mkdir -p $DB_PATH
mkdir -p $DB_PATH/user
mkdir -p $DB_PATH/tweet
mkdir -p $DB_PATH/file

USER_MONGODB_PORT=$(cat ../config/service_config.json | jq ".UserMongoDB.port")
TWEET_MONGODB_PORT=$(cat ../config/service_config.json | jq ".TweetMongoDB.port")
FILE_MONGODB_PORT=$(cat ../config/service_config.json | jq ".FileMongoDB.port")
USER_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".UserMemcached.port")
TWEET_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".TweetMemcached.port")
FILE_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".FileMemcached.port")
NGINX_PORT=$(cat ../config/service_config.json | jq ".Nginx.port")

# docker run --cpuset-cpus=19 -d -p $USER_MONGODB_PORT:27017 -v $DB_PATH/user:/data/db --name user_mongodb mongo
# docker run --cpuset-cpus=7 -d -p $TWEET_MONGODB_PORT:27017 -v $DB_PATH/tweet:/data/db --name tweet_mongodb mongo
# docker run --cpuset-cpus=5 -d -p $FILE_MONGODB_PORT:27017 -v $DB_PATH/file:/data/db --name file_mongodb mongo
# docker run --cpuset-cpus=3 -d -p $TWEET_MEMCACHED_PORT:11211 --name tweet_memcached memcached
# docker run --cpuset-cpus=1 -d -p $USER_MEMCACHED_PORT:11211 --name user_memcached memcached
# docker run --cpuset-cpus=9 -d -p $FILE_MEMCACHED_PORT:11211 --name file_memcached memcached

LUA_SCRIPT_PATH=$PWD/../nginx-lua/html/content.lua
NGINX_CONFIG_PATH=$PWD/../nginx-lua/conf/nginx.conf
GEN_LUA_PATH=$PWD/../gen-lua

# docker run -p $NGINX_PORT:8080 \
docker run --net=host \
-v $LUA_SCRIPT_PATH:/usr/local/openresty/nginx/html/content.lua \
-v $NGINX_CONFIG_PATH:/usr/local/openresty/nginx/conf/nginx.conf \
-v $GEN_LUA_PATH:/gen-lua \
--name twitter-nginx yg397/openresty-thrift:xenial

# taskset -c 17 ../build/UserService &
# taskset -c 15 ../build/FileService &
# taskset -c 13 ../build/TweetService &
# taskset -c 11 ../build/ComposeService &