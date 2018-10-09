import sys
sys.path.append('../gen-py')

from twitter import UserService,ttypes

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

  addr = config["UserService"]["addr"]
  port = config["UserService"]["port"]
  # Make socket
  transport = TSocket.TSocket(addr, port)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TBufferedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = UserService.Client(protocol)

  # Connect!
  transport.open()

  print("conn open")
  user_id = "user_" + str(random.randint(1, 1000000))
  res = client.getUser_(user_id)
  print(res.user_id, res.username, res.homepage)
  res = client.getUser_(user_id)
  print(res.user_id, res.username, res.homepage)

  transport.close()

if __name__ == '__main__':
  try:
    main()
  except Thrift.TException as tx:
    print('%s' % tx.message)