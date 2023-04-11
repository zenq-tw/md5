/**
*  $Id: md5.h,v 1.2 2006/03/03 15:04:49 tomas Exp $
*  Cryptographic module for Lua.
*  Original author:  Roberto Ierusalimschy
*  @author  ZenQ
*/


#ifndef md5_h
#define md5_h

#include <lua.h>
#include <stddef.h>


#define HASH_SIZE       16
#define CHUNK_SIZE      64

#if __STDC_VERSION__ >= 199901L

#include <stdint.h>
typedef uint32_t UWORD32;

#else
/* static assert that int equal or greater than 32bit. */
typedef char static_assert_sizeof_int[sizeof(unsigned int) >= 4 ? 1 : -1];
typedef unsigned int STRICT_UINT32;

#endif


typedef enum State {
    STATE_NOT_READY,
    STATE_COMPLETED,
    STATE_NEW_COMPLETED_DIGEST,
} TCalculationState;


typedef struct Buffer {
    unsigned char size; 
    char array[CHUNK_SIZE];
} TBuffer;


typedef struct md5_ctx {
  UWORD32       completed_digest[4];
  UWORD32       current_digest[4];
  size_t        len;
  TBuffer       buffer;
} md5_ctx;


void md5_ctx_init       (md5_ctx *m);
void md5_ctx_update     (md5_ctx *m, const char *message, size_t len);
void md5_ctx_finish     (md5_ctx *m, char output[HASH_SIZE]);
void md5_calculate (const char *message, size_t len, char output[HASH_SIZE]);

LUALIB_API int luaopen_md5 (lua_State *L);


#endif
