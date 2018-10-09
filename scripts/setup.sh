#!/usr/bin/env bash

mkdir -p ../mongodb
mkdir -p ../mongodb/user
mkdir -p ../mongodb/tweet
mkdir -p ../mongodb/file

USER_MONGODB_PORT=$(cat ../config/service_config.json | jq ".UserMongoDB.port")
TWEET_MONGODB_PORT=$(cat ../config/service_config.json | jq ".TweetMongoDB.port")
FILE_MONGODB_PORT=$(cat ../config/service_config.json | jq ".FileMongoDB.port")
USER_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".UserMemcached.port")
TWEET_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".TweetMemcached.port")
FILE_MEMCACHED_PORT=$(cat ../config/service_config.json | jq ".FileMemcached.port")

docker run -d -p $USER_MONGODB_PORT:27017 -v $PWD/../mongodb/user:/data/db --name user_mongodb mongo
docker run -d -p $TWEET_MONGODB_PORT:27017 -v $PWD/../mongodb/tweet:/data/db --name tweet_mongodb mongo
docker run -d -p $FILE_MONGODB_PORT:27017 -v $PWD/../mongodb/file:/data/db --name file_mongodb mongo
docker run -d -p $TWEET_MEMCACHED_PORT:11211 --name tweet_memcached memcached
docker run -d -p $USER_MEMCACHED_PORT:11211 --name user_memcached memcached
docker run -d -p $FILE_MEMCACHED_PORT:11211 --name file_memcached memcached