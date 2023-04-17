include ./config.mk



build: sources lib clean_objs


# ----------------------------------------------


test: 		$(TEST_SOURCES)		sources lib 	md5_test_exec 	embedded_lua 	execute_md5_test 	execute_api_test	clean_all

test_api: 						sources lib 					embedded_lua						execute_api_test	clean_all
test_md5: 	$(TEST_MD5_SRC)						md5_test_exec					execute_md5_test						clean



clean: 		clean_objs 	clean_exec 				echo_done
clean_all: 	clean_objs 	clean_exec	clean_lib	echo_done


# ----------------------------------------------


sources: $(MD5_SRC) $(API_SRC) $(LUA_PROXY)


lib: 
	@echo building "$(LIB_FILE)"...
	@echo -------------
	$(CC) -o build/$(LIB_FILE)     $(SOURCES:%=build/%.o) build/$(LUA_PROXY).o     $(LDFLAGS)
	@echo =============


# ----------------------------------------------

# TODO: replace with portable solution?

clean_objs:
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".o"			(del build\%%a)

clean_exec:
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".$(EXEC_E)"	(del build\%%a)

clean_lib:
	@for /f "delims=" %%a in ('dir /b build\') do if /i		"%%~xa"==".$(LIB_E)"	(del build\%%a)


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
