#include "containers.h"

#include <cstdlib>

#include <algorithm>
#include <deque>
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

typedef set<int, LuaComparator> LuaSTLSet;

class Set
{
  lua_State *L;
  int comparator_ref;
public:
  LuaSTLSet *stl_set;
  Set(lua_State *L, LuaSTLSet *stl_set)
    : L(L), comparator_ref(-1), stl_set(stl_set) {}
  Set(lua_State *L, LuaSTLSet *stl_set, int comparator_ref)
    : L(L), comparator_ref(comparator_ref), stl_set(stl_set) {}
  virtual ~Set()
  {
    if (comparator_ref != -1)
      luaL_unref(L, LUA_REGISTRYINDEX, comparator_ref);
    delete stl_set;
  }
};

lc_set lc_newset(lua_State *L)
{
  return new Set(L, new set<int, LuaComparator>(LuaComparator(L)));
}

lc_set lc_newset_w_comp(lua_State *L, int funref)
{
  return new Set(L, new set<int, LuaComparator>(LuaComparator(L, funref)), funref);
}

int lc_set_count(lc_set s, int e)
{
  auto s1 = static_cast<Set*>(s);
  return s1->stl_set->count(e);
}

void lc_set_insert(lc_set s, int e)
{
  auto s1 = static_cast<Set*>(s);
  s1->stl_set->insert(e);
}

unsigned lc_set_size(lc_set s)
{
  auto s1 = static_cast<Set*>(s);
  return s1->stl_set->size();
}

int lc_set_top(lc_set s)
{
  auto s1 = static_cast<Set*>(s);
  return *s1->stl_set->begin();
}

int lc_set_pop(lc_set s)
{
  auto s1 = static_cast<Set*>(s);
  // TODO: set should not be empty
  int ret = *s1->stl_set->begin();
  s1->stl_set->erase(s1->stl_set->begin());
  return ret;
}

unsigned lc_set_torefarray(lc_set s, int **arr)
{
  auto s1 = static_cast<Set*>(s);
  *arr = static_cast<int*>(malloc(sizeof(int) * s1->stl_set->size()));
  int arridx = 0;
  for (int r : *s1->stl_set)
    (*arr)[arridx++] = r;
  return s1->stl_set->size();
}

void lc_set_finalize(lc_set s)
{
  auto s1 = static_cast<Set*>(s);
  delete s1;
}

lc_deque lc_newdeque()
{
  return new deque<int>();
}

unsigned lc_deque_size(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  return d1->size();
}

int lc_deque_front(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  return d1->front();
}

int lc_deque_back(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  return d1->back();
}

void lc_deque_insert(lc_deque d, int idx, int elem)
{
  auto d1 = static_cast<deque<int>*>(d);
  int i = 0;
  for (auto iter = d1->begin(); iter != d1->end(); iter++)
    if (i++ == idx) {
      d1->insert(iter, elem);
      break;
    }
}

void lc_deque_push_front(lc_deque d, int elem)
{
  auto d1 = static_cast<deque<int>*>(d);
  d1->push_front(elem);
}

void lc_deque_push_back(lc_deque d, int elem)
{
  auto d1 = static_cast<deque<int>*>(d);
  d1->push_back(elem);
}

int lc_deque_pop_front(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  int ret = d1->front();
  d1->pop_front();
  return ret;
}

int lc_deque_pop_back(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  int ret = d1->back();
  d1->pop_back();
  return ret;
}

void lc_deque_finalize(lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  delete d1;
}

unsigned lc_deque_torefarray(lc_deque d, int **arr)
{
  auto d1 = static_cast<deque<int>*>(d);
  *arr = static_cast<int*>(malloc(sizeof(int) * d1->size()));
  int arridx = 0;
  for (int r : *d1)
    (*arr)[arridx++] = r;
  return d1->size();
}

void lc_deque_sort(lua_State *L, lc_deque d)
{
  auto d1 = static_cast<deque<int>*>(d);
  sort(d1->begin(), d1->end(), LuaComparator(L));
}

void lc_deque_sort_w_comp(lua_State *L, lc_deque d, int funref)
{
  auto d1 = static_cast<deque<int>*>(d);
  sort(d1->begin(), d1->end(), LuaComparator(L, funref));
}
