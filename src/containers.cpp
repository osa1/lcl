#include "containers.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <set>

using namespace std;

class LuaComparator
{
  lua_State *L;
  int lua_comparator;
public:
  LuaComparator(lua_State *L) : L(L), lua_comparator(-1) {}
  LuaComparator(lua_State *L, int lua_comparator)
    : L(L), lua_comparator(lua_comparator) {}
  bool operator()(int i1, int i2) const
  {
    int r;
    if (lua_comparator != -1) {
      lua_rawgeti(L, LUA_REGISTRYINDEX, lua_comparator);
      lua_rawgeti(L, LUA_REGISTRYINDEX, i1);
      lua_rawgeti(L, LUA_REGISTRYINDEX, i2);
      lua_call(L, 2, 1);
      r = lua_toboolean(L, -1);
      lua_pop(L, 1);
    } else {
      lua_rawgeti(L, LUA_REGISTRYINDEX, i1);
      lua_rawgeti(L, LUA_REGISTRYINDEX, i2);
      r = lua_compare(L, -2, -1, LUA_OPLT);
    }
    if (r == 1)
      return true;
    return false;
  }
};

lc_set lc_newset(lua_State *L)
{
  return new set<int, LuaComparator>(LuaComparator(L));
}

lc_set lc_newset_w_comp(lua_State *L, int funref)
{
  return new set<int, LuaComparator>(LuaComparator(L, funref));
}

int lc_set_count(lc_set s, int e)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  return s1->count(e);
}

void lc_set_insert(lc_set s, int e)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  s1->insert(e);
}

int lc_set_size(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  return s1->size();
}

int lc_set_top(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  return *s1->begin();
}

int lc_set_pop(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  // TODO: set should not be empty
  int ret = *s1->begin();
  s1->erase(s1->begin());
  return ret;
}

void lc_set_finalize(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  delete s1;
}

const char *lc_set_tostring(lc_set s)
{
  auto s1 = static_cast<set<int, LuaComparator>*>(s);
  stringstream ss;
  ss << "<set:";
  for (int i : *s1)
    ss << " " << i;
  ss << ">";
  return ss.str().c_str();
}

lc_queue lc_newqueue(lua_State *L)
{
  return new queue<int>();
}

int lc_queue_size(lc_queue q)
{
  auto q1 = static_cast<queue<int>*>(q);
  return q1->size();
}

void lc_queue_push(lc_queue q, int e)
{
  auto q1 = static_cast<queue<int>*>(q);
  q1->push(e);
}

int lc_queue_pop(lc_queue q)
{
  auto q1 = static_cast<queue<int>*>(q);
  int ret = q1->front();
  q1->pop();
  return ret;
}

void lc_queue_finalize(lc_queue q)
{
  auto q1 = static_cast<queue<int>*>(q);
  delete q1;
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
