#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "include_lua.h"


int expose_lua_c_api(void);


// do we need "extern" for functions below?
// anyway, we clearly tell those who read this code that these are external functions from the Lua C API





// extern void		    (*lua_call)             (lua_State *L, int nargs, int nresults);
// extern void		    (*lua_createtable)      (lua_State *L, int narr, int nrec);
extern void		        (*lua_getfield)         (lua_State *L, int idx, const char *k);
// extern void		    (*lua_gettable)         (lua_State *L, int idx);
// extern int		    (*lua_gettop)           (lua_State *L);
extern void*	        (*lua_newuserdata)      (lua_State *L, size_t sz);
// extern void		    (*lua_pushboolean)      (lua_State *L, int b);
extern void		        (*lua_pushcclosure)     (lua_State *L, lua_CFunction fn, int n);
// extern const char*   (*lua_pushfstring)      (lua_State *L, const char *fmt, ...);
// extern void		    (*lua_pushinteger)      (lua_State *L, lua_Integer n);
// extern void		    (*lua_pushlightuserdata)(lua_State *L, void *p);
extern void		        (*lua_pushlstring)      (lua_State *L, const char *s, size_t l);
extern void		        (*lua_pushnil)          (lua_State *L);
// extern void		    (*lua_pushnumber)       (lua_State *L, lua_Number n);
// extern void		    (*lua_pushstring)       (lua_State *L, const char *s);
extern void		        (*lua_pushvalue)        (lua_State *L, int idx);
// extern void		    (*lua_rawget)           (lua_State *L, int idx);
// extern void		    (*lua_rawset)           (lua_State *L, int idx);
// extern void		    (*lua_replace)          (lua_State *L, int idx);
// extern void		    (*lua_setfield)         (lua_State *L, int idx, const char *k);
extern int		        (*lua_setmetatable)     (lua_State *L, int objindex);
extern void		        (*lua_settable)         (lua_State *L, int idx);
extern void		        (*lua_settop)           (lua_State *L, int idx);
// extern int		    (*lua_toboolean)        (lua_State *L, int idx);
// extern lua_Integer   (*lua_tointeger)        (lua_State *L, int idx);
// extern const char*   (*lua_tolstring)        (lua_State *L, int idx, size_t *len);
// extern lua_Number	(*lua_tonumber)         (lua_State *L, int idx);
// extern const void*   (*lua_topointer)        (lua_State *L, int idx);
// extern void*         (*lua_touserdata)       (lua_State *L, int idx);
// extern int		    (*lua_type)             (lua_State *L, int idx);
// extern const char*   (*lua_typename)         (lua_State *L, int tp);

// extern int		    (*luaL_callmeta)        (lua_State *L, int obj, const char *e);
// extern void		    (*luaL_checkany)        (lua_State *L, int narg);
// extern lua_Integer	(*luaL_checkinteger)    (lua_State *L, int numArg);
extern void		        (*luaL_checkstack)      (lua_State *L, int space, const char *mes);                 // new
extern const char*      (*luaL_checklstring)    (lua_State *L, int numArg, size_t *l);
// extern void		    (*luaL_checktype)       (lua_State *L, int narg, int t);
extern void*            (*luaL_checkudata)      (lua_State *L, int ud, const char *tname);
// extern int		    (*luaL_error)           (lua_State *L, const char *fmt, ...);
extern int		        (*luaL_newmetatable)    (lua_State *L, const char *tname);
extern void		        (*luaL_register)        (lua_State *L, const char *libname, const luaL_Reg *l);
