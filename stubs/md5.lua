---@diagnostic disable: missing-return, unused-local

---@class MD5Lib
local md5 = {}

---Calculate MD5 checksum for provided `message`
---@param message string MD5 digest (hex)
---@return string checksum MD5 digest (hex)
function md5.calculate(message) end


---Create new context for iterative calculations
---@return MD5Context
function md5.new() end


---@class MD5Context: userdata
local MD5Context = {}

---Update stored digest with new message chunk
---@param chunk string
function MD5Context:update(chunk) end


---Calculate current MD5 checksum
---@return string checksum MD5 digest (hex)
function MD5Context:calculate() end


