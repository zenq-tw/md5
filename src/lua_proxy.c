#include <stdbool.h>

#include "lua_proxy.h"

#define OUTPUT_BUFF_SIZE 1000
#define BASE_ERROR_TITLE "Failed to load Lua C extension - "
#define FLAG_TO_RETCODE(flag) ((flag)? (0): (2))



int expose_lua_c_api(void);



// void		    (*lua_call)             (lua_State *L, int nargs, int nresults);
// void		    (*lua_createtable)      (lua_State *L, int narr, int nrec);
void		    (*lua_getfield)         (lua_State *L, int idx, const char *k);
// void		    (*lua_gettable)         (lua_State *L, int idx);
// int		    (*lua_gettop)           (lua_State *L);
void*	        (*lua_newuserdata)      (lua_State *L, size_t sz);
// void		    (*lua_pushboolean)      (lua_State *L, int b);
void		    (*lua_pushcclosure)     (lua_State *L, lua_CFunction fn, int n);
// const char*  (*lua_pushfstring)      (lua_State *L, const char *fmt, ...);
// void		    (*lua_pushinteger)      (lua_State *L, lua_Integer n);
// void		    (*lua_pushlightuserdata)(lua_State *L, void *p);
void		    (*lua_pushlstring)      (lua_State *L, const char *s, size_t l);
void		    (*lua_pushnil)          (lua_State *L);
// void		    (*lua_pushnumber)       (lua_State *L, lua_Number n);
// void		    (*lua_pushstring)       (lua_State *L, const char *s);
void		    (*lua_pushvalue)        (lua_State *L, int idx);
// void		    (*lua_rawget)           (lua_State *L, int idx);
// void		    (*lua_rawset)           (lua_State *L, int idx);
// void		    (*lua_replace)          (lua_State *L, int idx);
// void		    (*lua_setfield)         (lua_State *L, int idx, const char *k);
int		        (*lua_setmetatable)     (lua_State *L, int objindex);
void		    (*lua_settable)         (lua_State *L, int idx);
void		    (*lua_settop)           (lua_State *L, int idx);
// int		    (*lua_toboolean)        (lua_State *L, int idx);
// lua_Integer  (*lua_tointeger)        (lua_State *L, int idx);
// const char*  (*lua_tolstring)        (lua_State *L, int idx, size_t *len);
// lua_Number	(*lua_tonumber)         (lua_State *L, int idx);
// const void*  (*lua_topointer)        (lua_State *L, int idx);
// void*        (*lua_touserdata)       (lua_State *L, int idx);
// int		    (*lua_type)             (lua_State *L, int idx);
// const char*  (*lua_typename)         (lua_State *L, int tp);

// int		    (*luaL_callmeta)        (lua_State *L, int obj, const char *e);
// void		    (*luaL_checkany)        (lua_State *L, int narg);
// lua_Integer	(*luaL_checkinteger)    (lua_State *L, int numArg);
void		    (*luaL_checkstack)      (lua_State *L, int space, const char *mes);                 // new
const char*     (*luaL_checklstring)    (lua_State *L, int numArg, size_t *l);
// void		    (*luaL_checktype)       (lua_State *L, int narg, int t);
void*           (*luaL_checkudata)      (lua_State *L, int ud, const char *tname);
// int		    (*luaL_error)           (lua_State *L, const char *fmt, ...);
int		        (*luaL_newmetatable)    (lua_State *L, const char *tname);
void		    (*luaL_register)        (lua_State *L, const char *libname, const luaL_Reg *l);


static inline void _append_system_err_msg(char *output){
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        output + strlen(output),
        OUTPUT_BUFF_SIZE - strlen(output),
        NULL
    );
}


HMODULE _get_host_process() {
    HMODULE proc = GetModuleHandleA(NULL);
    if (proc == NULL) {
        char buff[OUTPUT_BUFF_SIZE];
        sprintf(buff, "%sFailed to get game process address.\nSystemErrorText: ", BASE_ERROR_TITLE);
        
        _append_system_err_msg(buff);
        printf("%s\n", buff);
    }
    return proc;
}


FARPROC _get_lua_func_address(HMODULE game_process, bool *is_all_funcs_exposed, const char *func_name) {
    FARPROC func_ptr = GetProcAddress(game_process, func_name);
    if (func_ptr == NULL) {
        (*is_all_funcs_exposed) = false;

        char buff[OUTPUT_BUFF_SIZE];
        sprintf(buff, "%sCannot find required function in host process: '%s'.\nSystemErrorText: ", BASE_ERROR_TITLE, func_name);
        
        _append_system_err_msg(buff);
    }
    return func_ptr;
}



int expose_lua_c_api(void) {
    HMODULE game = _get_host_process();
    if(game == NULL)
        return 1;


    /* 
    Incomplete list of Lua C API "functions" that are actually macros, and C API functions that these macros translate into.
        lua_pop            -> lua_settop
        lua_pushliteral    -> lua_pushlstring

        luaL_getmetatable  -> lua_getfield
    
    If you use these macros you MUST activate/add their dependencies to the initialization below
    */

    bool all_funcs_exposed = true, *f = &all_funcs_exposed;

    // lua_call             		= _get_lua_func_address(game, f, "lua_call");
	// lua_createtable      		= _get_lua_func_address(game, f, "lua_createtable");
	lua_getfield            		= _get_lua_func_address(game, f, "lua_getfield");
	// lua_gettable         		= _get_lua_func_address(game, f, "lua_gettable");
	// lua_gettop           		= _get_lua_func_address(game, f, "lua_gettop");
	lua_newuserdata         		= _get_lua_func_address(game, f, "lua_newuserdata");
	// lua_pushboolean      		= _get_lua_func_address(game, f, "lua_pushboolean");
	lua_pushcclosure        		= _get_lua_func_address(game, f, "lua_pushcclosure");
	// lua_pushfstring         		= _get_lua_func_address(game, f, "lua_pushfstring");
	// lua_pushinteger      		= _get_lua_func_address(game, f, "lua_pushinteger");
	// lua_pushlightuserdata		= _get_lua_func_address(game, f, "lua_pushlightuserdata");
	lua_pushlstring         		= _get_lua_func_address(game, f, "lua_pushlstring");
	lua_pushnil             		= _get_lua_func_address(game, f, "lua_pushnil");
	// lua_pushnumber       		= _get_lua_func_address(game, f, "lua_pushnumber");
	// lua_pushstring          		= _get_lua_func_address(game, f, "lua_pushstring");
	lua_pushvalue           		= _get_lua_func_address(game, f, "lua_pushvalue");
	// lua_rawget           		= _get_lua_func_address(game, f, "lua_rawget");
	// lua_rawset           		= _get_lua_func_address(game, f, "lua_rawset");
	// lua_replace          		= _get_lua_func_address(game, f, "lua_replace");
	// lua_setfield         		= _get_lua_func_address(game, f, "lua_setfield");
	lua_setmetatable        		= _get_lua_func_address(game, f, "lua_setmetatable");
	lua_settable            		= _get_lua_func_address(game, f, "lua_settable");
	lua_settop              		= _get_lua_func_address(game, f, "lua_settop");
	// lua_toboolean        		= _get_lua_func_address(game, f, "lua_toboolean");
	// lua_tointeger        		= _get_lua_func_address(game, f, "lua_tointeger");
	// lua_tolstring        		= _get_lua_func_address(game, f, "lua_tolstring");
	// lua_tonumber         		= _get_lua_func_address(game, f, "lua_tonumber");
	// lua_topointer        		= _get_lua_func_address(game, f, "lua_topointer");
	// lua_touserdata       		= _get_lua_func_address(game, f, "lua_touserdata");
	// lua_type             		= _get_lua_func_address(game, f, "lua_type");
	// lua_typename         		= _get_lua_func_address(game, f, "lua_typename");

	// luaL_callmeta        		= _get_lua_func_address(game, f, "luaL_callmeta");
	// luaL_checkany        		= _get_lua_func_address(game, f, "luaL_checkany");
	// luaL_checkinteger    		= _get_lua_func_address(game, f, "luaL_checkinteger");
    luaL_checkstack         		= _get_lua_func_address(game, f, "luaL_checkstack");                // new
	luaL_checklstring       		= _get_lua_func_address(game, f, "luaL_checklstring");
	// luaL_checktype       		= _get_lua_func_address(game, f, "luaL_checktype");
	luaL_checkudata         		= _get_lua_func_address(game, f, "luaL_checkudata");
	// luaL_error           		= _get_lua_func_address(game, f, "luaL_error");
	luaL_newmetatable       		= _get_lua_func_address(game, f, "luaL_newmetatable");
	luaL_register           		= _get_lua_func_address(game, f, "luaL_register");


    return FLAG_TO_RETCODE(all_funcs_exposed);
}
