#define LIB_H_IMPL
#include "lib.h"

int main(int argc, char **argv)
{
    print("[" LIB_H_OSNAME "]\n");

    if (argc == 1) {
        char name[32];
        int ret;

        ret = input("What is your name? ", name, sizeof(name));

        if (ret < 0)
            die(":(\n");
        else if (ret == sizeof(name)-1)
            print("(buffer filled) ");

        if (ret == 0) {
            print("(empty)\n");
        } else {
            print("Hello, ");
            print(name);
            print("!\n");
        }

        char slen[3];
        if (int_to_cstr(ret, slen, sizeof(slen)) >= 0) {
            print("[");
            print(slen);
            print("]\n");
        }
    } else if (argc == 2) {
        char *end = NULL;
        long n = cstr_to_int(argv[1], &end);

        if (*end != '\0') {
            print("Stopped at: ");
            print(end);
            print("\n");
        }
        n += 1;
        if (int_to_cstr(n, argv[1], cstr_len(argv[1])+1) < 0)
            print("Error: ");
        print(argv[1]);
        print("\n");
    } else {
        die("I don't know what to do...\n");
    }
    print("Bye...\n");
    return EXIT_SUCCESS;
}
