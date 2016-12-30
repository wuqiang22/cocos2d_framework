require("zlib")
local function  zlibCompress(input)
  local stream = zlib.deflate()
  local result, eof, bytes_in, bytes_out = stream(input,"finish")
  print("zlibCompress",eof,bytes_in,bytes_out)
  return result
end

local function zlibUnCompress(input)
  local stream = zlib.inflate()
  local result, eof, bytes_in, bytes_out = stream(input,"finish")
  print("zlibUnCompress",eof,bytes_in,bytes_out)
  return result
end

local function testLuaZip()
    local temp1 = zlibCompress("heheajsdfjasdfawsjjjjjjjjjjjjjjjjjjjaaafaiweasfdawwsef")
    local result = zlibUnCompress(temp1)
    print(result)
end

如果被压缩的字符串比较少，bytes_in > bytes_out 

