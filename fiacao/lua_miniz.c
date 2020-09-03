#include <stdio.h>
#include "miniz.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

struct miniz_arch {
	mz_zip_archive *archive;
};

int miniz_open (lua_State *L) {
	/* verify passed arguments */
	int n = lua_gettop(L);    /* number of arguments */
	if (n < 1){
		lua_pushliteral(L, "open: invalid number of arguments");
		lua_error(L);
	}
	luaL_argcheck(L, lua_isstring(L, 1), 1, "string expected");
	
	struct miniz_arch * zip;
	
	/* create a userdata object */
	zip = (struct miniz_arch *) lua_newuserdata(L, sizeof(struct miniz_arch *)); 
	luaL_getmetatable(L, "Zip");
	lua_setmetatable(L, -2);
	
	zip->archive = malloc(sizeof(mz_zip_archive));
	
	if(!zip->archive){
		lua_pushnil(L); /* return fail */
		return 1;
	}
	
	/* try to open the archive. */
	if (!mz_zip_reader_init_file(zip->archive, lua_tostring(L, 1), 0) )  {
		free(zip->archive);
		lua_pop(L, 1);
		lua_pushnil(L); /* return fail */
		return 1;
	}
	
	return 1;
}

int miniz_close (lua_State *L) {
	
	struct miniz_arch * zip;
	
	/* verify passed arguments */
	int n = lua_gettop(L);    /* number of arguments */
	if (n < 1){
		lua_pushliteral(L, "close: invalid number of arguments");
		lua_error(L);
	}
	if (!( zip =  luaL_checkudata(L, 1, "Zip") )) { /* the archive is a Lua userdata type*/
		lua_pushliteral(L, "close: incorrect argument type");
		lua_error(L);
	}
	
	/* check if it is not closed */
	luaL_argcheck(L, zip->archive != NULL, 1, "archive is closed");
	
	/* Close the archive, freeing any resources it was using */
	mz_zip_reader_end(zip->archive);
	free(zip->archive);
	zip->archive = NULL;
	
	lua_pushboolean(L, 1); /* return success */
	return 1;
}

int miniz_read (lua_State *L) {
	
	struct miniz_arch * zip;
	
	/* verify passed arguments */
	int n = lua_gettop(L);    /* number of arguments */
	if (n < 2){
		lua_pushliteral(L, "read: invalid number of arguments");
		lua_error(L);
	}
	if (!( zip =  luaL_checkudata(L, 1, "Zip") )) { /* the archive is a Lua userdata type*/
		lua_pushliteral(L, "read: incorrect argument type");
		lua_error(L);
	}
	/* check if it is not closed */
	luaL_argcheck(L, zip->archive != NULL, 1, "archive is closed");
	
	luaL_argcheck(L, lua_isstring(L, 2), 2, "string expected");
	
	/* Try to extract one files to the heap. */
	size_t size;
	char *p = mz_zip_reader_extract_file_to_heap(zip->archive, lua_tostring(L, 2), &size, 0);
	if (!p){
		lua_pushnil(L); /* return fail */
		return 1;
	}
	luaL_Buffer b;
	char *buff = luaL_buffinitsize (L, &b, size);
	memcpy(buff, p, size);
	luaL_pushresultsize (&b, size);
	
	/* free resources */
	mz_free(p);
	
	/* return success */
	return 1;
}


static const struct luaL_Reg miniz_meths[] = {
	{"read", miniz_read},
	{"close", miniz_close},
	{"__gc", miniz_close},
	{NULL, NULL}
};
static const struct luaL_Reg miniz_funcs[] = {
	{"open", miniz_open},
	{NULL, NULL}
};
int luaopen_miniz (lua_State *L) {
	/* create a new type of lua userdata to represent a ZIP archive */
	/* create metatable */
	luaL_newmetatable(L, "Zip");
	/* metatable.__index = metatable */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	/* register methods */
	luaL_setfuncs(L, miniz_meths, 0);
	/* register functions*/
	luaL_newlib(L, miniz_funcs);
	return 1;
}
