/**
*  $Id: md5lib.c,v 1.10 2008/05/12 20:51:27 carregal Exp $
*  Cryptographic and Hash functions for Lua
*  @author  Roberto Ierusalimschy
*  Modified by ZenQ
*/


#include <lua.h>
#include <lauxlib.h>

#include "md5.h"


static void convert_hash_to_hex(char hash[16], char result[32]) {
  for (int i=0; i<16; i++){
      sprintf((result + 2*i), "%02x", (unsigned char)hash[i]);
  }
}


/**
*  Hash function. Returns a hash for a given string.
*  @param message: string.
*  @return  String with the md5 hash value converted to hexadecimal digits.
*/
static int calculate (lua_State *L) {
  char result[16];
  size_t l;
  const char *message = luaL_checklstring(L, 1, &l);
  md5_calculate(message, l, result);

  char hex_hash[32];
  convert_hash_to_hex(result, hex_hash);

  lua_pushlstring(L, hex_hash, 32L);
  return 1;
}


static int new_ctx (lua_State *L) {
  size_t nbytes = sizeof(md5_ctx);
  md5_ctx *ctx_ptr = (md5_ctx *)lua_newuserdata(L, nbytes);

  md5_ctx_init(ctx_ptr);

  return 1;
}


static int ctx_update (lua_State *L) {
  // md5_ctx *ctx_ptr = (md5_ctx*)luaL_checkudata(L, 1, "MD5_Ctx");
  md5_ctx *ctx_ptr = (md5_ctx*) lua_touserdata(L, 1);
  luaL_argcheck(L, ctx_ptr != NULL, 1, "'ctx_ptr' expected");

  size_t l;
  const char *message = luaL_checklstring(L, 2, &l);
  md5_ctx_update(ctx_ptr, message, l);

  return 1;
}


static int ctx_finish (lua_State *L) {
  // md5_ctx *ctx_ptr = (md5_ctx*)luaL_checkudata(L, 1, "MD5_Ctx");
  md5_ctx *ctx_ptr = (md5_ctx*) lua_touserdata(L, 1);
  luaL_argcheck(L, ctx_ptr != NULL, 1, "'ctx_ptr' expected");
  
  char result[16];
  md5_ctx_finish(ctx_ptr, result);

  char hex_hash[32];
  convert_hash_to_hex(result, hex_hash);

  lua_pushlstring(L, hex_hash, 32L);

  return 1;
}


static struct luaL_Reg md5lib[] = {
  {"calculate", calculate},
  {"new", new_ctx},
  {"update", ctx_update},
  {"finish", ctx_finish},
  {NULL, NULL}
};


int luaopen_md5 (lua_State *L) {
  luaL_register(L, "md5", md5lib);
  return 1;
}
