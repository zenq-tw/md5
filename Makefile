include ./config.mk

LIB_FILE 		= $(LIB_NAME).$(LIB_E)
DUMPED_LIB_FILE = $(LIB_NAME)_$(LIB_E).lua


build: sources lib dump_lib clean


# ----------------------------------------------


test: 		$(TEST_SOURCES)		sources lib 	md5_test_exec 	embedded_lua 	execute_md5_test 	execute_api_test	clean_all

test_api: 						sources lib 					embedded_lua						execute_api_test	clean_all
test_md5: 	$(TEST_MD5_SRC)						md5_test_exec					execute_md5_test						clean

test_lib_dump:  sources  lib  dump_lib  test_dumped_lib  clean_all



clean: 		clean_objs 	clean_exec 							echo_done
clean_all: 	clean_objs 	clean_exec	clean_lib	clean_lua	echo_done


# ----------------------------------------------


sources: $(MD5_SRC) $(API_SRC) $(LUA_PROXY)


lib: 
	@echo building "$(LIB_FILE)"...
	@echo -------------
	$(CC) -o build/$(LIB_FILE)     $(SOURCES:%=build/%.o) build/$(LUA_PROXY).o     $(LDFLAGS)
	@echo =============


dump_lib:
	@echo dump $(LIB_FILE) to lua script ($(DUMPED_LIB_FILE)) ...
	@echo -------------
	$(CC) -o build/$(BIN_TO_LUA).$(EXEC_E) utils/$(BIN_TO_LUA).c
	build/$(BIN_TO_LUA).$(EXEC_E) build/$(LIB_FILE) build/$(DUMPED_LIB_FILE)
	@echo =============


# ----------------------------------------------

# TODO: replace with portable solution?

clean_objs:
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".o"			(del build\%%a)

clean_lua:
	@for /f "delims=" %%a in ('dir /b build\') do if /i		"%%~xa"==".lua"			(del build\%%a)

clean_exec:
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".$(EXEC_E)"	(del build\%%a)

clean_lib:
	@for /f "delims=" %%a in ('dir /b build\') do if /i		"%%~xa"==".$(LIB_E)"	(del build\%%a)


# ----------------------------------------------

embedded_lua: 
	@echo creating Lua static library from Lua sources ...
	@echo -------------
	@$(MAKE) -C $(LUA_SRC) a
	@echo -------------
	@echo creating "$(TEST_API).$(EXEC_E)" with embedded Lua and exposed C API (like WH3 does)  ...
	$(CC) -o build/$(TEST_API).$(EXEC_E)	utils/$(TEST_EMBEDDED_LUA_SRC).c $(LUA_SRC)/liblua.a	-I$(LUA_SRC) -Wl,--export-all-symbols
	@echo =============


# ----------------------------------------------

md5_test_exec:
	@echo compiling C test for MD5...
	@echo -------------
	$(CC) -o build/$(TEST_MD5_SRC).$(EXEC_E)     build/$(MD5_SRC).o  	build/$(TEST_MD5_SRC).o
	@echo =============


execute_md5_test:
	@echo executing "$(TEST_MD5_SRC).$(EXEC_E)"...
	@echo -------------
	@./build/$(TEST_MD5_SRC).$(EXEC_E)
	@echo =============


execute_api_test:
	@echo executing "$(TEST_API).lua" in embedded environment ...
	@echo -------------
	@cmd /C "set LUA_CPATH=build\?.dll&&.\build\$(TEST_API).exe tests/$(TEST_API).lua"
	@echo =============


RESTORED_LIB_FILE = $(LIB_NAME)__restored.$(LIB_E)

test_dumped_lib:
	@echo restore dumped lib with lua script ...
	@$(LUA_SRC)/lua.$(EXEC_E) utils/$().lua build/$(DUMPED_LIB_FILE) build/$(RESTORED_LIB_FILE)
	@echo -------------
	@echo compare restored lib with original one ...
	@echo -------------
# don't use fс.exe because it outputs too much text when binary files don't match
	@echo N | comp.exe build\$(LIB_FILE) build\$(RESTORED_LIB_FILE)
	@echo N
	@echo =============


# ----------------------------------------------


$(SOURCES) :
	@echo compiling "$@" source file...
	@echo -------------
	$(CC) -o build/$@.o     -c src/$@.c     $(CFLAGS)
	@echo =============

$(TEST_SOURCES) :
	@echo compiling "$@" test source file...
	@echo -------------
	$(CC) -o build/$@.o     -c tests/$@.c     $(CFLAGS_TEST)
	@echo =============


$(LUA_PROXY) :
	@echo compiling "$@" source file...
	@echo -------------
	$(CC) -o build/$@.o     -c src/$@.c     $(CFLAGS)     -Wno-cast-function-type -Wno-incompatible-pointer-types -Wno-missing-prototypes
	@echo =============



echo_done:
	@echo done
