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

typedef void* lc_queue;
typedef void* lc_stack;
typedef void* lc_vector;
typedef void* lc_set;

EXTERNC lc_set lc_newset(lua_State*);
EXTERNC int lc_set_count(lc_set set, int elem);
EXTERNC void lc_set_insert(lc_set set, int elem);
EXTERNC int lc_set_size(lc_set set);

EXTERNC lc_queue lc_newqueue(lua_State*);
EXTERNC int lc_queue_size(lc_queue queue);
EXTERNC void lc_queue_push(lc_queue queue, int elem);
EXTERNC int lc_queue_pop(lc_queue queue);

EXTERNC lc_stack lc_newstack();
EXTERNC void lc_stack_push(lc_stack, int);

EXTERNC lc_vector lc_newvector();

#undef EXTERNC
