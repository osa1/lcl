#include <stdio.h>
#include <stdlib.h>
#include "containers.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


static int set_new(lua_State *L)
{
  lc_set *a = lua_newuserdata(L, sizeof(lc_set*));
  *a = lc_newset(L);
  luaL_setmetatable(L, "containers_set_mt");
  return 1;
}

static int set_count(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set_mt");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_set *a = lua_touserdata(L, 1);
  int ret = lc_set_count(*a, ref);
  lua_pushinteger(L, ret);
  return 1;
}

static int set_insert(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set_mt");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_set *a = lua_touserdata(L, 1);
  lc_set_insert(*a, ref);
  return 0;
}

static int set_size(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set_mt");
  lc_set *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_set_size(*a));
  return 1;
}

static int queue_new(lua_State *L)
{
  lc_queue *a = lua_newuserdata(L, sizeof(lc_queue*));
  *a = lc_newqueue(L);
  luaL_setmetatable(L, "containers_queue_mt");
  return 1;
}

static int queue_size(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue_mt");
  lc_set *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_queue_size(*a));
  return 1;
}

static int queue_push(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue_mt");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_queue *a = lua_touserdata(L, 1);
  lc_queue_push(*a, ref);
  return 0;
}

static int queue_pop(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue_mt");
  lc_queue *a = lua_touserdata(L, 1);
  int ret = lc_queue_pop(*a);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ret);
  return 1;
}

static const struct luaL_Reg containerlib [] = {
  {"set_new", set_new},
  {"set_count", set_count},
  {"set_insert", set_insert},
  {"set_size", set_size},

  {"queue_new", queue_new},
  {"queue_size", queue_size},
  {"queue_push", queue_push},
  {"queue_pop", queue_pop},

  {NULL, NULL}
};

int luaopen_containerlib(lua_State *L)
{
  luaL_newmetatable(L, "containers_set_mt");
  luaL_newmetatable(L, "containers_queue_mt");

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
