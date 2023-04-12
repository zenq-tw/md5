/**
*    $Id: md5.c,v 1.2 2008/03/24 20:59:12 mascarenhas Exp $
*    Hash function MD5
*    @author    Marcela Ozorio Suarez, Roberto I.
* --------------------------------------------------------------
*    Modified by ZenQ
*    Changes:
*       - fixes for iterative checksum calculations (now it works)
*       - refactoring
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "md5.h"


#define WORD 32
#define MASK 0xFFFFFFFF
#define UNIQUENESS_BYTE '\200'


/**
* md5 hash function.
* @param message: aribtary string.
* @param len: message length.
* @param output: buffer to receive the hash value. Its size must be (at least) HASHSIZE.
*/
void md5_calculate (const char *message, size_t len, char output[HASH_SIZE]);

/**
* init a new md5 calculate context
* @param m a uninitialized md5_t type context
*/
void md5_ctx_init (md5_ctx *m);

/**
* update message to md5 context
* @param m a initialized md5_t type context
* @param message aribtary string
* @param len message length
*/
void md5_ctx_update (md5_ctx *m, const char *message, size_t len);

/**
* finish md5 calculate.
* @param m a md5_type context which previous md5_update on it return true.
* @param output buffer to receive the hash value. its size must be
* (at least) HASHSIZE.
*/
void md5_ctx_finish (md5_ctx *m, char output[HASH_SIZE]);

/*
** Performs a clockwise rotation of the bits of variable 'D' of type WORD32.
** Bits are shifted by 'num' positions
*/
#define rotate(D, num)    (D<<num) | (D>>(WORD-num))

/*Macros that define operations performed by the md5 algorithm */
#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z))))


/*vector of numbers used by the md5 algorithm to shuffle bits */
static const UWORD32 T[64]={
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


static void _word32tobytes (const UWORD32 *input, char *output) {
    int j = 0;
    while (j<4*4) {
        UWORD32 v = *input++;
        output[j++] = (char)(v & 0xff); v >>= 8;
        output[j++] = (char)(v & 0xff); v >>= 8;
        output[j++] = (char)(v & 0xff); v >>= 8;
        output[j++] = (char)(v & 0xff);
    }
}


static void _init_digest(UWORD32 *d) {
  d[0] = 0x67452301;
  d[1] = 0xEFCDAB89;
  d[2] = 0x98BADCFE;
  d[3] = 0x10325476;
}

/*function that implements the four main steps of the MD5 algorithm */
static inline void _calculate_digest(const UWORD32 *m, UWORD32 *d) {
    int j;
    /*MD5 STEP1 */
    for (j=0; j<4*4; j+=4) {
        d[0] = d[0]+ F(d[1], d[2], d[3])+ m[j] + T[j];                          d[0]=rotate(d[0], 7);
        d[0]+= d[1];
        d[3] = d[3]+ F(d[0], d[1], d[2])+ m[(j)+1] + T[j+1];                    d[3]=rotate(d[3], 12);
        d[3]+= d[0];
        d[2] = d[2]+ F(d[3], d[0], d[1])+ m[(j)+2] + T[j+2];                    d[2]=rotate(d[2], 17);
        d[2]+= d[3];
        d[1] = d[1]+ F(d[2], d[3], d[0])+ m[(j)+3] + T[j+3];                    d[1]=rotate(d[1], 22);
        d[1]+= d[2];
    }
    /*MD5 STEP2 */
    for (j=0; j<4*4; j+=4) {
        d[0] = d[0]+ G(d[1], d[2], d[3])+ m[(5*j+1)&0x0f] + T[(j-1)+17];        d[0] = rotate(d[0],5);
        d[0]+= d[1];
        d[3] = d[3]+ G(d[0], d[1], d[2])+ m[((5*(j+1)+1)&0x0f)] + T[(j+0)+17];  d[3] = rotate(d[3], 9);
        d[3]+= d[0];
        d[2] = d[2]+ G(d[3], d[0], d[1])+ m[((5*(j+2)+1)&0x0f)] + T[(j+1)+17];  d[2] = rotate(d[2], 14);
        d[2]+= d[3];
        d[1] = d[1]+ G(d[2], d[3], d[0])+ m[((5*(j+3)+1)&0x0f)] + T[(j+2)+17];  d[1] = rotate(d[1], 20);
        d[1]+= d[2];
    }
    /*MD5 STEP3 */
    for (j=0; j<4*4; j+=4) {
        d[0] = d[0]+ H(d[1], d[2], d[3])+ m[(3*j+5)&0x0f] + T[(j-1)+33];        d[0] = rotate(d[0], 4);
        d[0]+= d[1];
        d[3] = d[3]+ H(d[0], d[1], d[2])+ m[(3*(j+1)+5)&0x0f] + T[(j+0)+33];    d[3] = rotate(d[3], 11);
        d[3]+= d[0];
        d[2] = d[2]+ H(d[3], d[0], d[1])+ m[(3*(j+2)+5)&0x0f] + T[(j+1)+33];    d[2] = rotate(d[2], 16);
        d[2]+= d[3];
        d[1] = d[1]+ H(d[2], d[3], d[0])+ m[(3*(j+3)+5)&0x0f] + T[(j+2)+33];    d[1] = rotate(d[1], 23);
        d[1]+= d[2];
    }
    /*MD5 STEP4 */
    for (j=0; j<4*4; j+=4) {
        d[0] = d[0]+ I(d[1], d[2], d[3])+ m[(7*j)&0x0f] + T[(j-1)+49];          d[0] = rotate(d[0], 6);
        d[0]+= d[1];
        d[3] = d[3]+ I(d[0], d[1], d[2])+ m[(7*(j+1))&0x0f] + T[(j+0)+49];      d[3] = rotate(d[3], 10);
        d[3]+= d[0];
        d[2] = d[2]+ I(d[3], d[0], d[1])+ m[(7*(j+2))&0x0f] + T[(j+1)+49];      d[2] = rotate(d[2], 15);
        d[2]+= d[3];
        d[1] = d[1]+ I(d[2], d[3], d[0])+ m[(7*(j+3))&0x0f] + T[(j+2)+49];      d[1] = rotate(d[1], 21);
        d[1]+= d[2];
    }
}


static inline void _bytestoword32 (UWORD32 *x, const char *pt) {
    int processed_bytes, j;
    for (processed_bytes=0; processed_bytes<16; processed_bytes++) {
        j=processed_bytes*4;
        x[processed_bytes] = (((UWORD32)(unsigned char)pt[j+3]  << 8 |
                 (UWORD32)(unsigned char)pt[j+2]) << 8 |
                 (UWORD32)(unsigned char)pt[j+1]) << 8 |
                 (UWORD32)(unsigned char)pt[j];
    }

}


static inline void _put_length(UWORD32 *hash, long len) {
    /* in bits! */
    hash[14] = (UWORD32)((len<<3)     & MASK);
    hash[15] = (UWORD32)( len>>(32-3) & 0x7 );
}



/*/// ===============================================================================================================================
                                                SECONDARY FUNCTIONS 
/*/// ===============================================================================================================================
///// *it is better to read public functions first) 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_DATA_IN_CHUNK_TO_BE_ADD_UNIQBYTE_AND_PUT_LEN_IN_HASH   (CHUNK_SIZE - 9)
#define LENGTH_WILL_FIT(buffer) ((buffer->size - MAX_DATA_IN_CHUNK_TO_BE_ADD_UNIQBYTE_AND_PUT_LEN_IN_HASH) <= 0)
#define LENGTH_WILL_NOT_FIT(buffer) ((buffer->size - MAX_DATA_IN_CHUNK_TO_BE_ADD_UNIQBYTE_AND_PUT_LEN_IN_HASH) > 0)

#define COPY_DIGEST(src, dest)  \
    dest[0]=src[0];             \
    dest[1]=src[1];             \
    dest[2]=src[2];             \
    dest[3]=src[3];


#define COMBINE_DIGEST(curr, old)   \
    curr[0]+=old[0];                \
    curr[1]+=old[1];                \
    curr[2]+=old[2];                \
    curr[3]+=old[3];


#define PRINT_DIGEST(d) \
    printf("\\%u\\%u\\%u\\%u", d[0], d[1], d[2], d[3]);

#define PRINT_HASH(h) \
    printf("\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u\\%u", h[0], h[1], h[2], h[3], h[4], h[5], h[6], h[7], h[8], h[9], h[10], h[11], h[12], h[13], h[14], h[15]);



#define _add_byte(buffer) (buffer->array[buffer->size] = UNIQUENESS_BYTE)
#define _add_byte2(buffer) (buffer.array[buffer.size] = UNIQUENESS_BYTE)

#define _update_hash_from_msg(hash, msg) _bytestoword32(hash, msg);
#define _update_hash_from_buffer(hash, buffer) _bytestoword32(hash, buffer->array);
#define _update_hash_from_buffer2(hash, buffer) _bytestoword32(hash, buffer.array);


static inline void _update_hash_from_empty(UWORD32 *hash){
    char empty_chunk[64] = {0};             
    _bytestoword32(hash, empty_chunk);
}


static inline void _fill_buffer(TBuffer *buffer, const char *msg, int bytes_to_read) {
    printf("1. buffer->array=%p; buffer->size=%d; bytes_to_read=%d; msg='%s'\n", buffer->array, buffer->size, bytes_to_read, msg);
    printf("2. [buffer->array + buffer->size] = %p\n", (buffer->array + buffer->size));
    printf("3. (before) buffer->array = '%s'\n", buffer->array);
    
    memcpy(buffer->array + buffer->size, msg, bytes_to_read);
    
    printf("4. (after)  buffer->array = '%s'\n", buffer->array);

    buffer->size += bytes_to_read;

    printf("5. buffer->size = %d; CHUNK_SIZE=%d\n", buffer->size, CHUNK_SIZE);
    printf("6. [buffer->array + buffer->size] = %p\n", (buffer->array + buffer->size));
    
    memset(buffer->array + buffer->size, 0, CHUNK_SIZE - buffer->size);

    printf("7. (final)  buffer->array = '%s'\n", buffer->array);
    printf("---------------------------------------------------------------------------------------------------------------\n");
}

static inline void _clear_buffer(TBuffer *buffer){
    memset(buffer->array, 0, CHUNK_SIZE);
    buffer->size = 0;
}


static inline void _update_digest(UWORD32 *hash, UWORD32 *digest, UWORD32 *prev_digest){
    COPY_DIGEST(digest, prev_digest);
    _calculate_digest(hash, digest);
    COMBINE_DIGEST(digest, prev_digest);
}


static inline void _finalize_1__with_overflow(TBuffer *buffer, UWORD32 *hash, UWORD32 *digest, UWORD32 *prev_digest, size_t final_len){
    _add_byte(buffer);                                                             

    printf("buf->array (filed) = %.64s\n", buffer->array);

    _update_hash_from_buffer(hash, buffer);                                        

    printf("x2: 1 hash=");
    PRINT_HASH(hash);
    printf(";\n");

    _update_digest(hash, digest, prev_digest);                                     

    printf("x2: 1 digest=");
    PRINT_DIGEST(digest);
    printf("; prev_digest=");
    PRINT_DIGEST(prev_digest);
    printf(";\n");

    _update_hash_from_empty(hash);                                                 
    _put_length(hash, final_len);         

    printf("x2: 2 hash=");
    PRINT_HASH(hash);
    printf(";\n");

    _update_digest(hash, digest, prev_digest);

    printf("x2: 2 digest=");
    PRINT_DIGEST(digest);
    printf("; prev_digest=");
    PRINT_DIGEST(prev_digest);
    printf(";\n");
}

static inline void _finalize_2__inplace(TBuffer *buffer, UWORD32 *hash, UWORD32 *digest, UWORD32 *prev_digest, size_t final_len){
    _add_byte(buffer);                                 
    
    printf("buf->array (filed) = %.64s\n", buffer->array);

    _update_hash_from_buffer(hash, buffer);            
    _put_length(hash, final_len);                                            

    printf("x3: hash=");
    PRINT_HASH(hash);
    printf(";\n");

    _update_digest(hash, digest, prev_digest);

    printf("x3: digest=");
    PRINT_DIGEST(digest);
    printf("; prev_digest=");
    PRINT_DIGEST(prev_digest);
    printf(";\n");
}



static inline void _finalize (TBuffer *buf, UWORD32 *hash, const char *msg, int bytes_to_read, UWORD32 *d, UWORD32 *prev_d, size_t len) {
    _fill_buffer(buf, msg, bytes_to_read);

    if (LENGTH_WILL_NOT_FIT(buf))
        _finalize_1__with_overflow(buf, hash, d, prev_d, len);
    else                         
        _finalize_2__inplace(buf, hash, d, prev_d, len);
}


static inline TCalculationState _try_to_finalize (TBuffer *buf, UWORD32 *hash, const char *msg, int bytes_to_read, UWORD32 *d, UWORD32 *prev_d, size_t len) {
    printf("msg (shifted) = \"%s\" (%lld)\n", msg, strlen(msg));
    printf("- - - - - - - - - - - - - - - - - - - - -\n");
    printf("buf.size=%d; final_len=%llu; bytes_to_read=%d;\ndigest=", buf->size, len, bytes_to_read);
    PRINT_DIGEST(d);
    printf("; prev_digest=");
    PRINT_DIGEST(prev_d);
    printf(";\nhash=");
    PRINT_HASH(hash);
    printf(";\n");

    _fill_buffer(buf, msg, bytes_to_read);

    if (LENGTH_WILL_FIT(buf)){
        _finalize_2__inplace(buf, hash, d, prev_d, len);
                return STATE_COMPLETED;
    }

    if (buf->size < CHUNK_SIZE){
        _finalize_1__with_overflow(buf, hash, d, prev_d, len);
        return STATE_COMPLETED;
    }

    printf("buf->array (filed) = %.64s\n", buf->array);

    _update_hash_from_buffer(hash, buf);
            
    printf("x1: hash=");
    PRINT_HASH(hash);
    printf(";\n");

    _update_digest(hash, d, prev_d);
            
    printf("x1: digest=");
    PRINT_DIGEST(d);
    printf("; prev_digest=");
    PRINT_DIGEST(prev_d);
    printf(";\n");
            
    _clear_buffer(buf);

    return STATE_NEW_COMPLETED_DIGEST;
}


static inline void _read_full_chunk(UWORD32 *hash, const char *msg_ptr, UWORD32 *digest, UWORD32 *prev_digest){
    _update_hash_from_msg(hash, msg_ptr);
    _update_digest(hash, digest, prev_digest);
}



/*/// ===============================================================================================================================
                                                        PUBLIC FUNCTIONS
/*/// ===============================================================================================================================



    
void md5_calculate (const char *msg, size_t len, char output[HASH_SIZE]) {
    printf("\n\n\n\n");

    UWORD32 digest[4], prev_digest[4], hash[HASH_SIZE]; _init_digest(digest);
    size_t processed_bytes = 0;
    int bytes_to_read;

    printf("msg=%s\n", msg);

    bytes_to_read = MIN(len, CHUNK_SIZE);
    while (bytes_to_read == CHUNK_SIZE) {
        printf("-------------------------------------------------------------\n");
        _read_full_chunk(hash, msg + processed_bytes, digest, prev_digest);
        
        processed_bytes += bytes_to_read;
        bytes_to_read = MIN(len - processed_bytes, CHUNK_SIZE);
    }

    printf("-------------------------------------------------------------\n");

    TBuffer buffer, *buffer_ptr = &buffer; buffer.size = 0;
    _finalize(buffer_ptr, hash, msg + processed_bytes, bytes_to_read, digest, prev_digest, len);

    _word32tobytes(digest, output);
}



///  Iterative calculations with context



void md5_ctx_init(md5_ctx *m) {
    _init_digest(m->current_digest);
    _init_digest(m->completed_digest);
    
    m->len = 0;
    memset(m->buffer.array, 0, 64);
    m->buffer.size = 0;

    // set default digest (just in case update won't be called)
    UWORD32 dummy_hash[HASH_SIZE] = {0};

    _add_byte2(m->buffer);
    _update_hash_from_buffer2(dummy_hash, m->buffer);
    _put_length(dummy_hash, 0); 
    _calculate_digest(dummy_hash, m->current_digest);
    
    COMBINE_DIGEST(m->current_digest, m->completed_digest);
}


void md5_ctx_finish (md5_ctx *m, char output[HASH_SIZE]) {
    _word32tobytes(m->current_digest, output);
}



void md5_ctx_update(md5_ctx *m, const char *msg, size_t len) {
    printf("=============================================================\n");

    TCalculationState current_state = STATE_NOT_READY;

    UWORD32 digest[4], prev_digest[4], hash[HASH_SIZE];
    COPY_DIGEST(m->completed_digest, digest);  

    int bytes_to_read = MIN(CHUNK_SIZE - m->buffer.size, len);
    size_t processed_bytes = 0, final_len = m->len + len;
    TBuffer *buffer_ptr = &m->buffer;

    printf("message=%s; len=%lld\n", msg, len);

    while (current_state != STATE_COMPLETED) {
        printf("-------------------------------------------------------------\n");

        if (bytes_to_read == CHUNK_SIZE) {
            _read_full_chunk(hash, msg + processed_bytes, digest, prev_digest);
            COPY_DIGEST(digest, m->completed_digest);
        } else {
            current_state = _try_to_finalize(buffer_ptr, hash, msg + processed_bytes, bytes_to_read, digest, prev_digest, final_len);
            if (current_state == STATE_NEW_COMPLETED_DIGEST) {
                printf("completedITER\n");

                COPY_DIGEST(digest, m->completed_digest);
            }
        }
        printf("  after (old): processed_bytes=%lld; bytes_to_read=%d\n", processed_bytes, bytes_to_read);

        processed_bytes += bytes_to_read;
        bytes_to_read = MIN(CHUNK_SIZE, len - processed_bytes);

        printf("  after (new): processed_bytes=%lld; bytes_to_read=%d\n", processed_bytes, bytes_to_read);
    }

    COPY_DIGEST(digest, m->current_digest);
    m->len = final_len;
}
