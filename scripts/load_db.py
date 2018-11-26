#!/usr/bin/env python3

import pymongo as pm
from optparse import OptionParser
from multiprocessing import Process
import random
import string
import json
import numpy as np

def gen_dict(options, index):
  if options.type == "user":
    user_id = "user_" + str(index)
    return {
      "user_id": user_id,
      "username": user_id,
      "homepage": "https://twitter.com/" + user_id
    }
  elif options.type == "tweet":
    tweet_id = "tweet_" + str(index)
    user_id = "user_" + str(index)
    file_id = "file_" + str(index)
    text = ""
    text_size = random.randint(1, options.text_size)
    for i in range(text_size):
      text += random.choice(string.ascii_letters)
    return {
      "tweet_id": tweet_id,
      "user_id": user_id,
      "file_id": file_id,
      "text": text
    }
  elif options.type == "file":
    file_id = "file_" + str(index)
    content = ""
    file_size = random.randint(1, options.file_size)
    for i in range(file_size):
      content += random.choice(string.ascii_letters)
    return {
      "file_id": file_id,
      "content": content
    }
  else:
    print("Type is not identified.")
    return {}


def worker(options, worker_id, service_config):

  if options.type == "user":
    addr = service_config["UserMongoDB"]["addr"]
    port = service_config["UserMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.user
    collection = db.user
  elif options.type == "tweet":
    addr = service_config["TweetMongoDB"]["addr"]
    port = service_config["TweetMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.tweet
    collection = db.tweet
  elif options.type == "file":
    addr = service_config["FileMongoDB"]["addr"]
    port = service_config["FileMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.file
    collection = db.file
  else:
    print("Type is not identified.")
    return

  for i in range(int(np.ceil(options.n_records / options.n_threads))):
    collection.insert_one(gen_dict(options, int(i + worker_id
                                   * options.n_records / options.n_threads)))
    if i % (options.n_records / options.n_threads / 50) == 0:
      print("Worker", worker_id, "inserted", i, "records successfully")

  print("Worker", worker_id, "inserted", int(options.n_records /
        options.n_threads), "records successfully")

def main():
  parser=OptionParser()
  parser.add_option("-r", "--n_records", type="int", dest="n_records",
                    default=1000)
  parser.add_option("-t", "--n_threads", type="int", dest="n_threads",
                    default=8)
  parser.add_option("-T", "--type", type="string", dest="type")
  parser.add_option("-s", "--text_size", type="int", dest="text_size",
                    default=280)
  parser.add_option("-S", "--file_size", type="int", dest="file_size",
                    default=4096)
  
  (options, args) = parser.parse_args()

  with open("../config/service_config.json") as json_file:
    service_config = json.load(json_file)
  p_list = []
  for i in range(options.n_threads):
    p = Process(target=worker, args=(options, i, service_config))
    p_list.append(p)
    p.start()
  for i in range(options.n_threads):
    p_list[i].join()


  if options.type == "user":
    addr = service_config["UserMongoDB"]["addr"]
    port = service_config["UserMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.user
    collection = db.user
    collection.create_index([("user_id", pm.ASCENDING)])
  elif options.type == "tweet":
    addr = service_config["TweetMongoDB"]["addr"]
    port = service_config["TweetMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.tweet
    collection = db.tweet
    collection.create_index([("tweet_id", pm.ASCENDING)])
  elif options.type == "file":
    addr = service_config["FileMongoDB"]["addr"]
    port = service_config["FileMongoDB"]["port"]
    mongo = pm.MongoClient(addr, port)
    db = mongo.file
    collection = db.file
    collection.create_index([("file_id", pm.ASCENDING)])

if __name__ == "__main__":
  main()

