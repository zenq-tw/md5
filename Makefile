include ./config.mk



build: sources dll clean

test:  sources dll test_c execute_api_test clean


clean: clean_objects clean_exe
	@echo done

clean_all: clean_dll clean_objects clean_exe
	@echo done

# ----------------------------------------------

sources: $(SOURCES)


dll: 
	@echo making "$(LIB_NAME).dll"...
	@echo -------------
	$(COMPILER) $(LINK_OPTIONS) -o build/$(LIB_NAME).dll $(SOURCES:%=build/%.o) $(LUA_DLL)
	@echo =============


$(SOURCES) :
	@echo building "$@" source file...
	@echo -------------
	$(COMPILER) -o build/$@.o -c src/$@.c $(CFLAGS)
	@echo =============


# ----------------------------------------------


clean_objects:
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".o"		(del build\%%a)

clean_exe: clean_objects
	@for /f "delims=" %%a in ('dir /b build\') do if /i    	"%%~xa"==".exe"		(del build\%%a)

clean_dll: clean_objects
	@for /f "delims=" %%a in ('dir /b build\') do if /i		"%%~xa"==".dll"		(del build\%%a)


# ----------------------------------------------


test_c: make_md5c_test	execute_md5c_test
test_api: build 		execute_api_test


make_md5c_test:
	@echo compiling C test for MD5...
	@echo -------------
	$(COMPILER) -o build/$(MD5_C).o  		-c src/$(MD5_C).c			$(CFLAGS)
	$(COMPILER) -o build/$(MD5_C_TEST).o 	-c tests/$(MD5_C_TEST).c  	$(CFLAGS_TEST)
	$(COMPILER) -o build/$(MD5_C_TEST).exe     build/$(MD5_C_TEST).o  	build/$(MD5_C).o 
	@echo =============


execute_md5c_test:
	@echo executing "$(MD5_C_TEST).exe"...
	@echo -------------
	@./build/$(MD5_C_TEST).exe
	@echo =============


execute_api_test:
	@echo executing "$(API_LUA_TEST)"...
	@echo -------------
	@cmd /C "set LUA_CPATH=build\?.dll&&$(LUA_INTERPRETER) tests/$(API_LUA_TEST)"
	@echo =============
