require 'Thrift'
require 'math'

local socket = require 'socket'
start_time = socket.gettime() * 1000000
math.randomseed(start_time)
math.random(); math.random(); math.random()

local function getTweet()
  local TSocket = require('TSocket')
  local sock = TSocket:new{    
    host = '127.0.0.1',
    port = 9000
  }
  
  assert(sock, 'Failed to create client socket')
  sock:setTimeout(1000)

  local TBufferedTransport = require('TBufferedTransport')
  local transport = TBufferedTransport:new{
    trans = sock,
    isServer = false
  }  

  local TBinaryProtocol = require('TBinaryProtocol')
  local protocol = TBinaryProtocol:new{
    trans = transport
  }
  assert(protocol, 'Failed to create binary protocol')

  local ComposeServiceClient = require('twitter_ComposeService')
  local client = ComposeServiceClient:new{
    protocol = protocol
  }
  assert(client, 'Failed to create client')


  local id = math.random(1, 999)
  local user_id = 'user_' .. tostring(id)
  local tweet_id = 'tweet_' .. tostring(id)

  -- assert(status, 'Failed to connect to server')
  sock:open()
  compose_send_time = socket.gettime() * 1000000
  local ret_tweet = client:getTweet(user_id, tweet_id, '{}')
  sock:setKeepAlive(60000, 100)

  -- Latency tracing
  local json = require 'json'
  local header_json = json.decode(ret_tweet.header)
  ngx.header["nginx_start"] = start_time
  ngx.header["nginx"] = compose_send_time - start_time
  ngx.header["compose_queue"] = header_json['Compose']['compose_start'] - compose_send_time
  ngx.header["compose_proc"] = header_json['Compose']['compose_end'] - header_json['Compose']['compose_start']

  ngx.header["tweet_queue"] = header_json['Tweet']['getTweet_start'] - header_json['Compose']['getTweet_start']
  ngx.header["tweet_proc"] = header_json['Tweet']['getTweet_end'] - header_json['Tweet']['getTweet_start']
  ngx.header["tweet_mmc_get"] = header_json['Tweet']['get_end'] - header_json['Tweet']['get_start']  
  if header_json['Tweet']['find_start'] ~= nil then
    ngx.header["tweet_mmc_set"] = header_json['Tweet']['set_end'] - header_json['Tweet']['set_start']
    ngx.header["tweet_mongo_find"] = header_json['Tweet']['find_end'] - header_json['Tweet']['find_start']
  end

  ngx.header["file_queue"] = header_json['File']['getFile_start'] - header_json['Compose']['getFile_start']
  ngx.header["file_proc"] = header_json['File']['getFile_end'] - header_json['File']['getFile_start']
  ngx.header["file_mmc_get"] = header_json['File']['get_end'] - header_json['File']['get_start']  
  if header_json['File']['find_start'] ~= nil then
    ngx.header["file_mmc_set"] = header_json['File']['set_end'] - header_json['File']['set_start']
    ngx.header["file_mongo_find"] = header_json['File']['find_end'] - header_json['File']['find_start']
  end

  ngx.header["user_queue"] = header_json['User']['getUser_start'] - header_json['Compose']['getUser_start']
  ngx.header["user_proc"] = header_json['User']['getUser_end'] - header_json['User']['getUser_start']
  ngx.header["user_mmc_get"] = header_json['User']['get_end'] - header_json['User']['get_start']  
  if header_json['User']['find_start'] ~= nil then
    ngx.header["user_mmc_set"] = header_json['User']['set_end'] - header_json['User']['set_start']
    ngx.header["user_mongo_find"] = header_json['User']['find_end'] - header_json['User']['find_start']
  end


  return ret_tweet.text
end

ngx.say(getTweet())







