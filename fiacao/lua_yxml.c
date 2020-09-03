#include <stdlib.h>
#include "yxml.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define BUFSIZE 4096

struct yxml_state {
	yxml_t *x;
};

int yxml_new (lua_State *L) {
	
	struct yxml_state * state;
	
	/* create a userdata object */
	state = (struct yxml_state *) lua_newuserdata(L, sizeof(struct yxml_state *)); 
	luaL_getmetatable(L, "Yxml");
	lua_setmetatable(L, -2);
	
	/* memory allocation */
	state->x = malloc(sizeof(yxml_t) + BUFSIZE);
	
	if(!state->x){
		lua_pushnil(L); /* return fail */
		return 1;
	}
	
	/* init the parser */
	yxml_init(state->x, state->x+1, BUFSIZE);
	
	return 1;
}

int yxml_close (lua_State *L) {
	
	struct yxml_state * state;
	
	/* verify passed arguments */
	int n = lua_gettop(L);    /* number of arguments */
	if (n < 1){
		lua_pushliteral(L, "close: invalid number of arguments");
		lua_error(L);
	}
	if (!( state =  luaL_checkudata(L, 1, "Yxml") )) { /* the archive is a Lua userdata type*/
		lua_pushliteral(L, "close: incorrect argument type");
		lua_error(L);
	}
	
	/* check if it is not closed */
	luaL_argcheck(L, state->x != NULL, 1, "parser is closed");
	
	/* free any resources it was using */
	free(state->x);
	state->x = NULL;
	
	lua_pushboolean(L, 1); /* return success */
	return 1;
}

int yxml_read (lua_State *L) {
	
	struct yxml_state * state;
	
	/* verify passed arguments */
	int n = lua_gettop(L);    /* number of arguments */
	if (n < 2){
		lua_pushliteral(L, "read: invalid number of arguments");
		lua_error(L);
	}
	if (!( state =  luaL_checkudata(L, 1, "Yxml") )) { /* the parser is a Lua userdata type*/
		lua_pushliteral(L, "read: incorrect argument type");
		lua_error(L);
	}
	/* check if it is not closed */
	luaL_argcheck(L, state->x != NULL, 1, "parser is closed");
	
	luaL_argcheck(L, lua_isstring(L, 2), 2, "string expected");
	
	luaL_Buffer b;  /* to store parcial strings */
	int content = 0, attr = 0, attrval = 0;
	
	char *doc = (char*) lua_tostring(L, 2);  /*get document to parse */
	for(; *doc; doc++) { /* sweep document, analysing each character */
		yxml_ret_t r = yxml_parse(state->x, *doc);
		if(r < 0){ /* parsing error */
			lua_pushliteral(L, "parse error");
			lua_error(L);
		}
		
		/* Handle any tokens we are interested in */
		switch(r) {
			case YXML_ELEMSTART: /* start a element */
				/*previous attr table not stored yet*/
				if (attr){
					attr = 0;
					if (lua_istable(L, -1) && lua_istable(L, -2)){
						/* store table with key "attr" in owner table */
						lua_pushstring(L, "attr");
						lua_pushvalue (L, -2);
						lua_remove (L, -3);
						lua_rawset(L, -3);
					}
				}
				/* element main table */
				lua_newtable(L);
				/* store element name with key "id" */
				lua_pushstring(L, "id");
				lua_pushstring(L, state->x->elem);
				lua_rawset(L, -3);
				/*reset flags */
				content = 0;
				attr = 0;
				attrval = 0;
				break;
			case YXML_ELEMEND:
				/* element attr table not stored yet*/
				if (attr){
					attr = 0;
					if (lua_istable(L, -1) && lua_istable(L, -2)){
						/* store table with key "attr" in owner table */
						lua_pushstring(L, "attr");
						lua_pushvalue (L, -2);
						lua_remove (L, -3);
						lua_rawset(L, -3);
					}
				}
				/* store content string with key "cont" in element owner table */
				if (content && lua_istable(L, -1)){
					lua_pushstring(L, "cont");
					luaL_pushresult(&b); /* finalize string and put on Lua stack */
					lua_rawset(L, -3);
				}
				/* store element in its owner table, if exists */
				if (lua_istable(L, -1) && lua_istable(L, -2)){
					int index = lua_rawlen (L, -2) + 1;
					lua_rawseti(L, -2, index);  /* set table at key `index' */
				}
				/*reset flags */
				content = 0;
				attr = 0;
				attrval = 0;
				break;
			case YXML_CONTENT:
				if (!content){ /*init content */
					content = 1;
					luaL_buffinit(L, &b); /* init the Lua buffer */
					/* store attr table in its owner */
					if (attr){
						attr = 0;
						if (lua_istable(L, -1) && lua_istable(L, -2)){
							/* store table with key "attr" in owner table */
							lua_pushstring(L, "attr");
							lua_pushvalue (L, -2);
							lua_remove (L, -3);
							lua_rawset(L, -3);
						}
					}
				}
				/* store parcial string */
				luaL_addstring(&b, state->x->data);
				break;
			case YXML_ATTRSTART:
				if (!attr){ /* init attributes */
					attr = 1;
					lua_newtable(L); /* element attr table */
				}
				break;
			case YXML_ATTRVAL:
				if (!attrval){ /*init attribute value */
					attrval = 1;
					luaL_buffinit(L, &b); /* init the Lua buffer */
				}
				/* store parcial string */
				luaL_addstring(&b, state->x->data);
				break;
			case YXML_ATTREND:
				/* Now we have a full attribute. Its name is in x->attr, 
				and its value is in Lua buffer "b". */
				if (attrval){
					attrval = 0;
					if (lua_istable(L, -1)){
						/* store in its owner table, where its name is the key */
						lua_pushstring(L, state->x->attr);
						luaL_pushresult(&b); /* finalize string and put on Lua stack */
						lua_rawset(L, -3);
					}
				}
				break;
		}
	}
	/* end parsing */
	yxml_eof(state->x);
	/* restart parser */
	yxml_init(state->x, state->x+1, BUFSIZE);
	
	if (!lua_istable(L, -1)) lua_pushnil(L); /* return fail */
	/* or return success */
	return 1;
}


static const struct luaL_Reg yxml_meths[] = {
	{"read", yxml_read},
	{"close", yxml_close},
	{"__gc", yxml_close},
	{NULL, NULL}
};
static const struct luaL_Reg yxml_funcs[] = {
	{"new", yxml_new},
	{NULL, NULL}
};
int luaopen_yxml (lua_State *L) {
	/* create a new type of lua userdata to represent a ZIP archive */
	/* create metatable */
	luaL_newmetatable(L, "Yxml");
	/* metatable.__index = metatable */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	/* register methods */
	luaL_setfuncs(L, yxml_meths, 0);
	/* register functions*/
	luaL_newlib(L, yxml_funcs);
	return 1;
}
