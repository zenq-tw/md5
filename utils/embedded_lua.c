#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>


#define LUA_OK 0


/* Copied from lua.c */

static lua_State *globalL = NULL;


static void lstop (lua_State *L, lua_Debug *ar) {
	(void)ar;  /* unused arg. */
	lua_sethook(L, NULL, 0, 0);  /* reset hook */
	luaL_error(L, "interrupted!");
}


static void laction (int i) {
	signal(i, SIG_DFL); /* if another SIGINT happens, terminate process */
	lua_sethook(globalL, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}


static int msghandler (lua_State *L) {
	const char *msg = lua_tostring(L, 1);

	if (msg == NULL) {  /* is error object not a string? */
		if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
				lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
			return 1;  /* that is the message */
		else
			msg = lua_pushfstring(L, "(error object is a %s value)", luaL_typename(L, 1));
	}

	/* Call debug.traceback() instead of luaL_traceback() for Lua 5.1 compatibility. */
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
    
	/* debug */
	lua_remove(L, -2);
	lua_pushstring(L, msg);

	/* original msg */
	lua_remove(L, -3);
	lua_pushinteger(L, 2);  /* skip this function and traceback */

	lua_call(L, 2, 1); /* call debug.traceback */

	return 1;  /* return the traceback */
}

static int docall (lua_State *L, int narg, int nres) {
	int status;
	int base = lua_gettop(L) - narg;  /* function index */

	lua_pushcfunction(L, msghandler);  /* push message handler */
	lua_insert(L, base);  /* put it under function and args */
	globalL = L;  /* to be available to 'laction' */
	signal(SIGINT, laction);  /* set C-signal handler */

	status = lua_pcall(L, narg, nres, base);
	
    signal(SIGINT, SIG_DFL); /* reset C-signal handler */
    lua_remove(L, base);  /* remove message handler from the stack */

    return status;
}

/* end of copied code */


static inline void process_lua_error(lua_State *L, int error_code){
    const char* error_message = lua_tostring(L, -1);
    printf("Error (%d): %s\n", error_code, error_message);
    lua_close(L);
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Lua test file expected\n");
        return 1;

    } else if (argc != 2) {
        printf("One and only one filename must be provided!\n");
        return 2;
    }

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

    int error_code = luaL_loadfile(L, argv[1]);
    if (error_code) {
        process_lua_error(L, error_code);
        return 3;
    }

    error_code = docall(L, 0, 0);
    if (error_code) {
        process_lua_error(L, error_code);
        return 4;
    }

	lua_close(L);
	return 0;
}
