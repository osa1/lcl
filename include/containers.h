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
EXTERNC lc_queue lc_newqueue(lua_State*);

EXTERNC lc_stack lc_newstack();
EXTERNC void lc_stack_push(lc_stack, int);

EXTERNC lc_vector lc_newvector();



#undef EXTERNC
