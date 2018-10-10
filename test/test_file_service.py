import sys
sys.path.append('../gen-py')

from twitter import FileService,ttypes

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

  addr = config["FileService"]["addr"]
  port = config["FileService"]["port"]
  # Make socket
  transport = TSocket.TSocket(addr, port)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TBufferedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = FileService.Client(protocol)

  # Connect!
  transport.open()

  print("conn open")
  file_id = "file_" + str(random.randint(1, 1000000))
  res = client.getFile_(file_id, [])
  print(res.file_id, res.content, res.timestamps)
  res = client.getFile_(file_id, [])
  print(res.file_id, res.content, res.timestamps)

  transport.close()

if __name__ == '__main__':
  try:
    main()
  except Thrift.TException as tx:
    print('%s' % tx.message)