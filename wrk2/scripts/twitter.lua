require "socket"
time = socket.gettime()*1000
math.randomseed(time)
math.random(); math.random(); math.random()

request = function()  
  return wrk.format(nil, "http://localhost:8080/")    
end


response = function(status, headers, body)
  -- _return = {}
  -- table.insert(_return, headers["nginx-start"])
  -- table.insert(_return, headers["nginx"])
  -- table.insert(_return, headers["compose-queue"])
  -- table.insert(_return, headers["compose-proc"])
  -- table.insert(_return, headers["tweet-queue"])
  -- table.insert(_return, headers["tweet-proc"])
  -- table.insert(_return, headers["user-queue"])
  -- table.insert(_return, headers["user-proc"])
  -- table.insert(_return, headers["file-queue"])
  -- table.insert(_return, headers["file-proc"])
  -- table.insert(_return, headers["tweet-mmc-get"])
  -- if headers["tweet-mmc-set"] ~= nil then    
  --   table.insert(_return, headers["tweet-mmc-set"])
  --   table.insert(_return, headers["tweet-mongo-find"])
  -- else
  --   table.insert(_return, 0)
  --   table.insert(_return, 0)
  -- end
  -- if headers["user-mmc-set"] ~= nil then    
  --   table.insert(_return, headers["user-mmc-set"])
  --   table.insert(_return, headers["user-mongo-find"])
  -- else
  --   table.insert(_return, 0)
  --   table.insert(_return, 0)
  -- end
  -- if headers["file-mmc-set"] ~= nil then    
  --   table.insert(_return, headers["file-mmc-set"])
  --   table.insert(_return, headers["file-mongo-find"])
  -- else
  --   table.insert(_return, 0)
  --   table.insert(_return, 0)
  -- end
  return headers["nginx-start"], headers["nginx"], headers["compose-queue"], headers["compose-proc"], headers["tweet-queue"], headers["tweet-proc"], headers["user-queue"], headers["user-proc"], headers["file-queue"], headers["file-proc"], headers["tweet-mmc-get"], headers["tweet-mmc-set"], headers["tweet-mongo-find"], headers["user-mmc-set"], headers["user-mongo-find"], headers["file-mmc-set"], headers["file-mongo-find"]
  -- return _return
end