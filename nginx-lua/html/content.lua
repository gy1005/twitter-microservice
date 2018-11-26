require 'Thrift'
require 'twitter_ttypes'



-- time = socket.gettime()*1000
-- math.randomseed(time)
-- math.random(); math.random(); math.random()

local function getTweet()
  ngx.say("hello")
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


  local id = math.random(1000)
  local user_id = 'user_' .. tostring(id)
  local tweet_id = 'tweet_' .. tostring(id)

  -- assert(status, 'Failed to connect to server')
  sock:open()
  local ret_tweet = client:getTweet(user_id, tweet_id)
  sock:setKeepAlive(60000, 100)
  return ret_tweet.text
end

ngx.say(getTweet())







