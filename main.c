#include <stdio.h>
#include <stdlib.h>
#include "containers.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


// used for debugging purposes
static void stackdump(lua_State *L)
{
  int top = lua_gettop(L);
  for (int i = 1; i <= top; i++)
  {
    int t = lua_type(L, i);
    switch (t) {
      case LUA_TSTRING:
      {
        printf("'%s'", lua_tostring(L, i));
        break;
      }
      case LUA_TBOOLEAN:
      {
        printf(lua_toboolean(L, i) ? "true" : "false");
        break;
      }
      case LUA_TNUMBER:
      {
        printf("%g", lua_tonumber(L, i));
        break;
      }
      default:
      {
        printf("%s", lua_typename(L, t));
        break;
      }
    }
    printf("  ");
  }
  printf("\n");
}


static int set_new(lua_State *L)
{
  int stack_depth = lua_gettop(L);
  if (stack_depth == 0) {
    lc_set *a = lua_newuserdata(L, sizeof(lc_set*));
    *a = lc_newset(L);
  } else {
    luaL_checktype(L, 1, LUA_TFUNCTION);
    int funref = luaL_ref(L, LUA_REGISTRYINDEX);
    lc_set *a = lua_newuserdata(L, sizeof(lc_set*));
    *a = lc_newset_w_comp(L, funref);
  }
  luaL_setmetatable(L, "containers_set");
  return 1;
}

static int set_call(lua_State *L)
{
  lua_insert(L, 1);
  lua_pop(L, 1);
  return set_new(L);
}

static int set_count(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_set *a = lua_touserdata(L, 1);
  int ret = lc_set_count(*a, ref);
  lua_pushinteger(L, ret);
  return 1;
}

static int set_insert(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_set *a = lua_touserdata(L, 1);
  lc_set_insert(*a, ref);
  return 0;
}

static int set_size(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  lc_set *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_set_size(*a));
  return 1;
}

static int set_top(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  lc_set *a = lua_touserdata(L, 1);
  int ret = lc_set_top(*a);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ret);
  return 1;
}

static int set_pop(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  lc_set *a = lua_touserdata(L, 1);
  int ret = lc_set_pop(*a);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ret);
  return 1;
}

static int set_gc(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);
  lc_set_finalize(*a);
  return 0;
}

static int set_tostring(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);

  // get reference array
  int *refs;
  int len = lc_set_torefarray(*a, &refs);

  // push table.concat function
  lua_getglobal(L, "table");
  lua_getfield(L, -1, "concat");

  // pop `table` table
  lua_remove(L, -2);

  // create a list with length of `len + 2`
  lua_createtable(L, len+2, 0);

  // set first element
  lua_pushstring(L, "< set:");
  lua_rawseti(L, -2, 1);

  for (unsigned i = 0; i < len; i++)
  {
    lua_getglobal(L, "tostring");
    lua_rawgeti(L, LUA_REGISTRYINDEX, refs[i]);
    lua_call(L, 1, 1);
    lua_rawseti(L, -2, i+2);
  }

  lua_pushstring(L, ">");
  lua_rawseti(L, -2, len+2);

  lua_pushstring(L, " ");

  lua_call(L, 2, 1);
  free(refs);
  return 1;
}

static int queue_new(lua_State *L)
{
  lc_queue *a = lua_newuserdata(L, sizeof(lc_queue*));
  *a = lc_newqueue(L);
  luaL_setmetatable(L, "containers_queue");
  return 1;
}

static int queue_size(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue");
  lc_set *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_queue_size(*a));
  return 1;
}

static int queue_push(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_queue *a = lua_touserdata(L, 1);
  lc_queue_push(*a, ref);
  return 0;
}

static int queue_pop(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_queue");
  lc_queue *a = lua_touserdata(L, 1);
  int ret = lc_queue_pop(*a);
  lua_rawgeti(L, LUA_REGISTRYINDEX, ret);
  return 1;
}

static int queue_gc(lua_State *L)
{
  lc_queue *a = lua_touserdata(L, 1);
  lc_queue_finalize(*a);
  return 0;
}

static const struct luaL_Reg containerlib_set [] = {
  {"new", set_new},
  {"count", set_count},
  {"insert", set_insert},
  {"size", set_size},
  {"top", set_top},
  {"pop", set_pop},
  {"__gc", set_gc},
  {"__tostring", set_tostring},
  {NULL, NULL}
};

static const struct luaL_Reg containerlib_set_mt [] = {
  {"__call", set_call},
  {NULL, NULL}
};

static const struct luaL_Reg containerlib_queue [] = {
  {"new", queue_new},
  {"size", queue_size},
  {"push", queue_push},
  {"pop", queue_pop},
  {"__gc", queue_gc},
  {NULL, NULL}
};

int luaopen_containerlib(lua_State *L)
{
  // load Set
  luaL_newmetatable(L, "containers_set");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, containerlib_set, 0);

  // setmetatable(containers_set, { __call = ... })
  lua_newtable(L);
  luaL_setfuncs(L, containerlib_set_mt, 0);
  lua_setmetatable(L, 1);

  lua_setglobal(L, "Set");

  // load Queue
  // TODO: setmetatable(containers_queue, { __call = ... })
  luaL_newmetatable(L, "containers_queue");
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, containerlib_queue, 0);
  lua_setglobal(L, "Queue");

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
  load(L, "test2.lua");

  lua_close(L);

  return 0;
}
