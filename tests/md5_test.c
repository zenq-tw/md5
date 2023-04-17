#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "md5.h"


#define PLUS  '+'
#define MINUS '-'
#define GET_MARKER(is_succeed) (is_succeed)? PLUS : MINUS

#define MAX_MSG_OUTPUT_SIZE 150
#define END (char*)NULL


bool check(const char expected[33], char *buff, char *checksum) {
    for (int i=0; i<16; i++) sprintf(checksum+i*2, "%02x", (unsigned char)buff[i]);
    checksum[32] = '\0';

    return strcmp(checksum, expected) == 0;
}


bool test(const char *name, const char expected[33], md5_ctx *t, char *buff, char *checksum, ...){
    printf("test: %-20s", name);

    //   SETUP

    char details[1000];
    int dlen = 0;

    va_list args_ptr;
    va_start(args_ptr, checksum);

    //   CLEANUP

    memset(buff, 0, 16);
    memset(checksum, 0, 33);

    //   ITERATIVE CHECKSUM CALCULATION
    //   -- md5 context init --

    md5_ctx_init(t);
    
    size_t token_len, msg_len = 0;
    char *msg = malloc(0);
    char *token;

    //   -- md5 context update(s) --

    while ((token = va_arg(args_ptr, char*))) {
        token_len = strlen(token);
        md5_ctx_update(t, token, token_len);

        msg = realloc(msg, msg_len + token_len); memcpy(msg + msg_len, token, token_len); msg_len += token_len;
    }

    // Complete test message parsing & construct details

    msg = realloc(msg, msg_len + 1); msg[msg_len] = 0;
    if (msg_len > MAX_MSG_OUTPUT_SIZE) dlen = sprintf(details, "    msg=\"%.*s\"... <truncated (len>%d)>;\n",    MAX_MSG_OUTPUT_SIZE, msg, MAX_MSG_OUTPUT_SIZE);
    else                               dlen = sprintf(details, "    msg=\"%s\";\n",                              msg);
    dlen += sprintf(details + dlen, "    expected    = \"%s\"\n", expected);


    //   -- md5 context calculate checksum (finish/finalize) --

    md5_ctx_finish(t, buff);

    bool iterative_ok = check(expected, buff, checksum);
    dlen += sprintf(details + dlen, "      iterative = \"%s\" [%c]\n", checksum, GET_MARKER(iterative_ok));


    //   CLEANUP

    va_end(args_ptr);
    
    memset(buff, 0, 16);
    memset(checksum, 0, 33);


    //   DIRECT CHECKSUM CALCULATION

    md5_calculate(msg, msg_len, buff);
    
    bool direct_ok = check(expected, buff, checksum);
    dlen += sprintf(details + dlen, "      direct  = \"%s\" [%c]\n", checksum, GET_MARKER(direct_ok));
    

    //   CLEANUP

    free(msg);


    //   Print Results

    if (iterative_ok && direct_ok) {
        printf("[+]\n");
        return true;
    }

    printf("[-]\n%s--------------------------------------------------------\n", details);
    return false;
}


int main(void){
    struct md5_ctx t, *p = &t;
    char b[16], r[33];  // output buffer      , hex checksum (result)
    int s = 0, a = 0;   // succeed tests count, all tests count;

    a++; s+=test("empty (no update)",   "d41d8cd98f00b204e9800998ecf8427e", p, b, r,     END);
    a++; s+=test("empty string",        "d41d8cd98f00b204e9800998ecf8427e", p, b, r, "", END);

    a++; s+=test("1B",                  "7fc56270e7a70fa81a5935b72eacbe29", p, b, r, "A",             END);
    a++; s+=test("Hello",               "8b1a9953c4611296a827abf8c47804d7", p, b, r, "Hello",         END);
    a++; s+=test("Hello (splited)",     "8b1a9953c4611296a827abf8c47804d7", p, b, r, "He", "ll", "o", END);

    a++; s+=test("55B (data+len)",      "68e5d43dd4df8865ae4c3821a3d5d67c", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "4___4",               END);
    a++; s+=test("60B",                 "88b105463d36cc995bc2ce87ddf5a4b9", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........",          END);
    a++; s+=test("63B",                 "54f0a2e902f3a6370893d39d78300841", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "ABC",   END);
    a++; s+=test("64B ",                "b80a321ad75c472f0423ddfff70d104c", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "4!@4",  END);
    a++; s+=test("65B",                 "ca5335dd60eaa119e692be7f7cb953cd", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "5$_^5", END);

    a++; s+=test("119B (ch+data+len)",  "63bab0a5f519b1b5c2220b04ab66d249", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "7.........", "8.........", "9.........", "10........", "11........", "999___999",                  END);
    a++; s+=test("127B",                "718ca9f273a5be6766512d111de62708", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "4444", "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "333",  END);
    a++; s+=test("128B (2 chunks)",     "646c13efcfc71a695a759be6569efa5e", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "4444", "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "4444", END);
    a++; s+=test("130B",                "54ba6ed3dd7d7d3c48e728480259dd06", p, b, r, "1.........", "2.........", "3.........", "4.........", "5.........", "6.........", "7.........", "8.........", "9.........", "10........", "11........", "12........", "13_abcdefg",   END);
    

    a++; s+=test("1KB (at once)",       "c34afd95f646f2445e5b5ab90e151024", p, b, r,
        "Curabitur viverra faucibus magna. Sed bibendum metus vitae nisl fringilla, vitae interdum velit consequat. Nullam id orci euismod, commodo nibh a, condimentum velit. Nam bibendum, metus nec tempus tincidunt, augue lectus aliquam orci, sed sollicitudin nibh urna eget massa. Proin eu luctus felis. Duis posuere ex at purus rutrum mollis. Duis vitae vulputate erat. Sed vel tempor nisi. In posuere elit ac viverra consectetur. Suspendisse nec nunc ac felis bibendum mattis at sed sapien. Mauris libero neque, pellentesque non ipsum eu, iaculis euismod quam. Etiam elementum, sem sit amet convallis consectetur, nulla nisi elementum risus, porta mattis lectus sem imperdiet felis. Praesent faucibus varius urna a euismod. Pellentesque semper, nisl nec euismod ullamcorper, lacus mi dictum purus, quis consequat ipsum magna a eros. Nullam eget convallis nibh. Vivamus commodo laoreet quam nec rutrum. Aenean orci massa,lacinia id nisi ut, ornare tempus tortor. Pellentesque commodo porta elit, sit amet fringilla nunc efficitur.",
        END
    );
    a++; s+=test("1KB (splitted)",      "c34afd95f646f2445e5b5ab90e151024", p, b, r,
        "Curabitur viverra faucibus magna. Sed bibendum metus vitae nisl fringilla, vitae interdum velit consequat. ",
        "Nullam id orci euismod, commodo nibh a, condimentum velit. Nam bibendum, metus nec tempus tincidunt, augue lectus aliquam orci, sed ",
        "sollicitudin nibh urna eget massa. Proin eu luctus felis. Duis posuere ex at purus rutrum mollis. Duis vitae vulputate erat. Sed vel tempor nisi. In posuere elit ac viverra consectetur. ",
        "Suspendisse nec nunc ac felis bibendum mattis at sed sapien. Mauris libero neque, pellentesque non ipsum eu, iaculis euismod quam. Etiam elementum, sem sit amet convallis consectetur, nulla nisi elementum risus, porta mattis lectus sem imperdiet felis. ",
        "Praesent faucibus varius urna a euismod. Pellentesque semper, nisl nec euismod ullamcorper, lacus mi dictum purus, quis consequat ipsum magna a eros. Nullam eget convallis nibh. Vivamus commodo laoreet quam nec rutrum. Aenean orci massa,lacinia id nisi ut, ornare tempus tortor. Pellentesque commodo porta elit, sit amet fringilla nunc ",
        "efficitur.",
        END
    );



    printf("\nSummary: %d/%d (%d%%)", s, a, 100*s/a);
    if (s != a) printf(" [failed %d]", a-s);
    printf("\n\n");

    return 0;
}