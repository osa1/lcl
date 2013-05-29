#include <stdio.h>
#include <stdlib.h>
#include "containers.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


static int newset(lua_State *L)
{
  lc_set *a = lua_newuserdata(L, sizeof(lc_set*));
  *a = lc_newset(L);
  /*luaL_getmetatable(L, "Containers.set");*/
  /*lua_setmetatable(L, 1);*/
  return 1;
}

static int set_count(lua_State *L)
{
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  /*lc_set *a = luaL_checkudata(L, 1, "Containers.set");*/
  lc_set *a = lua_touserdata(L, 1);
  int ret = lc_set_count(*a, ref);
  lua_pushinteger(L, ret);
  return 1;
}

static int set_insert(lua_State *L)
{
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_set *a = lua_touserdata(L, 1);
  lc_set_insert(*a, ref);
  return 0;
}

static int set_size(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_set_size(*a));
  return 1;
}

static const struct luaL_Reg containerlib [] = {
  {"newset", newset},
  {"set_count", set_count},
  {"set_insert", set_insert},
  {"set_size", set_size},
  {NULL, NULL}
};

int luaopen_containerlib(lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs(L, containerlib, 0);
  lua_setglobal(L, "containers");
  return 1;
}

void error(lua_State *L, const char *fmt, ...)
{
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(EXIT_FAILURE);
}

void load(lua_State *L, const char *fname)
{
  if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
    error(L, "cannot run file: %s", lua_tostring(L, -1));
}

int main()
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_containerlib(L);

  load(L, "test.lua");

  /*lc_queue queue = lc_newqueue(L);*/
  /*printf("created queue\n");*/



  return 0;
}
