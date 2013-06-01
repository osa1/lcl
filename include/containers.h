#ifdef __cplusplus
#define EXTERNC extern "C"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#else
#define EXTERNC
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif

lua_State *L;

typedef void* lc_set;
typedef void* lc_deque;

EXTERNC lc_set    lc_newset(lua_State *L);
EXTERNC lc_set    lc_newset_w_comp(lua_State *L, int);
EXTERNC int       lc_set_count(lc_set set, int elem);
EXTERNC void      lc_set_insert(lc_set set, int elem);
EXTERNC unsigned  lc_set_size(lc_set set);
EXTERNC int       lc_set_top(lc_set set);
EXTERNC int       lc_set_pop(lc_set set);
EXTERNC void      lc_set_finalize(lc_set set);
EXTERNC unsigned  lc_set_torefarray(lc_set set, int **arr);


EXTERNC lc_deque  lc_newdeque();
EXTERNC unsigned  lc_deque_size(lc_deque deque);
EXTERNC int       lc_deque_front(lc_deque deque);
EXTERNC int       lc_deque_back(lc_deque deque);
EXTERNC void      lc_deque_insert(lc_deque deque, int idx, int elem);
EXTERNC void      lc_deque_push_front(lc_deque deque, int elem);
EXTERNC void      lc_deque_push_back(lc_deque deque, int elem);
EXTERNC int       lc_deque_pop_front(lc_deque deque);
EXTERNC int       lc_deque_pop_back(lc_deque deque);
EXTERNC void      lc_deque_finalize(lc_deque deque);
EXTERNC unsigned  lc_deque_torefarray(lc_deque deque, int **arr);

#undef EXTERNC
