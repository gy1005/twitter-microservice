--
-- Autogenerated by Thrift
--
-- DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
-- @generated
--


require 'Thrift'
require 'twitter_ttypes'

UserServiceClient = __TObject.new(__TClient, {
  __type = 'UserServiceClient'
})

function UserServiceClient:getUser_(user_id, header)
  self:send_getUser_(user_id, header)
  return self:recv_getUser_(user_id, header)
end

function UserServiceClient:send_getUser_(user_id, header)
  self.oprot:writeMessageBegin('getUser_', TMessageType.CALL, self._seqid)
  local args = getUser__args:new{}
  args.user_id = user_id
  args.header = header
  args:write(self.oprot)
  self.oprot:writeMessageEnd()
  self.oprot.trans:flush()
end

function UserServiceClient:recv_getUser_(user_id, header)
  local fname, mtype, rseqid = self.iprot:readMessageBegin()
  if mtype == TMessageType.EXCEPTION then
    local x = TApplicationException:new{}
    x:read(self.iprot)
    self.iprot:readMessageEnd()
    error(x)
  end
  local result = getUser__result:new{}
  result:read(self.iprot)
  self.iprot:readMessageEnd()
  if result.success ~= nil then
    return result.success
  end
  error(TApplicationException:new{errorCode = TApplicationException.MISSING_RESULT})
end
UserServiceIface = __TObject:new{
  __type = 'UserServiceIface'
}


UserServiceProcessor = __TObject.new(__TProcessor
, {
 __type = 'UserServiceProcessor'
})

function UserServiceProcessor:process(iprot, oprot, server_ctx)
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

function UserServiceProcessor:process_getUser_(seqid, iprot, oprot, server_ctx)
  local args = getUser__args:new{}
  local reply_type = TMessageType.REPLY
  args:read(iprot)
  iprot:readMessageEnd()
  local result = getUser__result:new{}
  local status, res = pcall(self.handler.getUser_, self.handler, args.user_id, args.header)
  if not status then
    reply_type = TMessageType.EXCEPTION
    result = TApplicationException:new{message = res}
  else
    result.success = res
  end
  oprot:writeMessageBegin('getUser_', reply_type, seqid)
  result:write(oprot)
  oprot:writeMessageEnd()
  oprot.trans:flush()
end

-- HELPER FUNCTIONS AND STRUCTURES

getUser__args = __TObject:new{
  user_id,
  header
}

function getUser__args:read(iprot)
  iprot:readStructBegin()
  while true do
    local fname, ftype, fid = iprot:readFieldBegin()
    if ftype == TType.STOP then
      break
    elseif fid == 1 then
      if ftype == TType.STRING then
        self.user_id = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 2 then
      if ftype == TType.STRING then
        self.header = iprot:readString()
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

function getUser__args:write(oprot)
  oprot:writeStructBegin('getUser__args')
  if self.user_id ~= nil then
    oprot:writeFieldBegin('user_id', TType.STRING, 1)
    oprot:writeString(self.user_id)
    oprot:writeFieldEnd()
  end
  if self.header ~= nil then
    oprot:writeFieldBegin('header', TType.STRING, 2)
    oprot:writeString(self.header)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end

getUser__result = __TObject:new{
  success
}

function getUser__result:read(iprot)
  iprot:readStructBegin()
  while true do
    local fname, ftype, fid = iprot:readFieldBegin()
    if ftype == TType.STOP then
      break
    elseif fid == 0 then
      if ftype == TType.STRUCT then
        self.success = User_:new{}
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

function getUser__result:write(oprot)
  oprot:writeStructBegin('getUser__result')
  if self.success ~= nil then
    oprot:writeFieldBegin('success', TType.STRUCT, 0)
    self.success:write(oprot)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end