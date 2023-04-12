# MD5 - Digest Library for Lua 5.1

> Original repo: https://lunarmodules.github.io/md5/

Library offers basic digest facilities for Lua 5.1.

Version of MD5 is 1.3.

Original version modified by me to add MD5 checksum calculation for use in TW: WH3 mods. Improved usability and removed all irrelevant features.

## Usage

```lua
md5 = require('md5')    -- md5.dll must be under your LUA_CPATH

checksum = md5.calculate('some string')
print(checksum)         -- -> 5ac749fbeec93607fc28d666be85e73a

-- 'MD5Context' allows you to perform iterative checksum calculation.
-- Useful for processing files or any streaming content.
-- Call 'update()' for each new content chunk and 'calculate()' whenewher you want
md5_context = md5.new()
md5_context:update('ABC')
md5_context:update('DEFG')
md5_context:update('H')

checksum = md5_context:calculate()  -- same as md5.calculate('ABCDEFGH')
print(checksum)       -- -> 4783e784b4fa2fba9e4d6502dbc64f8f

md5_context:update('12')
md5_context:update('34')

checksum = md5_context:calculate()    -- same as md5.calculate('ABCDEFGH1234')
print(checksum)       -- -> c9552f2ccf723bbb3957a423fc032494
```

## Build
All Makefiles are configured for MinGW (and obviously only for Windows).

When you call `build` target, compiler will build lua sources from `src/lua5.1_tw` and the library files. After that you will find `md5.dll` under path `build\md5.dll`.

```powershell
mingw32-make.exe build
```

You can test the built library by calling `test`.


```powershell
mingw32-make.exe test
```



> **Note:** To create the `md5.dll` file, we need `lua51.dll` and the source header files. I don't know how to get them from the game (haha), so I tried to build new ones from the default Lua sources. Therefore, the library comes with modified Lua5.1.5 sources that will need to be built before building the library itself. The only change in the source codes is that the lua type `number` is defined as `float` instead of `double` to be closer to Lua shipped with game.

## History

### Version 2.0.0 [11/Apr/2023] 

Initial release of forked version. 

Key differences from original (v1.2):
1. Support only Lua 5.1 and Windows
2. Removed all non-digest related features
3. Fixed iterative calculation of MD5 checksums (should work now)
4. Provided `MD5Context` class in Lua to encapsulate iterative checksum calculations

### Version 1.2 [06/Sep/2013]

* Code adapted to compile for Lua 5.0, 5.1 and 5.2

### Version 1.1.2 [12/May/2008]

* Fixed bug in 64-bit systems
* Fixed the Windows makefile to accept longer directory names
  (patch by Alessandro Hecht and Ignacio Burgueño).

## License

MD5 is free software and uses the same license as Lua (MIT). 


> ### Original Licence text
> 
> MD5 is free software: it can be used for both academic and commercial
> purposes at absolutely no cost. There are no royalties or GNU-like "copyleft"
> restrictions. MD5 qualifies as [Open Source](http://www.opensource.org/docs/definition.html)
> software. Its licenses are compatible with [GPL](http://www.gnu.org/licenses/gpl.html). MD5 is not in the public domain and PUC-Rio keeps its copyright. The legal details are below.
> 
> The spirit of the license is that you are free to use MD5 for any purpose at no cost without having to ask us. The only requirement is that if you do use MD5,
> then you should give us credit by including the appropriate copyright notice
> somewhere in your product or its documentation.
> 
> MD5 was designed and implemented by Roberto Ierusalimschy and Marcela Oz&oacute;rio Suarez. The implementation is not derived from licensed software. The DES 56 C library was implemented by Stuart Levy and uses a MIT licence too.
> 
> __Copyright &copy; 2003-2013 PUC-Rio.  All rights reserved.__
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in
> all copies or substantial portions of the Software.
> 
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
> THE SOFTWARE.
