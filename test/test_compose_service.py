import sys
sys.path.append('../gen-py')

from twitter import ComposeService

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

  addr = config["ComposeService"]["addr"]
  port = config["ComposeService"]["port"]
  # Make socket
  transport = TSocket.TSocket(addr, port)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TBufferedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = ComposeService.Client(protocol)

  # Connect!
  transport.open()

  print("conn open")
  id = random.randint(1, 1000000)
  user_id = "user_" + str(id)
  tweet_id = "tweet_" + str(id)
  res = client.getTweet(user_id, tweet_id)
  print(res.tweet_id, res.text,
        res.user.user_id, res.user.username, res.user.homepage,
        res.file.file_id, res.file.content)


  transport.close()

if __name__ == '__main__':
  try:
    main()
  except Thrift.TException as tx:
    print('%s' % tx.message)