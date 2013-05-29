#include <stdio.h>
#include "containers.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main()
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  lc_queue queue = lc_newqueue(L);
  printf("created queue\n");

  return 0;
}
