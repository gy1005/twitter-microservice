--
-- Autogenerated by Thrift
--
-- DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
-- @generated
--


require 'Thrift'
require 'twitter_ttypes'

TweetServiceClient = __TObject.new(__TClient, {
  __type = 'TweetServiceClient'
})

function TweetServiceClient:getTweet_(tweet_id)
  self:send_getTweet_(tweet_id)
  return self:recv_getTweet_(tweet_id)
end

function TweetServiceClient:send_getTweet_(tweet_id)
  self.oprot:writeMessageBegin('getTweet_', TMessageType.CALL, self._seqid)
  local args = getTweet__args:new{}
  args.tweet_id = tweet_id
  args:write(self.oprot)
  self.oprot:writeMessageEnd()
  self.oprot.trans:flush()
end

function TweetServiceClient:recv_getTweet_(tweet_id)
  local fname, mtype, rseqid = self.iprot:readMessageBegin()
  if mtype == TMessageType.EXCEPTION then
    local x = TApplicationException:new{}
    x:read(self.iprot)
    self.iprot:readMessageEnd()
    error(x)
  end
  local result = getTweet__result:new{}
  result:read(self.iprot)
  self.iprot:readMessageEnd()
  if result.success ~= nil then
    return result.success
  end
  error(TApplicationException:new{errorCode = TApplicationException.MISSING_RESULT})
end
TweetServiceIface = __TObject:new{
  __type = 'TweetServiceIface'
}


TweetServiceProcessor = __TObject.new(__TProcessor
, {
 __type = 'TweetServiceProcessor'
})

function TweetServiceProcessor:process(iprot, oprot, server_ctx)
  local name, mtype, seqid = iprot:readMessageBegin()
  local func_name = 'process_' .. name
  if not self[func_name] or ttype(self[func_name]) ~= 'function' then
    iprot:skip(TType.STRUCT)
    iprot:readMessageEnd()
    x = TApplicationException:new{
      errorCode = TApplicationException.UNKNOWN_METHOD
    }
    oprot:writeMessageBegin(name, TMessageType.EXCEPTION, seqid)
    x:write(oprot)
    oprot:writeMessageEnd()
    oprot.trans:flush()
  else
    self[func_name](self, seqid, iprot, oprot, server_ctx)
  end
end

function TweetServiceProcessor:process_getTweet_(seqid, iprot, oprot, server_ctx)
  local args = getTweet__args:new{}
  local reply_type = TMessageType.REPLY
  args:read(iprot)
  iprot:readMessageEnd()
  local result = getTweet__result:new{}
  local status, res = pcall(self.handler.getTweet_, self.handler, args.tweet_id)
  if not status then
    reply_type = TMessageType.EXCEPTION
    result = TApplicationException:new{message = res}
  else
    result.success = res
  end
  oprot:writeMessageBegin('getTweet_', reply_type, seqid)
  result:write(oprot)
  oprot:writeMessageEnd()
  oprot.trans:flush()
end

-- HELPER FUNCTIONS AND STRUCTURES

getTweet__args = __TObject:new{
  tweet_id
}

function getTweet__args:read(iprot)
  iprot:readStructBegin()
  while true do
    local fname, ftype, fid = iprot:readFieldBegin()
    if ftype == TType.STOP then
      break
    elseif fid == 1 then
      if ftype == TType.STRING then
        self.tweet_id = iprot:readString()
      else
        iprot:skip(ftype)
      end
    else
      iprot:skip(ftype)
    end
    iprot:readFieldEnd()
  end
  iprot:readStructEnd()
end

function getTweet__args:write(oprot)
  oprot:writeStructBegin('getTweet__args')
  if self.tweet_id ~= nil then
    oprot:writeFieldBegin('tweet_id', TType.STRING, 1)
    oprot:writeString(self.tweet_id)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end

getTweet__result = __TObject:new{
  success
}

function getTweet__result:read(iprot)
  iprot:readStructBegin()
  while true do
    local fname, ftype, fid = iprot:readFieldBegin()
    if ftype == TType.STOP then
      break
    elseif fid == 0 then
      if ftype == TType.STRUCT then
        self.success = Tweet_:new{}
        self.success:read(iprot)
      else
        iprot:skip(ftype)
      end
    else
      iprot:skip(ftype)
    end
    iprot:readFieldEnd()
  end
  iprot:readStructEnd()
end

function getTweet__result:write(oprot)
  oprot:writeStructBegin('getTweet__result')
  if self.success ~= nil then
    oprot:writeFieldBegin('success', TType.STRUCT, 0)
    self.success:write(oprot)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end