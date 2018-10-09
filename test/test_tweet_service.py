import sys
sys.path.append('../gen-py')

from twitter import TweetService,ttypes

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
import json
import random

def main():
  config = {}
  with open("../config/service_config.json") as json_file:
    config = json.load(json_file)

  addr = config["TweetService"]["addr"]
  port = config["TweetService"]["port"]
  # Make socket
  transport = TSocket.TSocket(addr, port)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TBufferedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = TweetService.Client(protocol)

  # Connect!
  transport.open()

  print("conn open")
  tweet_id = "tweet_" + str(random.randint(1, 1000000))
  res = client.getTweet_(tweet_id)
  print(res.tweet_id, res.user_id, res.file_id, res.text)
  res = client.getTweet_(tweet_id)
  print(res.tweet_id, res.user_id, res.file_id, res.text)

  transport.close()

if __name__ == '__main__':
  try:
    main()
  except Thrift.TException as tx:
    print('%s' % tx.message)