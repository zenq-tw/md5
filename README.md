# MD5 - Cryptographic Library for Lua

Original repo: https://lunarmodules.github.io/md5/

MD5 offers basic cryptographic facilities for Lua 5.1: a hash (digest)
function, ~~a pair crypt/decrypt based on MD5 and CFB, and a pair crypt/decrypt based on DES with 56-bit keys.~~ (removed in this fork)

MD5 current version is 1.2.

## History

* 1.0.0 - initial release: Support only for Lua5.1 with digest functionality.

## License

MD5 is free software and uses the same license as Lua (MIT). 

The DES 56 C library was implemented by Stuart Levy and uses a MIT license too (check the source).


### Original Licence text

MD5 is free software: it can be used for both academic and commercial
purposes at absolutely no cost. There are no royalties or GNU-like "copyleft"
restrictions. MD5 qualifies as [Open Source](http://www.opensource.org/docs/definition.html)
software. Its licenses are compatible with [GPL](http://www.gnu.org/licenses/gpl.html). MD5 is not in the public domain and PUC-Rio keeps its copyright. The legal details are below.

The spirit of the license is that you are free to use MD5 for any purpose at no cost without having to ask us. The only requirement is that if you do use MD5,
then you should give us credit by including the appropriate copyright notice
somewhere in your product or its documentation.

MD5 was designed and implemented by Roberto Ierusalimschy and Marcela Oz&oacute;rio Suarez. The implementation is not derived from licensed software. The DES 56 C library was implemented by Stuart Levy and uses a MIT licence too.

__Copyright &copy; 2003-2013 PUC-Rio.  All rights reserved.__

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
