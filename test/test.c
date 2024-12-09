#define TEST
#define ARENA

#define _DEFAULT_SOURCE
#include <ctype.h>
#include <string.h>

#define L_AT_EXIT do{ print("Tests run: "); print_i_ln(tests_run); }while(0)

#define LIB_H_IMPL
#include "lib.h"

#define STR_SIZE 2048
typedef struct {
    char s[STR_SIZE];
    size_t len;
} CstrEntry;

typedef struct {
    char a[STR_SIZE];
    char b[STR_SIZE];
    int comp;
} CstrCompEntry;

static CstrEntry cstr_entries[] = {/*{{{*/
    {"", 0},
    {"a", 1},
    {"aa", 2},
    {"aaa", 3},
    {"aaaa", 4},
    {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n", 1024},
/*}}}*/};

static CstrCompEntry cstr_comp_entries[] = {/*{{{*/
    {"", "", 0},
    {"a", "a", 0},
    {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0},
    {"", "a", -1},
    {"", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", -1},
    {"a", "aaa", -1},
    {"a", "", 1},
    {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "", 1},
    {"aaa", "a", 1},
/*}}}*/};

typedef struct {
    char c;
    bool is_ascii;
    bool is_cntrl;
    bool is_blank;
    bool is_space;
    bool is_print;
    bool is_graph;
    bool is_digit;
    bool is_upper;
    bool is_lower;
    bool is_alpha;
    bool is_alnum;
    bool is_punct;
    bool is_xdigt;
} CharInfo;

const CharInfo ascii_chars[128] = {
    /* {{{           a  c  b  s  p  g  d  u  l  a  a  p  x
                     s  n  l  p  r  r  i  p  o  l  l  u  d
                     c  t  a  a  i  a  g  p  w  p  n  n  i
                     i  r  n  c  n  p  i  e  e  h  u  c  g
                     i  l  k  e  t  h  t  r  r  a  m  t  t */
    /*000*/ {'\0',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* NUL (null) */
    /*001*/ {'\1',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* SOH (start of header) */
    /*002*/ {'\2',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* STX (start of text) */
    /*003*/ {'\3',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ETX (end of text) */
    /*004*/ {'\4',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* EOT (end of transmission) */
    /*005*/ {'\5',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ENQ (enquiry) */
    /*006*/ {'\6',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ACK (acknowledge) */
    /*007*/ {'\7',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* BEL (bell) */
    /*008*/ {'\b',   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* BS (backspace) */
    /*009*/ {'\t',   1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* HT (horizontal tab) */
    /*010*/ {'\n',   1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* LF (line feed - new line) */
    /*011*/ {'\v',   1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* VT (vertical tab) */
    /*012*/ {'\f',   1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* FF (form feed - new page) */
    /*013*/ {'\r',   1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* CR (carriage return) */
    /*014*/ {'\x0E', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* SO (shift out) */
    /*015*/ {'\x0F', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* SI (shift in) */
    /*016*/ {'\x10', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DLE (data link escape) */
    /*017*/ {'\x11', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DC1 (device control 1) */
    /*018*/ {'\x12', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DC2 (device control 2) */
    /*019*/ {'\x13', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DC3 (device control 3) */
    /*020*/ {'\x14', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DC4 (device control 4) */
    /*021*/ {'\x15', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* NAK (negative acknowledge) */
    /*022*/ {'\x16', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* SYN (synchronous idle) */
    /*023*/ {'\x17', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ETB (end of transmission block) */
    /*024*/ {'\x18', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* CAN (cancel) */
    /*025*/ {'\x19', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* EM (end of medium) */
    /*026*/ {'\x1A', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* SUB (substitute) */
    /*027*/ {'\x1B', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ESC (escape) */
    /*028*/ {'\x1C', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* FS (file separator) */
    /*029*/ {'\x1D', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GS (group separator) */
    /*030*/ {'\x1E', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* RS (record separator) */
    /*031*/ {'\x1F', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* US (unit separator) */
    /*032*/ {' ',    1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    /*033*/ {'!',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*034*/ {'"',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*035*/ {'#',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*036*/ {'$',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*037*/ {'%',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*038*/ {'&',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*039*/ {'\'',   1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*040*/ {'(',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*041*/ {')',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*042*/ {'*',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*043*/ {'+',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*044*/ {',',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*045*/ {'-',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*046*/ {'.',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*047*/ {'/',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*048*/ {'0',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*049*/ {'1',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*050*/ {'2',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*051*/ {'3',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*052*/ {'4',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*053*/ {'5',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*054*/ {'6',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*055*/ {'7',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*056*/ {'8',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*057*/ {'9',    1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    /*058*/ {':',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*059*/ {';',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*060*/ {'<',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*061*/ {'=',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*062*/ {'>',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*063*/ {'?',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*064*/ {'@',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*065*/ {'A',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*066*/ {'B',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*067*/ {'C',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*068*/ {'D',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*069*/ {'E',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*070*/ {'F',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    /*071*/ {'G',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*072*/ {'H',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*073*/ {'I',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*074*/ {'J',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*075*/ {'K',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*076*/ {'L',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*077*/ {'M',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*078*/ {'N',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*079*/ {'O',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*080*/ {'P',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*081*/ {'Q',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*082*/ {'R',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*083*/ {'S',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*084*/ {'T',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*085*/ {'U',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*086*/ {'V',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*087*/ {'W',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*088*/ {'X',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*089*/ {'Y',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*090*/ {'Z',    1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},
    /*091*/ {'[',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*092*/ {'\\',   1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*093*/ {']',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*094*/ {'^',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*095*/ {'_',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*096*/ {'`',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*097*/ {'a',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*098*/ {'b',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*099*/ {'c',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*100*/ {'d',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*101*/ {'e',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*102*/ {'f',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    /*103*/ {'g',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*104*/ {'h',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*105*/ {'i',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*106*/ {'j',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*107*/ {'k',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*108*/ {'l',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*109*/ {'m',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*110*/ {'n',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*111*/ {'o',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*112*/ {'p',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*113*/ {'q',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*114*/ {'r',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*115*/ {'s',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*116*/ {'t',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*117*/ {'u',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*118*/ {'v',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*119*/ {'w',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*120*/ {'x',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*121*/ {'y',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*122*/ {'z',    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    /*123*/ {'{',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*124*/ {'|',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*125*/ {'}',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*126*/ {'~',    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
    /*127*/ {'\x7F', 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* DEL (delete) }}} */
};

const struct{ulong x, y;} square_roots[] = {
    { 0, 0},
    { 1, 1},
    { 2, 1},
    { 3, 2},
    { 4, 2},
    { 5, 2},
    { 6, 2},
    { 7, 2},
    { 8, 2},
    { 9, 3},
    {16, 4},
    {25, 5},
    {36, 6},
    {49, 7},
    {64, 8},
    {81, 9},
    {100,10},
    {1<<8, 1<<4},
    {1<<10,1<<5},
    {1<<12,1<<6},
    {1<<14,1<<7},
};

int tests_run = 0;

testmsg test_cstr_len(void)
{
    for (int i = 0; i < LENGTH(cstr_entries); ++i) {
        size_t a = cstr_len(cstr_entries[i].s);
        size_t b = strlen(cstr_entries[i].s);
        size_t len = cstr_entries[i].len;

        DBG_LOG("cstr_len(\""); {
            dbg_print(len > 74 ? "..." : cstr_entries[i].s);
            dbg_print("\") = ");
            dbg_print_i((int)a);
            dbg_print(",\tstrlen(\"");
            dbg_print(len > 74 ? "..." : cstr_entries[i].s);
            dbg_print("\") = ");
            dbg_print_i((int)b);
            dbg_print(" (expects ");
            dbg_print_i((int)len);
            dbg_print(")");
        } DBG_LOG_END;

        test_assert("test_cstr_len", EQ3(a, b, len));
    }
    return NULL;
}

testmsg test_cstr_len_max(void)
{
    for (int i = 0; i < LENGTH(cstr_entries); ++i) {
        size_t len = cstr_entries[i].len;

        test_assert("test_cstr_len_max: Strings must be initially null-terminated.",
                    cstr_entries[i].s[len] == '\0');
        cstr_entries[i].s[len] = '#';
        cstr_entries[i].s[len+1] = '\0';

        size_t a = cstr_len_max(cstr_entries[i].s, len);
        size_t b = strnlen(cstr_entries[i].s, len);

        DBG_LOG("cstr_len_max(\""); {
            dbg_print(len > 74 ? "..." : cstr_entries[i].s);
            dbg_print("\", max) = ");
            dbg_print_i((int)a);
            dbg_print(",\tstrnlen(\"");
            dbg_print(len > 74 ? "..." : cstr_entries[i].s);
            dbg_print("\", max) = ");
            dbg_print_i((int)b);
            dbg_print(" (expects ");
            dbg_print_i((int)len);
            dbg_print(", max == ");
            dbg_print_i((int)len);
            dbg_print(")");
        } DBG_LOG_END;

        test_assert("test_cstr_len_max", EQ3(a, b, len));

        cstr_entries[i].s[cstr_entries[i].len] = '\0';
    }
    return NULL;
}

testmsg test_cstr_comp(void)
{
    for (int i = 0; i < LENGTH(cstr_comp_entries); ++i) {
        char *s1 = cstr_len(cstr_comp_entries[i].a) > 30 ? "......" : cstr_comp_entries[i].a;
        char *s2 = cstr_len(cstr_comp_entries[i].b) > 30 ? "......" : cstr_comp_entries[i].b;

        int a = cstr_comp(cstr_comp_entries[i].a, cstr_comp_entries[i].b);
        int b = strcmp(cstr_comp_entries[i].a, cstr_comp_entries[i].b);
        int c = cstr_comp_entries[i].comp;

        DBG_LOG("s1 = \""); {
            dbg_print(s1);
            dbg_print("\", s2 = \"");
            dbg_print(s2);
            dbg_print("\"; cstr_comp(s1, s2) = ");
            dbg_print_i(a);
            dbg_print(", strcmp(s1, s2) = ");
            dbg_print_i(b);
            dbg_print(" (expects ");
            dbg_print_i(c);
            dbg_print(")");
        } DBG_LOG_END;

        test_assert("test_cstr_comp", EQ3(a, b, c));
    }
    return NULL;
}

testmsg test_cstr_comp_max(void)
{
    for (int i = 0; i < LENGTH(cstr_comp_entries); ++i) {
        size_t a_len = cstr_len(cstr_comp_entries[i].a);
        size_t b_len = cstr_len(cstr_comp_entries[i].b);
        size_t max = MAX(a_len, b_len);

        char *s1 = a_len > 30 ? "......" : cstr_comp_entries[i].a;
        char *s2 = b_len > 30 ? "......" : cstr_comp_entries[i].b;

        int a = cstr_comp_max(cstr_comp_entries[i].a, cstr_comp_entries[i].b, max);
        int b = strncmp(cstr_comp_entries[i].a, cstr_comp_entries[i].b, max);
        int c = cstr_comp_entries[i].comp;

        cstr_comp_entries[i].a[a_len] = '\0';
        cstr_comp_entries[i].b[b_len] = '#';

        DBG_LOG("s1 = \""); {
            dbg_print(s1);
            dbg_print("\", s2 = \"");
            dbg_print(s2);
            dbg_print("\"; cstr_comp_max(s1, s2, max) = ");
            dbg_print_i(a);
            dbg_print(", strncmp(s1, s2, max) = ");
            dbg_print_i(b);
            dbg_print(" (expects ");
            dbg_print_i(c);
            dbg_print(", max == ");
            dbg_print_i((int)max);
            dbg_print(")");
        } DBG_LOG_END;

        test_assert("test_cstr_comp_max", EQ3(a, b, c));

        cstr_comp_entries[i].a[a_len] = '\0';
        cstr_comp_entries[i].b[b_len] = '\0';
    }
    return NULL;
}

static char cstr_copies[LENGTH(cstr_entries)][STR_SIZE];
testmsg test_cstr_copy(void)
{
    for (int i = 0; i < LENGTH(cstr_entries); ++i) {
        size_t len = cstr_entries[i].len;

        cstr_copies[i][len+1] = '#';
        char *p = cstr_copy(cstr_copies[i], cstr_entries[i].s);

        DBG_LOG("cstr_copy(dst, \""); {
            dbg_print(len > 30 ? "..." : cstr_entries[i].s);
            dbg_print("\") = \"");
            dbg_print(len > 30 ? "..." : cstr_copies[i]);
            dbg_print("\"");
        } DBG_LOG_END;

        test_assert("Returned string must be pointing to null byte.", *p == '\0');
        test_assert("test_cstr_copy", cstr_comp(cstr_copies[i], cstr_entries[i].s) == 0);
        test_assert("test_cstr_copy", cstr_copies[i][len+1] == '#');

        cstr_copies[i][len+1] = '\0';
    }
    return NULL;
}

testmsg test_sqrt_ul(void)
{
    for (int i = 0; i < LENGTH(square_roots); ++i) {
        ulong root = sqrt_ul(square_roots[i].x);

        DBG_LOG("sqrt("); {
            dbg_print_i((int)square_roots[i].x);
            dbg_print(") = ");
            dbg_print_i((int)root);
            dbg_print(" (expects ");
            dbg_print_i((int)square_roots[i].y);
            dbg_print(")");
        } DBG_LOG_END;

        test_assert("test_sqrt_ul", root == square_roots[i].y);
    }
    return NULL;
}

testmsg test_misc(void)
{
    DBG_LOG("Testing miscellaneous stuff...\n");

    test_assert("test_misc", false == 0);
    test_assert("test_misc", true == !false);
    test_assert("test_misc", MAX(1, 0) == 1);
    test_assert("test_misc", MIN(1, 0) == 0);
    test_assert("test_misc", MAX(0, 1) == 1);
    test_assert("test_misc", MIN(0, 1) == 0);
    test_assert("test_misc", BETWEEN(-1, 0, 2) == 0);
    test_assert("test_misc", BETWEEN(0, 0, 2) == 1);
    test_assert("test_misc", BETWEEN(1, 0, 2) == 1);
    test_assert("test_misc", BETWEEN(2, 0, 2) == 1);
    test_assert("test_misc", BETWEEN(3, 0, 2) == 0);

    for (int i = 0; i < LENGTH(ascii_chars); ++i) { /*{{{*/
        char c = ascii_chars[i].c;
        test_assert("test_misc: is_ascii", EQ3(IS_ASCII(c),  !!isascii(ascii_chars[i].c), ascii_chars[i].is_ascii));
        test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(c),  !!iscntrl(ascii_chars[i].c), ascii_chars[i].is_cntrl));
        test_assert("test_misc: is_blank", EQ3(IS_BLANK(c),  !!isblank(ascii_chars[i].c), ascii_chars[i].is_blank));
        test_assert("test_misc: is_space", EQ3(IS_SPACE(c),  !!isspace(ascii_chars[i].c), ascii_chars[i].is_space));
        test_assert("test_misc: is_print", EQ3(IS_PRINT(c),  !!isprint(ascii_chars[i].c), ascii_chars[i].is_print));
        test_assert("test_misc: is_graph", EQ3(IS_GRAPH(c),  !!isgraph(ascii_chars[i].c), ascii_chars[i].is_graph));
        test_assert("test_misc: is_digit", EQ3(IS_DIGIT(c),  !!isdigit(ascii_chars[i].c), ascii_chars[i].is_digit));
        test_assert("test_misc: is_upper", EQ3(IS_UPPER(c),  !!isupper(ascii_chars[i].c), ascii_chars[i].is_upper));
        test_assert("test_misc: is_lower", EQ3(IS_LOWER(c),  !!islower(ascii_chars[i].c), ascii_chars[i].is_lower));
        test_assert("test_misc: is_alpha", EQ3(IS_ALPHA(c),  !!isalpha(ascii_chars[i].c), ascii_chars[i].is_alpha));
        test_assert("test_misc: is_alnum", EQ3(IS_ALNUM(c),  !!isalnum(ascii_chars[i].c), ascii_chars[i].is_alnum));
        test_assert("test_misc: is_punct", EQ3(IS_PUNCT(c),  !!ispunct(ascii_chars[i].c), ascii_chars[i].is_punct));
        test_assert("test_misc: is_xdigt", EQ3(IS_XDIGT(c), !!isxdigit(ascii_chars[i].c), ascii_chars[i].is_xdigt));
    } /*}}}*/

    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(-1), !!isascii(-1), 0));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(-128), !!isascii(-128), 0));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(128), !!isascii(128), 0));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(0), !!isascii(0), 1));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(1), !!isascii(1), 1));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(126), !!isascii(126), 1));
    test_assert("test_misc: is_ascii", EQ3(IS_ASCII(127), !!isascii(127), 1));

    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(-1), !!iscntrl(-1), 0));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(-2), !!iscntrl(-2), 0));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(32), !!iscntrl(32), 0));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(128), !!iscntrl(128), 0));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(0), !!iscntrl(0), 1));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(31), !!iscntrl(31), 1));
    test_assert("test_misc: is_cntrl", EQ3(IS_CNTRL(127), !!iscntrl(127), 1));

    int i;
    llong l;
    for (i = 0, l = 1; i < POWER10_LEN-1; ++i, l *= 10) {
        test_assert("test_misc: power10", power10[i] == l);
    }
    test_assert("test_misc: power10", power10[POWER10_LEN-1] == l);

    return NULL;
}

static testmsg all_tests(void)
{
    test_run(test_misc);
    test_run(test_cstr_len);
    test_run(test_cstr_len_max);
    test_run(test_cstr_comp);
    test_run(test_cstr_comp_max);
    test_run(test_cstr_copy);
    test_run(test_sqrt_ul);
    return NULL;
}

int main(void)
{
    testmsg res = all_tests();

    if (res) {
        print("FAILED: ");
        print_ln(res);
    } else {
        print_ln("ALL TESTS PASSED");
    }

    L_AT_EXIT;
    return res ? EXIT_FAILURE : EXIT_SUCCESS;
}
