#include "containers.h"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>

using namespace std;

class LuaComparator
{
  lua_State *L;
public:
  LuaComparator(lua_State *L) : L(L) {}
  bool operator()(int i1, int i2) const
  {
    lua_rawgeti(L, LUA_REGISTRYINDEX, i1);
    lua_rawgeti(L, LUA_REGISTRYINDEX, i2);
    int r = lua_compare(L, -1, -2, LUA_OPLT);
    if (r == 1)
      return true;
    return false;
  }
};

lc_set lc_newset(lua_State *L)
{
  return new set<int, LuaComparator>(LuaComparator(L));
}

int lc_set_count(lc_set s, int elem)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  return s1->count(elem);
}

void lc_set_insert(lc_set s, int elem)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  s1->insert(elem);
}

int lc_set_size(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  return s1->size();
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
