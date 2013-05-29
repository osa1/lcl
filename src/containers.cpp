#include "containers.h"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>

using namespace std;

class Container
{
  lua_State *L;
public:
  Container(lua_State *L) : L(L) {}
  bool operator()(int i1, int i2)
  {
    lua_rawgeti(L, LUA_REGISTRYINDEX, i1);
    lua_rawgeti(L, LUA_REGISTRYINDEX, i2);
    int r = lua_compare(L, 1, 2, LUA_OPLT);
    if (r == 1)
      return true;
    return false;
  }
};

lc_set lc_newset(lua_State *L)
{
  return new set<int, Container>(Container(L));
}

lc_queue lc_newqueue(lua_State *L)
{
  return new queue<int>();
}

lc_stack lc_newstack(lua_State *L)
{
  return new stack<int>();
}

void lc_stack_push(lc_stack s, int i)
{
  auto s1 = static_cast<stack<int>*>(s);
  s1->push(i);
}

lc_vector lc_newvector(lua_State *L)
{
  return new vector<int>();
}
