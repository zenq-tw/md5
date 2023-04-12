/**
 *  $Id: md5lib.c,v 1.10 2008/05/12 20:51:27 carregal Exp $
 *  Cryptographic and Hash functions for Lua
 *  @author  Roberto Ierusalimschy
 *  Modified by ZenQ
 */


#include <lua.h>
#include <lauxlib.h>

#include "md5.h"


#define LIB_NAME "md5"
#define TYPE_NAME "MD5Context"



#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM == 501

/*
** Adapted from Lua 5.2.0
*/
static void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup){
    luaL_checkstack(L, nup + 1, "too many upvalues");
    for (; l->name != NULL; l++)
    { /* fill the table with given functions */
        int i;
        lua_pushstring(L, l->name);
        for (i = 0; i < nup; i++) /* copy upvalues to the top */
            lua_pushvalue(L, -(nup + 1));
        lua_pushcclosure(L, l->func, nup); /* closure with those upvalues */
        lua_settable(L, -(nup + 3));
    }
    lua_pop(L, nup); /* remove upvalues */
}
#endif




static inline void convert_hash_to_hex(char hash[16], char result[32]) {
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


static int ctx_new (lua_State *L) {
    lua_settop(L, 0);

    md5_ctx *ctx_ptr = (md5_ctx*) lua_newuserdata(L, sizeof(md5_ctx));
    md5_ctx_init(ctx_ptr);

    luaL_getmetatable(L, TYPE_NAME);
    lua_setmetatable(L, 1);

    return 1;
}


static int ctx_update (lua_State *L) {
    md5_ctx *ctx_ptr = (md5_ctx*) luaL_checkudata(L, 1, TYPE_NAME);

    size_t l;
    const char *message = luaL_checklstring(L, 2, &l);
    md5_ctx_update(ctx_ptr, message, l);

    return 1;
}


static int ctx_calculate (lua_State *L) {
    md5_ctx *ctx_ptr = (md5_ctx*)luaL_checkudata(L, 1, TYPE_NAME);

    char result[16];
    md5_ctx_finish(ctx_ptr, result);

    char hex_hash[32];
    convert_hash_to_hex(result, hex_hash);

    lua_pushlstring(L, hex_hash, 32L);

    return 1;
}


static int ctx__tostring (lua_State *L) {
    md5_ctx *ctx_ptr = (md5_ctx*) luaL_checkudata(L, 1, TYPE_NAME);

    char buff[32];
    sprintf(buff, "%p", ctx_ptr);

    lua_pushfstring(L, "%s (%s)", TYPE_NAME, buff);
    return 1;
}


int luaopen_md5 (lua_State *L) {
    lua_settop(L, 0);  // The user may pass in values here, but we'll ignore those values.

    if (luaL_newmetatable(L, TYPE_NAME)) {    // If this metatable already exists, the library is already loaded.
        static struct luaL_Reg class_methods[] = {
            {"update", ctx_update},
            {"calculate", ctx_calculate},
            {"__tostring", ctx__tostring},
            {NULL, NULL}
        };
        luaL_setfuncs(L, class_methods, 0);  

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);

		lua_pushliteral(L, "__metatable");
		lua_pushliteral(L, "must not access this metatable");
		lua_settable(L, -3);
    }
    lua_pop(L, 1);  // The MT is saved in the Lua's registry.
    
    static struct luaL_Reg lib_functions[] = {
        {"calculate", calculate},
        {"new", ctx_new},
        {NULL, NULL}
    };

    luaL_register(L, LIB_NAME, lib_functions);

    lua_pushliteral (L, "_COPYRIGHT");
    lua_pushliteral (L, "Original Copyright (C) 2003-2019 PUC-Rio");
    lua_settable (L, -3);
    lua_pushliteral (L, "_DESCRIPTION");
    lua_pushliteral (L, "Checksum facilities for Lua (shipped with TotalWar WH3)");
    lua_settable (L, -3);
    lua_pushliteral (L, "_VERSION");
    lua_pushliteral (L, "2.0.0; MD5 1.3");
    lua_settable (L, -3);
    lua_pushliteral (L, "_AUTHOR");
    lua_pushliteral (L, "ZenQ (Original author: Roberto Ierusalimschy)");
    lua_settable (L, -3);

    return 1;
}
