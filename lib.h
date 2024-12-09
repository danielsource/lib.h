/* lib.h: a C99 header-only library for fun.
Usage:
    ...
    #define LIB_H_IMPL
    #include "lib.h"
    ...

Version: 2024-12-13
Repository: https://github.com/danielsource/lib.h

LICENSE AT THE END OF THE FILE */

#ifndef LIB_H_
#define LIB_H_

#include <stddef.h>

#if !(defined(LIB_H_OS_WINDOWS) || defined(LIB_H_OS_UNIXLIKE) || defined(LIB_H_OS_UNKNOWN))
#   if defined(_WIN32) || defined(_WIN64)
#      define LIB_H_OS_WINDOWS
#   elif defined(__linux__) || defined(__unix__) || defined(__unix) ||\
        (defined(__APPLE__) && defined(__MACH__))
#      define LIB_H_OS_UNIXLIKE
#   else
#      pragma message("Unknown platform")
#      define LIB_H_OS_UNKNOWN
#   endif
#endif

#if defined(LIB_H_OS_WINDOWS)
#   define LIB_H_OSNAME "win"
#elif defined(LIB_H_OS_UNIXLIKE)
#   define LIB_H_OSNAME "unix"
#else
#   define LIB_H_OSNAME "unknown"
#endif

#define COMPILE_ASSERT(cond) extern char (*LIB_H_COMPILE_ASSERT_(void)) [sizeof(char[1 - 2*!(cond)])]

#ifndef l_int
#   define l_int long
#endif
COMPILE_ASSERT(sizeof(l_int) >= 4);

#if !defined(__cplusplus) && !defined(bool)
#   define bool _Bool
#   define false (0)
#   define true  (!false)
#endif

#if defined(int8_t)
    typedef int8_t   i8;
    typedef int16_t  i16;
    typedef int32_t  i32;
    typedef int64_t  i64;
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef float    f32;
    typedef double   f64;
#endif

typedef long long          llong;
typedef unsigned char      byte;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))
#define UNUSED(x)   ((void)(x))
#define EQ3(x, y, z)    ((x) == (y) && (y) == (z))
#define EQ4(x, y, z, w) ((x) == (y) && (y) == (z) && (z) == (w))

#define STRINGIFY(macro) STRINGIFY_(macro)
#define STRINGIFY_(macro) #macro
#define AT __FILE__ ":" STRINGIFY(__LINE__)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define BETWEEN(x, a, b) ((x) >= (a) && (x) <= (b))

#ifndef L_BUFSIZE
#   define L_BUFSIZE 512
#endif
COMPILE_ASSERT(sizeof(L_BUFSIZE) >= 2);

#ifndef EXIT_SUCCESS
#   define EXIT_SUCCESS 0
#   define EXIT_FAILURE 1
#endif

#ifdef TEST
/* TODO: Minimal unit testing.
 * Source (modified): https://jera.com/techinfo/jtns/jtn002 */
typedef const char *testmsg;
#define test_assert(msg, cond) do{ if (!(cond)) return AT ": " msg; }while(0)
#define test_run(test_func)    do{ testmsg msg = test_func(); tests_run++;\
                                   if (msg) return msg; }while(0)
extern int tests_run;
#endif /* TEST END */

/* TODO: Assertions should be more debugger-oriented.
 * Source (modified): https://nullprogram.com/blog/2022/06/26/ */
#ifdef DEBUG
#   if __GNUC__
#       define dbg_assert(c) do{ if (!(c)) __builtin_trap(); }while(0)
#   elif _MSC_VER
#       define dbg_assert(c) do{ if (!(c)) __debugbreak(); }while(0)
#   else
#       define dbg_assert(c) do{ if (!(c)) *(volatile int *)0 = 0; }while(0)
#   endif
#else
#   define dbg_assert(c)
#endif

#define IS_ASCII(c) (((c) & ~127) == 0)
#define IS_CNTRL(c) (IS_ASCII(c) && ((c) <= 31 || (c) == 127))
#define IS_BLANK(c) ((c) == ' ' || (c) == '\t')
#define IS_SPACE(c) (IS_BLANK(c) || (c) == '\f' || (c) == '\n' || (c) == '\r' || (c) == '\v')
#define IS_PRINT(c) (IS_ASCII(c) && !IS_CNTRL(c))
#define IS_GRAPH(c) (IS_PRINT(c) && (c) != ' ')
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define IS_ALNUM(c) (IS_ALPHA(c) || IS_DIGIT(c))
#define IS_PUNCT(c) (IS_GRAPH(c) && !IS_ALNUM(c))
#define IS_XDIGT(c) (IS_DIGIT(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

#define TO_ASCII(c) ((c) & 127)
#define TO_UPPER(c) (IS_LOWER(c) ? (c) - 32 : (c))
#define TO_LOWER(c) (IS_UPPER(c) ? (c) + 32 : (c))

#if defined(__cplusplus)
extern "C" {
#endif

size_t cstr_len     (const char *s);
size_t cstr_len_max (const char s[/*max*/], size_t max);
int    cstr_comp    (const char *a, const char *b);
int    cstr_comp_max(const char a[/*max*/], const char b[/*max*/], size_t max);
char  *cstr_copy    (char *dst, const char *src);
char  *cstr_copy_max(char *dst, const char src[/*max*/], size_t max);
byte  *copy         (byte dst[/*len*/], const byte src[/*len*/], size_t len);
byte  *reverse      (byte data[/*len*/], size_t len);
l_int  cstr_to_int  (const char *s, char **end);
int    int_to_cstr  (l_int n, char s[/*max*/], size_t max);
int    print_cstr   (bool err, bool newl, const char *s);
int    print_int    (bool err, bool newl, l_int n);
int    input        (const char *prompt, char dst[/*max*/], size_t max);
int    int_digits   (l_int n);
ulong  sqrt_ul      (ulong x);
void   exit_process (int status);
void   die          (const char *msg);

int histogram_charfreq(const char *text);           /*TODO*/
int histogram_wordlen(const char *text);            /*TODO*/
int rpn_calc        (const char *expr, char **end); /*TODO*/

#ifdef ARENA

#ifndef L_CALLOC
#   define L_CALLOC(n, size) calloc(n, size)
#endif
#ifndef L_FREE
#   define L_FREE(ptr) free(ptr)
#endif

typedef struct ArenaNode {
    void *data;
    size_t size;
    struct ArenaNode *next;
} ArenaNode;

typedef struct Arena {
    ArenaNode *elems;
} Arena;

void *ecalloc(size_t elems, size_t size);
Arena ArenaNew(size_t elems);
void  ArenaClear(Arena *ar);
void *ArenaAlloc(Arena ar, size_t size);
#endif /* ARENA END */

#if defined(__cplusplus)
}
#endif

#define print(s)      print_cstr(false, false, s)
#define print_ln(s)   print_cstr(false, true, s)
#define print_i(n)    print_int(false, false, n)
#define print_i_ln(n) print_int(false, true, n)

#define err_print(s)      print_cstr(true, false, s)
#define err_print_ln(s)   print_cstr(true, true, s)
#define err_print_i(n)    print_int(true, false, n)
#define err_print_i_ln(n) print_int(true, true, n)

#ifdef DEBUG
#define dbg_print(s)      print_cstr(true, false, s)
#define dbg_print_ln(s)   print_cstr(true, true, s)
#define dbg_print_i(n)    print_int(true, false, n)
#define dbg_print_i_ln(n) print_int(true, true, n)
#define DBG_LOG(msg) do{ print_cstr(true, false, AT ":");\
                         print_cstr(true, false, __func__);\
                         print_cstr(true, false, ": ");\
                         print_cstr(true, false, msg); }while(0)
#define DBG_LOG_END      print_cstr(true, false, "\n")
#else
#define dbg_print(s)
#define dbg_print_ln(s)
#define dbg_print_i(n)
#define dbg_print_i_ln(n)
#define DBG_LOG(msg)
#define DBG_LOG_END
#endif

#define POWER10_LEN 19
extern const llong power10[POWER10_LEN];

#ifdef LIB_H_IMPL

#if defined(LIB_H_OS_WINDOWS)
#   include <windows.h>
#elif defined(LIB_H_OS_UNIXLIKE)
#   include <unistd.h>
#else
#   include <stdio.h>
#endif

#if defined(LIB_H_OS_UNKNOWN) || defined(ARENA)
#   include <stdlib.h>
#endif

/* 10^0 to 10^18. */
const llong power10[POWER10_LEN] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000,
    1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000
};

static char l_buf_[L_BUFSIZE];

size_t cstr_len(const char *s)
{
    size_t len = 0;
    dbg_assert(s);
    while (*(s++)) ++len;
    return len;
}

size_t cstr_len_max(const char s[/*max*/], size_t max)
{
    size_t len = 0;
    dbg_assert(s);
    while (max-- && *(s++)) ++len;
    return len;
}

/* Returns:
 * -1, if a < b
 *  1, if a > b
 *  0, otherwise */
int cstr_comp(const char *a, const char *b)
{
    dbg_assert(a);
    dbg_assert(b);
    while (*a && *b) {
        ++a;
        ++b;
    }
    if (!*a && *b) return -1;
    if (*a && !*b) return 1;
    return 0;
}

/* Returns:
 * -1, if a < b
 *  1, if a > b
 *  0, otherwise */
int cstr_comp_max(const char a[/*max*/], const char b[/*max*/], size_t max)
{
    dbg_assert(a);
    dbg_assert(b);
    if (max == 0) return 0;
    while (max-- && *a && *b) {
        ++a;
        ++b;
    }
    if (!*a && *b) return -1;
    if (*a && !*b) return 1;
    return 0;
}

/* Returns a pointer to the null byte of destination */
char *cstr_copy(char *dst, const char *src)
{
    dbg_assert(dst);
    dbg_assert(src);
    while ((*dst = *(src++))) ++dst;
    return dst;
}

/* Returns a pointer to the null byte of destination */
char *cstr_copy_max(char *dst, const char src[/*max*/], size_t max)
{
    dbg_assert(dst);
    dbg_assert(src);
    while (max-- && (*(dst++) = *(src++)));
    return dst;
}

byte *copy(byte dst[/*len*/], const byte src[/*len*/], size_t len)
{
    dbg_assert(dst);
    dbg_assert(src);
    for (size_t i = 0; i < len; ++i) dst[i] = src[i];
    return dst;
}

byte *reverse(byte data[/*len*/], size_t len)
{
    byte tmp;
    dbg_assert(data);
    for (size_t i = 0; i < len/2; ++i) {
        tmp = data[i];
        data[i] = data[len-1 - i];
        data[len-1 - i] = tmp;
    }
    return data;
}

l_int cstr_to_int(const char *s, char **end)
{
    l_int n = 0, sign = 1, len = 10;

    dbg_assert(s);

    while (IS_SPACE(*s)) ++s;

    if (*s == '+') {
        ++s;
    } else if (*s == '-') {
        sign = -1;
        ++s;
    }

    while (IS_DIGIT(*s) && len > 0) {
        /* XXX: This does not handle overflow. */
        n = n*10 + (*s - '0');
        ++s;
        --len;
    }

    if (end) *end = (char *)s;
    return n * sign;
}

/* Returns non-negative on success. */
int int_to_cstr(l_int n, char s[/*max*/], size_t max)
{
    if (max <= 1) return -1;

    dbg_assert(s);

    if (n < 0) {
        n *= -1;
        *s = '-';
        ++s;
        --max;
    }

    if (max == 1) {
        *s = '\0';
        return -1;
    }

    int digits = int_digits(n);

    if (digits <= 0) {
        *s = '\0';
        return -1;
    }

    *s = (char)(n / power10[digits-1]) + '0';
    ++s;
    --max;
    --digits;

    while (max > 1 && digits > 0) {
        *s = (char)((n / power10[digits-1]) % 10) + '0';
        ++s;
        --max;
        --digits;
    }

    *s = '\0';
    return digits;
}

/* Returns non-negative on success. */
int print_cstr(bool err, bool newl, const char *s)
{
    int ret = 0;

    dbg_assert(s);
    const size_t len = cstr_len(s);
    dbg_assert(len <= (1 << 15) - 2);

#if defined(LIB_H_OS_WINDOWS)
    HANDLE out = GetStdHandle((err) ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
    if (!out || out == INVALID_HANDLE_VALUE ||
            !WriteConsoleA(out, s, len, &ret, NULL))
        return -1;
#elif defined(LIB_H_OS_UNIXLIKE)
    ret = (int)write((err) ? STDERR_FILENO : STDOUT_FILENO, s, len);
    if (ret < 0) return -1;
#else
    if (fputs(s, (err) ? stderr : stdout) >= 0)
        ret = len;
    else
        ret = -1;
#endif

    if (newl) {
        if (print_cstr(err, false, "\n") != 1)
            return -1;
        ++ret;
    }

    return ret;
}

/* Returns non-negative on success. */
int print_int(bool err, bool newl, l_int n)
{
    int ret;
    char s[12];

    ret = int_to_cstr(n, s, sizeof(s));
    dbg_assert(ret >= 0);

    return print_cstr(err, newl, s);
}

/* TODO: read more than L_BUFSIZE-1 */
/* Returns non-negative on success. */
int input(const char *prompt, char dst[/*max*/], size_t max)
{
    int ret = 0;

    if (max <= 1) return -1;

    dbg_assert(dst);
    dbg_assert(max <= L_BUFSIZE);

    if (prompt) print(prompt);

#if defined(LIB_H_OS_WINDOWS)
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (!in || in == INVALID_HANDLE_VALUE ||
            !ReadFile(in, l_buf_, L_BUFSIZE-1, &ret, NULL)) {
        print("\n");
        return -1;
    }
    cstr_copy_max(dst, l_buf_, max-1);
    if (ret > (int)max-1) ret = (int)max-1;
#elif defined(LIB_H_OS_UNIXLIKE)
    ret = (int)read(STDIN_FILENO, l_buf_, L_BUFSIZE-1);
    if (ret <= 0) {
        print("\n");
        return -1;
    }
    cstr_copy_max(dst, l_buf_, max-1);
    if (ret > (int)max-1) ret = (int)max-1;
#else
    if (!fgets(dst, max, stdin)) {
        print("\n");
        return -1;
    }
    ret = cstr_len(dst);
#endif
    dst[ret] = '\0';

    if (dst[ret-1] == '\n') {
        dst[ret-1] = '\0';
        --ret;
    } else if (ret < (int)max-1) {
        print("\n");
    }

    return ret;
}

int int_digits(l_int n)
{
    static const l_int sizes[] = {
        9, 99, 999, 9999, 99999, 999999, 9999999,
        99999999, 999999999, (1l << 31) - 1
    };

    if (n < 0) n *= -1;

    for (int i = 0; i < LENGTH(sizes); ++i)
        if (n <= sizes[i])
            return i + 1;
    return 0;
}

ulong sqrt_ul(ulong x)
{
    if (x < 2) return x;
    ulong a = x, b = 1;
    a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    b = x/a; a = (a+b)/2;
    return a;
}

void exit_process(int status)
{
#ifdef L_AT_EXIT
    L_AT_EXIT;
#endif

#if defined(LIB_H_OS_WINDOWS)
    ExitProcess(status);
#elif defined(LIB_H_OS_UNIXLIKE)
    _exit(status);
#else
    exit(status);
#endif
}

void die(const char *msg)
{
    if (msg) err_print_ln(msg);
    exit_process(EXIT_FAILURE);
}

#ifdef ARENA

void *ecalloc(size_t elems, size_t size)
{
	void *p;
	if (!(p = L_CALLOC(elems, size))) die("lib.h: calloc fail");
	return p;
}

Arena ArenaNew(size_t elems)
{
    Arena ar = {0};
    if (elems == 0) return ar;
    ar.elems = (ArenaNode *)ecalloc(1, sizeof(ArenaNode));
    for (ArenaNode *e = ar.elems; --elems; e = e->next)
        e->next = (ArenaNode *)ecalloc(1, sizeof(ArenaNode));
    return ar;
}

void ArenaClear(Arena *ar)
{
    ArenaNode *next;
    if (!ar || !ar->elems) return;
    for (ArenaNode *e = ar->elems; e; e = next) {
        next = e->next;
        L_FREE(e->data);
        L_FREE(e);
    }
    ar->elems = NULL;
}

void *ArenaAlloc(Arena ar, size_t size)
{
    if (size == 0) return NULL;

    if (!ar.elems) {
        ar.elems = (ArenaNode *)ecalloc(1, sizeof(ArenaNode));
        ar.elems->data = ecalloc(1, size);
        ar.elems->size = size;
        return ar.elems->data;
    }

    ArenaNode *prev;
    for (ArenaNode *e = ar.elems; e; e = e->next) {
        if (e->size == 0) {
            e->data = ecalloc(1, size);
            e->size = size;
            return e->data;
        }
        prev = e;
    }

    prev->next = (ArenaNode *)ecalloc(1, sizeof(ArenaNode));
    prev->next->data = ecalloc(1, size);
    prev->next->size = size;
    return prev->next->data;
}

#endif /* ARENA END */

#endif /* LIB_H_IMPL_ END */

/*
C/C++ OPERATOR PRECEDENCE TABLE (https://en.cppreference.com/w/cpp/language/operator_precedence)
======================================================================================
Precedence   Operator          Description                          Associativity
======================================================================================
01 (highest) ::                Scope resolution (C++)               None

02           ++                Postfix increment                    Left-to-right
"            --                Postfix decrement                    "
"            ()                Function call                        "
"            []                Array subscripting                   "
"            .                 Element selection by reference       "
"            ->                Element selection through pointer    "
"            typeid()          Run-time type information (C++)      "
"            const_cast        Type cast (C++)                      "
"            dynamic_cast      Type cast (C++)                      "
"            reinterpret_cast  Type cast (C++)                      "
"            static_cast       Type cast (C++)                      "

03           ++                Prefix increment                     Right-to-left
"            --                Prefix decrement                     "
"            +                 Unary plus                           "
"            -                 Unary minus                          "
"            !                 Logical NOT                          "
"            ~                 Bitwise NOT (ones' complement)       "
"            (type)            Type cast                            "
"            *                 Indirection (dereference)            "
"            &                 Address-of                           "
"            sizeof            Sizeof                               "
"            _Alignof          Alignment requirement (since C11)    "
"            new, new[]        Dynamic memory allocation (C++)      "
"            delete, delete[]  Dynamic memory deallocation (C++)    "

04           .*                Pointer to member (C++)              Left-to-right
"            ->*               Pointer to member (C++)              "

05           *                 Multiplication                       Left-to-right
"            /                 Division                             "
"            %                 Modulo (remainder)                   "

06           +                 Addition                             Left-to-right
"            -                 Subtraction                          "

07           <<                Bitwise left shift                   Left-to-right
"            >>                Bitwise right shift                  "

08           <                 Less than                            Left-to-right
"            <=                Less than or equal to                "
"            >                 Greater than                         "
"            >=                Greater than or equal to             "

09           ==                Equal to                             Left-to-right
"            !=                Not equal to                         "

10           &                 Bitwise AND                          Left-to-right

11           ^                 Bitwise XOR (exclusive or)           Left-to-right

12           |                 Bitwise OR (inclusive or)            Left-to-right

13           &&                Logical AND                          Left-to-right

14           ||                Logical OR                           Left-to-right

15           ?:                Ternary conditional operator         Right-to-left
"            =                 Direct assignment                    "
"            +=                Assignment by sum                    "
"            -=                Assignment by difference             "
"            *=                Assignment by product                "
"            /=                Assignment by quotient               "
"            %=                Assignment by remainder              "
"            <<=               Assignment by bitwise left shift     "
"            >>=               Assignment by bitwise right shift    "
"            &=                Assignment by bitwise AND            "
"            ^=                Assignment by bitwise XOR            "
"            |=                Assignment by bitwise OR             "
"            throw             Throw operator (C++)                 "

16 (lowest)  ,                 Comma                                Left-to-right
======================================================================================
Precedence   Operator          Description                          Associativity
======================================================================================
TABLE END
*/

#endif /* LIB_H_ END */

/*
Copyright (c) 2024 Daniel de Lima Franceschetti <0xfeedcafe@proton.me>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
