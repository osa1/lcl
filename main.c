#include <stdio.h>
#include <stdlib.h>
#include "containers.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#if LUA_VERSION_NUM == 501
void luaL_setmetatable(lua_State *L, const char *tname) {
  luaL_getmetatable(L, tname);
  lua_setmetatable(L, -2);
}
#endif

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

static void refarray_to_string(lua_State *L, const char *cname, int *refs, unsigned len)
{
  // push table.concat function
  lua_getglobal(L, "table");
  lua_getfield(L, -1, "concat");

  // pop `table` table
  lua_remove(L, -2);

  // create a list with length of `len + 2`
  lua_createtable(L, len+2, 0);

  // set first element
  lua_pushstring(L, cname);
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
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_set_top(*a));
  return 1;
}

static int set_pop(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_set");
  lc_set *a = lua_touserdata(L, 1);
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_set_pop(*a));
  return 1;
}

static int set_gc(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);

  int *refs;
  int len = lc_set_torefarray(*a, &refs);

  for (int i = 0; i < len; i++)
    luaL_unref(L, LUA_REGISTRYINDEX, refs[i]);

  lc_set_finalize(*a);
  free(refs);
  return 0;
}

static int set_tostring(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);
  int *refs;
  unsigned len = lc_set_torefarray(*a, &refs);
  refarray_to_string(L, "< set", refs, len);
  free(refs);
  return 1;
}

static int deque_new(lua_State *L)
{
  lc_deque *a = lua_newuserdata(L, sizeof(lc_deque*));
  *a = lc_newdeque();
  luaL_setmetatable(L, "containers_deque");
  return 1;
}

static int deque_call(lua_State *L)
{
  lua_insert(L, 1);
  lua_pop(L, 1);
  return deque_new(L);
}

static int deque_size(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  lua_pushinteger(L, lc_deque_size(*a));
  return 1;
}

static int deque_front(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_deque_front(*a));
  return 1;
}

static int deque_back(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_deque_back(*a));
  return 1;
}

static int deque_insert(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  int idx = luaL_checkint(L, 2);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_deque_insert(*a, idx-1, ref);
  return 0;
}

static int deque_push_front(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_deque *a = lua_touserdata(L, 1);
  lc_deque_push_front(*a, ref);
  return 0;
}

static int deque_push_back(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);
  lc_deque *a = lua_touserdata(L, 1);
  lc_deque_push_back(*a, ref);
  return 0;
}

static int deque_pop_front(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_deque_pop_front(*a));
  return 1;
}

static int deque_pop_back(lua_State *L)
{
  luaL_checkudata(L, 1, "containers_deque");
  lc_deque *a = lua_touserdata(L, 1);
  lua_rawgeti(L, LUA_REGISTRYINDEX, lc_deque_pop_back(*a));
  return 1;
}

static int deque_gc(lua_State *L)
{
  lc_deque *a = lua_touserdata(L, 1);

  int *refs;
  int len = lc_deque_torefarray(*a, &refs);

  for (int i = 0; i < len; i++)
    luaL_unref(L, LUA_REGISTRYINDEX, refs[i]);

  lc_deque_finalize(*a);
  free(refs);
  return 0;
}

static int deque_tostring(lua_State *L)
{
  lc_set *a = lua_touserdata(L, 1);
  int *refs;
  unsigned len = lc_deque_torefarray(*a, &refs);
  refarray_to_string(L, "< deque", refs, len);
  free(refs);
  return 1;
}

static int deque_sort(lua_State *L)
{
  int stack_depth = lua_gettop(L);
  if (stack_depth == 1) {
    lc_set *a = lua_touserdata(L, 1);
    lc_deque_sort(L, *a);
  } else {
    luaL_checktype(L, -1, LUA_TFUNCTION);
    int funref = luaL_ref(L, LUA_REGISTRYINDEX);
    lc_set *a = lua_touserdata(L, -1);
    lc_deque_sort_w_comp(L, *a, funref);
    luaL_unref(L, LUA_REGISTRYINDEX, funref);
  }
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

static const struct luaL_Reg containerlib_deque [] = {
  {"new", deque_new},
  {"size", deque_size},
  {"front", deque_front},
  {"back", deque_back},
  {"insert", deque_insert},
  {"push_front", deque_push_front},
  {"push_back", deque_push_back},
  {"pop_front", deque_pop_front},
  {"pop_back", deque_pop_back},
  {"sort", deque_sort},
  {"__gc", deque_gc},
  {"__tostring", deque_tostring},
  {NULL, NULL}
};

static const struct luaL_Reg containerlib_deque_mt [] = {
  {"__call", deque_call},
  {NULL, NULL}
};

static void loadlib(lua_State *L, const char *global_name,
    const char *mt_name, const luaL_Reg *funs, const luaL_Reg *mt_funs)
{
  luaL_newmetatable(L, mt_name);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

#if LUA_VERSION_NUM == 501
  luaL_register(L, NULL, funs);
#else
  luaL_setfuncs(L, funs, 0);
#endif

  // setmetatable(mt_name, { __call = ... })
  lua_newtable(L);
#if LUA_VERSION_NUM == 501
  luaL_register(L, NULL, mt_funs);
#else
  luaL_setfuncs(L, mt_funs, 0);
#endif

  lua_setmetatable(L, 1);

  lua_setglobal(L, global_name);
}

int luaopen_containerlib(lua_State *L)
{
  loadlib(L, "Set", "containers_set", containerlib_set, containerlib_set_mt);
  loadlib(L, "Deque", "containers_deque", containerlib_deque, containerlib_deque_mt);
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
  load(L, "tests/test_main.lua");
  lua_close(L);
  return 0;
}
