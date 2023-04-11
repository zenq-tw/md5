include ./config.mk


MD5_C = md5
MD5_C_TEST = test_md5

SOURCES= $(MD5_C) api
DLL_NAME = md5.dll


all: sources dll
	@echo done

$(SOURCES) :
	@echo building "$@" source file...
	@echo -------------
	$(COMPILER) -o build/$@.o -c src/$@.c $(CFLAGS)
	@echo =============

dll: 
	@echo making "$(DLL_NAME)"...
	@echo -------------
	$(COMPILER) $(LINK_OPTIONS) -o build/$(DLL_NAME) $(SOURCES:%=build/%.o) $(LUA_DLL)
	@echo =============

make_c_test:
	@echo compiling C test for MD5...
	@echo -------------
	$(COMPILER) -o build/$(MD5_C).o  		-c src/$(MD5_C).c			$(CFLAGS)
	$(COMPILER) -o build/$(MD5_C_TEST).o 	-c tests/$(MD5_C_TEST).c  	$(CFLAGS_TEST)
	$(COMPILER) -o build/$(MD5_C_TEST).exe     build/$(MD5_C_TEST).o  	build/$(MD5_C).o 
	@echo =============

test_c: make_c_test
	@echo executing "build/$(MD5_C_TEST).exe"...
	@echo -------------
	@./build/$(MD5_C_TEST).exe
	@echo =============

clean:
	del -f $(MD5_OBJS) build/$(MD5_DLL)
