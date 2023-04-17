assert(type(arg[1]) == 'string', 'missing dumped library file')
assert(type(arg[2]) == 'string', 'missing restored library file')

local md5_dll_content = assert(assert(loadfile(arg[1]))())

local dll_file = assert(io.open(arg[2], 'wb'))
dll_file:write(md5_dll_content)
dll_file:close()
