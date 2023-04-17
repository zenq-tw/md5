local md5 = require('md5')
assert(type(md5) == "table", ("Failed to load md5 module (troubles with Lua C API linkage?). Retval from require: '%s' (type: %s); should be <table>"):format(tostring(md5), type(md5)))


--================================================================================================================================================

-- test some known sums
assert("d41d8cd98f00b204e9800998ecf8427e" == md5.calculate(""))
assert("0cc175b9c0f1b6a831c399e269772661" == md5.calculate("a"))
assert("900150983cd24fb0d6963f7d28e17f72" == md5.calculate("abc"))
assert("f96b697d7cb7938d525a2f31aaf161d0" == md5.calculate("message digest"))
assert("c3fcd3d76192e4007dfb496cca67e13b" == md5.calculate("abcdefghijklmnopqrstuvwxyz"))
assert("d174ab98d277d9f5a5611c2c9f419d9f" == md5.calculate("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"))


------------------------------------------------

-- test padding borders
assert("e9e7e260dce84ffa6e0e7eb5fd9d37fc" == md5.calculate(string.rep('a',53)))
assert("eced9e0b81ef2bba605cbc5e2e76a1d0" == md5.calculate(string.rep('a',54)))
assert("ef1772b6dff9a122358552954ad0df65" == md5.calculate(string.rep('a',55)))
assert("3b0c8ac703f828b04c6c197006d17218" == md5.calculate(string.rep('a',56)))
assert("652b906d60af96844ebd21b674f35e93" == md5.calculate(string.rep('a',57)))
assert("b06521f39153d618550606be297466d5" == md5.calculate(string.rep('a',63)))
assert("014842d480b571495a4a0363793f7367" == md5.calculate(string.rep('a',64)))
assert("c743a45e0d2e6a95cb859adae0248435" == md5.calculate(string.rep('a',65)))
assert("46bc249a5a8fc5d622cf12c42c463ae0" == md5.calculate(string.rep('a',255)))
assert("81109eec5aa1a284fb5327b10e9c16b9" == md5.calculate(string.rep('a',256)))

assert("57edf4a22be3c955ac49da2e2107b67a" == md5.calculate("12345678901234567890123456789012345678901234567890123456789012345678901234567890"))


------------------------------------------------

print('test: md5.calculate()                           [+]')


--================================================================================================================================================


local ctx = md5.new()

assert(type(ctx) == 'userdata', 'md5.new() -> custom userdata expected')

assert(tostring(ctx):sub(1, 10) == 'MD5Context', 'invalid MD5Context.__tostring()')

------------------------------------------------

assert('d41d8cd98f00b204e9800998ecf8427e' == md5.new():calculate())

ctx = md5.new(); ctx:update("a");
assert("0cc175b9c0f1b6a831c399e269772661" == ctx:calculate())       -- "a"
ctx:update("b"); ctx:update("c");
assert("900150983cd24fb0d6963f7d28e17f72" == ctx:calculate())       -- "abc"


ctx = md5.new(); ctx:update(" "); ctx:update("22222222"); ctx:update("3")
assert("f31f956b2a35741dcc173169f3f0bd59" == ctx:calculate())       -- " 222222223"


ctx = md5.new(); ctx:update("message"); ctx:update(" "); ctx:update("digest")
assert("f96b697d7cb7938d525a2f31aaf161d0" == ctx:calculate())       -- "message digest"


------------------------------------------------


local ONE_KB_SPLITTED = {
  "Curabitur viverra faucibus magna. Sed bibendum metus vitae nisl fringilla, vitae interdum velit consequat. Nullam id orci euismod, commodo nibh a, condimentum velit. Nam bibendum, metus nec tempus tincidunt, augue lectus aliquam orci, sed ",
  "sollicitudin nibh urna eget massa. Proin eu luctus felis. Duis posuere ex at purus rutrum mollis. Duis vitae vulputate erat. Sed vel tempor nisi. In posuere elit ac viverra consectetur. ",
  "Suspendisse nec nunc ac felis bibendum mattis at sed sapien. Mauris libero neque, pellentesque non ipsum eu, iaculis euismod quam. Etiam elementum, sem sit amet convallis consectetur, nulla nisi elementum risus, porta mattis lectus sem imperdiet felis. ",
  "Praesent faucibus varius urna a euismod. Pellentesque semper, nisl nec euismod ullamcorper, lacus mi dictum purus, quis consequat ipsum magna a eros. Nullam eget convallis nibh. Vivamus commodo laoreet quam nec rutrum. Aenean orci massa,lacinia id nisi ut, ornare tempus tortor. Pellentesque commodo porta elit, sit amet fringilla nunc efficitur.",
}

ctx = md5.new()
for _, chunk in pairs(ONE_KB_SPLITTED) do
  ctx:update(chunk)
end

assert("c34afd95f646f2445e5b5ab90e151024" == ctx:calculate())


------------------------------------------------

print('test: iterative calculations (MD5Context)       [+]')
