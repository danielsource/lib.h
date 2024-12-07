/* lib.h: a C99 header-only library for fun.
Usage:
    #define LIB_H_IMPL
    #include "lib.h"
*/
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

#ifndef l_int
#   define l_int long /* At least 32 bits. */
#else
#   pragma message("l_int defined")
#endif

/* Boolean. */
#if !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
#endif

/* stdint.h alternate labels. */
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

/* Make unsigned types shorter. */
typedef long long          llong;
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

/* Common useful macros. */
#define LENGTH(arr) ((int)(sizeof(arr) / sizeof(*(arr))))
#define UNUSED(x) ((void)(x))

/* Default buffer size.  */
#ifndef L_BUFSIZE
#define L_BUFSIZE 512
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

#ifdef TEST
/* TODO: Minimal unit testing.
 * Source (modified): https://jera.com/techinfo/jtns/jtn002 */
#define test_assert(msg, cond) do{ if (!(cond)) return msg; }while(0)
#define test_run(test_func)    do{ char *msg = test_func(); tests_run++;\
                                   if (msg) return msg; }while(0)
extern int tests_run = 0;
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

/* TODO: test with and without gcc -funsigned-char. */
/* ctype.h replacement of is[a-z]+ functions. */
#define is_ascii(c) (((c) & ~127) == 0)
#define is_cntrl(c) (((c) >= 0 && (c) <= 31) || (c) == 127)
#define is_blank(c) ((c) == ' ' || (c) == '\t')
#define is_space(c) (is_blank(c) || (c) == '\f' || (c) == '\n' || (c) == '\r' || (c) == '\v')
#define is_print(c) (is_ascii(c) && !is_cntrl(c))
#define is_graph(c) (is_print(c) && (c) != ' ')
#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_upper(c) ((c) >= 'A' && (c) <= 'Z')
#define is_lower(c) ((c) >= 'a' && (c) <= 'z')
#define is_alpha(c) (is_upper(c) || is_lower(c))
#define is_alnum(c) (is_alpha(c) || is_digit(c))
#define is_punct(c) (is_graph(c) && !is_alnum(c))
#define is_xdigt(c) (is_digit(c) || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

/* ctype.h replacement of to[a-z]+ functions. */
#define to_ascii(c) ((c) & 127)
#define to_upper(c) (is_lower(c) ? (c) - 32 : (c))
#define to_lower(c) (is_upper(c) ? (c) + 32 : (c))

#if defined(__cplusplus)
extern "C" {
#endif

size_t cstr_len     (const char *s);
size_t cstr_len_max (const char s[/*max*/], size_t max);
char  *cstr_copy    (char *dst, const char *src);
char  *cstr_copy_max(char *dst, const char src[/*max*/], size_t max);
void  *reverse      (void *data, size_t len);       /*TODO*/
l_int  cstr_to_int  (const char *s, char **end);
int    int_to_cstr  (l_int n, char s[/*max*/], size_t max);
int    print_cstr   (bool err, const char *s);
int    print_int    (bool err, l_int n);
int    input        (const char *prompt, char dst[/*max*/], size_t max);
int    int_digits   (l_int n);
void   exit_process (int status);
void   die          (const char *msg);

int histogram_charfreq(const char *text);           /*TODO*/
int histogram_wordlen(const char *text);            /*TODO*/
int rpn_calc        (const char *expr, char **end); /*TODO*/

#define print(s)  print_cstr(false, s)
#define printn(n) print_int(false, n)

#ifdef DEBUG
#define dbg_print(s)  print_cstr(true, s)
#define dbg_printn(n) print_int(true, n)
#else
#define dbg_print(s)
#define dbg_printn(n)
#endif

#if defined(__cplusplus)
}
#endif

extern const llong power10[];

#ifdef LIB_H_IMPL

#if defined(LIB_H_OS_WINDOWS)
#   include <windows.h>
#elif defined(LIB_H_OS_UNIXLIKE)
#   include <unistd.h>
#else
#   include <stdio.h>
#   include <stdlib.h>
#endif

/* 10^0 to 10^18. */
const llong power10[] = {
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

char *cstr_copy(char *dst, const char *src)
{
    dbg_assert(dst);
    dbg_assert(src);
    while ((*(dst++) = *(src++)));
    return dst;
}

char *cstr_copy_max(char *dst, const char src[/*max*/], size_t max)
{
    dbg_assert(dst);
    dbg_assert(src);
    while (max-- && (*(dst++) = *(src++)));
    return dst;
}

l_int cstr_to_int(const char *s, char **end)
{
    l_int n = 0, sign = 1, len = 10;

    dbg_assert(s);

    while (is_space(*s)) ++s;

    if (*s == '+') {
        ++s;
    } else if (*s == '-') {
        sign = -1;
        ++s;
    }

    while (is_digit(*s) && len > 0) {
        /* NOTE: This does not handle overflow. */
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

    l_int digits = int_digits(n);

    if (digits <= 0) {
        *s = '\0';
        return -1;
    }

    *s = (n / power10[digits-1]) + '0';
    ++s;
    --max;
    --digits;

    while (max > 1 && digits > 0) {
        *s = ((n / power10[digits-1]) % 10) + '0';
        ++s;
        --max;
        --digits;
    }

    *s = '\0';
    return digits;
}

/* Returns non-negative on success. */
int print_cstr(bool err, const char *s)
{
    int ret = 0;

    dbg_assert(s);
    const size_t len = cstr_len(s);
    dbg_assert(len <= (1 << 15) - 1);

#if defined(LIB_H_OS_WINDOWS)
    HANDLE out = GetStdHandle((err)? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
    if (!out || out == INVALID_HANDLE_VALUE ||
            !WriteConsoleA(out, s, len, &ret, NULL)
        return -1;
#elif defined(LIB_H_OS_UNIXLIKE)
    ret = write((err)? STDERR_FILENO : STDOUT_FILENO, s, len);
    if (ret < 0) return -1;
#else
    if (fputs(s, (err)? stderr : stdout) >= 0)
        ret = len;
    else
        ret = -1;
#endif

    return ret;
}

/* Returns non-negative on success. */
int print_int(bool err, l_int n)
{
    int ret;
    char s[12];

    ret = int_to_cstr(n, s, sizeof(s));
    dbg_assert(ret >= 0);

    return print_cstr(err, s);
}

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
    if (ret > (int)max-1) ret = max-1;
#elif defined(LIB_H_OS_UNIXLIKE)
    ret = read(STDIN_FILENO, l_buf_, L_BUFSIZE-1);
    if (ret <= 0) {
        print("\n");
        return -1;
    }
    cstr_copy_max(dst, l_buf_, max-1);
    if (ret > (int)max-1) ret = max-1;
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

void exit_process(int status)
{
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
    if (msg) print_cstr(true, msg);
    exit_process(EXIT_FAILURE);
}

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
