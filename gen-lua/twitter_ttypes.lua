--
-- Autogenerated by Thrift
--
-- DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
-- @generated
--


require 'Thrift'
require 'twitter_constants'

Tweet_ = __TObject:new{
  tweet_id,
  user_id,
  file_id,
  text,
  header
}

function Tweet_:read(iprot)
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
    elseif fid == 2 then
      if ftype == TType.STRING then
        self.user_id = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 3 then
      if ftype == TType.STRING then
        self.file_id = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 4 then
      if ftype == TType.STRING then
        self.text = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 5 then
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

function Tweet_:write(oprot)
  oprot:writeStructBegin('Tweet_')
  if self.tweet_id ~= nil then
    oprot:writeFieldBegin('tweet_id', TType.STRING, 1)
    oprot:writeString(self.tweet_id)
    oprot:writeFieldEnd()
  end
  if self.user_id ~= nil then
    oprot:writeFieldBegin('user_id', TType.STRING, 2)
    oprot:writeString(self.user_id)
    oprot:writeFieldEnd()
  end
  if self.file_id ~= nil then
    oprot:writeFieldBegin('file_id', TType.STRING, 3)
    oprot:writeString(self.file_id)
    oprot:writeFieldEnd()
  end
  if self.text ~= nil then
    oprot:writeFieldBegin('text', TType.STRING, 4)
    oprot:writeString(self.text)
    oprot:writeFieldEnd()
  end
  if self.header ~= nil then
    oprot:writeFieldBegin('header', TType.STRING, 5)
    oprot:writeString(self.header)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end

User_ = __TObject:new{
  user_id,
  username,
  homepage,
  header
}

function User_:read(iprot)
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
        self.username = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 3 then
      if ftype == TType.STRING then
        self.homepage = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 4 then
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

function User_:write(oprot)
  oprot:writeStructBegin('User_')
  if self.user_id ~= nil then
    oprot:writeFieldBegin('user_id', TType.STRING, 1)
    oprot:writeString(self.user_id)
    oprot:writeFieldEnd()
  end
  if self.username ~= nil then
    oprot:writeFieldBegin('username', TType.STRING, 2)
    oprot:writeString(self.username)
    oprot:writeFieldEnd()
  end
  if self.homepage ~= nil then
    oprot:writeFieldBegin('homepage', TType.STRING, 3)
    oprot:writeString(self.homepage)
    oprot:writeFieldEnd()
  end
  if self.header ~= nil then
    oprot:writeFieldBegin('header', TType.STRING, 4)
    oprot:writeString(self.header)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end

File_ = __TObject:new{
  file_id,
  content,
  header
}

function File_:read(iprot)
  iprot:readStructBegin()
  while true do
    local fname, ftype, fid = iprot:readFieldBegin()
    if ftype == TType.STOP then
      break
    elseif fid == 1 then
      if ftype == TType.STRING then
        self.file_id = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 2 then
      if ftype == TType.STRING then
        self.content = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 3 then
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

function File_:write(oprot)
  oprot:writeStructBegin('File_')
  if self.file_id ~= nil then
    oprot:writeFieldBegin('file_id', TType.STRING, 1)
    oprot:writeString(self.file_id)
    oprot:writeFieldEnd()
  end
  if self.content ~= nil then
    oprot:writeFieldBegin('content', TType.STRING, 2)
    oprot:writeString(self.content)
    oprot:writeFieldEnd()
  end
  if self.header ~= nil then
    oprot:writeFieldBegin('header', TType.STRING, 3)
    oprot:writeString(self.header)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end

Tweet = __TObject:new{
  tweet_id,
  user,
  text,
  file,
  header
}

function Tweet:read(iprot)
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
    elseif fid == 2 then
      if ftype == TType.STRUCT then
        self.user = User_:new{}
        self.user:read(iprot)
      else
        iprot:skip(ftype)
      end
    elseif fid == 3 then
      if ftype == TType.STRING then
        self.text = iprot:readString()
      else
        iprot:skip(ftype)
      end
    elseif fid == 4 then
      if ftype == TType.STRUCT then
        self.file = File_:new{}
        self.file:read(iprot)
      else
        iprot:skip(ftype)
      end
    elseif fid == 5 then
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

function Tweet:write(oprot)
  oprot:writeStructBegin('Tweet')
  if self.tweet_id ~= nil then
    oprot:writeFieldBegin('tweet_id', TType.STRING, 1)
    oprot:writeString(self.tweet_id)
    oprot:writeFieldEnd()
  end
  if self.user ~= nil then
    oprot:writeFieldBegin('user', TType.STRUCT, 2)
    self.user:write(oprot)
    oprot:writeFieldEnd()
  end
  if self.text ~= nil then
    oprot:writeFieldBegin('text', TType.STRING, 3)
    oprot:writeString(self.text)
    oprot:writeFieldEnd()
  end
  if self.file ~= nil then
    oprot:writeFieldBegin('file', TType.STRUCT, 4)
    self.file:write(oprot)
    oprot:writeFieldEnd()
  end
  if self.header ~= nil then
    oprot:writeFieldBegin('header', TType.STRING, 5)
    oprot:writeString(self.header)
    oprot:writeFieldEnd()
  end
  oprot:writeFieldStop()
  oprot:writeStructEnd()
end