#!/usr/bin/env bash

mkdir -p ../mongodb
mkdir -p ../mongodb/user
mkdir -p ../mongodb/tweet
mkdir -p ../mongodb/file

# USER_SERVICE_PORT=$(cat ../config/service_config.json | jq ".UserService.port")
# TWEET_SERVICE_PORT=$(cat ../config/service_config.json | jq ".TweetService.port")
# FILE_SERVICE_PORT=$(cat ../config/service_config.json | jq ".FileService.port")
# COMPOSE_SERVICE_PORT=$(cat ../config/service_config.json | jq ".ComposeService.port")

USER_MONGODB_PORT=$(cat ../config/service_config.json | jq ".UserMongoDB.port")
TWEET_MONGODB_PORT=$(cat ../config/service_config.json | jq ".TweetMongoDB.port")
FILE_MONGODB_PORT=$(cat ../config/service_config.json | jq ".FileMongoDB.port")
USER_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".UserMemcached.port")
TWEET_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".TweetMemcached.port")
FILE_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".FileMemcached.port")

docker run --cpuset-cpus=19 -d -p $USER_MONGODB_PORT:27017 -v $PWD/../mongodb/user:/data/db --name user_mongodb mongo
docker run --cpuset-cpus=18 -d -p $TWEET_MONGODB_PORT:27017 -v $PWD/../mongodb/tweet:/data/db --name tweet_mongodb mongo
docker run --cpuset-cpus=17 -d -p $FILE_MONGODB_PORT:27017 -v $PWD/../mongodb/file:/data/db --name file_mongodb mongo
docker run --cpuset-cpus=16 -d -p $TWEET_MEMCACHED_PORT:11211 --name tweet_memcached memcached
docker run --cpuset-cpus=15 -d -p $USER_MEMCACHED_PORT:11211 --name user_memcached memcached
docker run --cpuset-cpus=14 -d -p $FILE_MEMCACHED_PORT:11211 --name file_memcached memcached

taskset -c 13 ../build/UserService &
taskset -c 12 ../build/FileService &
taskset -c 11 ../build/TweetService &
taskset -c 10 ../build/ComposeService &