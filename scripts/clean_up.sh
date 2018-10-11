#!/usr/bin/env bash

docker stop user_mongodb
docker rm user_mongodb
docker stop tweet_mongodb
docker rm tweet_mongodb
docker stop file_mongodb
docker rm file_mongodb

docker stop user_memcached
docker rm user_memcached
docker stop tweet_memcached
docker rm tweet_memcached
docker stop file_memcached
docker rm file_memcached

killall UserService
killall FileService
killall TweetService
killall ComposeService