#pragma once

#define lua_call			    lua_call__unref
#define lua_createtable			lua_createtable__unref
#define lua_getfield			lua_getfield__unref
#define lua_gettable			lua_gettable__unref
#define lua_gettop			    lua_gettop__unref
#define lua_newuserdata			lua_newuserdata__unref
#define lua_pushboolean			lua_pushboolean__unref
#define lua_pushcclosure		lua_pushcclosure__unref
#define lua_pushfstring			lua_pushfstring__unref
#define lua_pushinteger			lua_pushinteger__unref
#define lua_pushlstring			lua_pushlstring__unref
#define lua_pushlightuserdata	lua_pushlightuserdata__unref
#define lua_pushnil			    lua_pushnil__unref
#define lua_pushnumber			lua_pushnumber__unref
#define lua_pushstring			lua_pushstring__unref
#define lua_pushvalue			lua_pushvalue__unref
#define lua_rawget			    lua_rawget__unref
#define lua_rawset			    lua_rawset__unref
#define lua_replace			    lua_replace__unref
#define lua_setfield			lua_setfield__unref
#define lua_setmetatable		lua_setmetatable__unref
#define lua_settable			lua_settable__unref
#define lua_settop			    lua_settop__unref
#define lua_toboolean			lua_toboolean__unref
#define lua_tointeger			lua_tointeger__unref
#define lua_tolstring			lua_tolstring__unref
#define lua_tonumber			lua_tonumber__unref
#define lua_topointer			lua_topointer__unref
#define lua_touserdata			lua_touserdata__unref
#define lua_type			    lua_type__unref
#define lua_typename			lua_typename__unref

#define luaL_callmeta			luaL_callmeta__unref
#define luaL_checkany			luaL_checkany__unref
#define luaL_checkinteger		luaL_checkinteger__unref
#define luaL_checklstring		luaL_checklstring__unref
#define luaL_checkstack			luaL_checkstack__unref      // new
#define luaL_checktype			luaL_checktype__unref
#define luaL_checkudata			luaL_checkudata__unref
#define luaL_error			    luaL_error__unref
#define luaL_newmetatable		luaL_newmetatable__unref
#define luaL_register			luaL_register__unref




#include <lua.h>
#include <lauxlib.h>



#undef lua_call
#undef lua_createtable
#undef lua_getfield
#undef lua_gettable
#undef lua_gettop
#undef lua_newuserdata
#undef lua_pushboolean
#undef lua_pushcclosure
#undef lua_pushfstring
#undef lua_pushinteger
#undef lua_pushlightuserdata
#undef lua_pushlstring
#undef lua_pushnil
#undef lua_pushnumber
#undef lua_pushstring
#undef lua_pushvalue
#undef lua_rawget
#undef lua_rawset
#undef lua_replace
#undef lua_setfield
#undef lua_setmetatable
#undef lua_settable
#undef lua_settop
#undef lua_toboolean
#undef lua_tointeger
#undef lua_tolstring
#undef lua_tonumber
#undef lua_topointer
#undef lua_touserdata
#undef lua_type
#undef lua_typename

#undef luaL_callmeta
#undef luaL_checkany
#undef luaL_checkinteger
#undef luaL_checklstring
#undef luaL_checkstack          // new
#undef luaL_checktype
#undef luaL_checkudata
#undef luaL_error
#undef luaL_newmetatable
#undef luaL_register
