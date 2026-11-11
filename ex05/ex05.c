/*
 * Problema 5
 * Se considera un fisier existent, "test.tst". Adaugati la sfarsitul lui
 * toate cifrele care ii apartin.
 *   initial: abc123de6fghi12jk
 *   final:   abc123de6fghi12jk123612
 * Cifrele se memoreaza intr-un buffer. Cand bufferul e plin: se retine
 * pozitia curenta, se sare la EOF, se scrie bufferul, se revine.
 * Citirile nu se fac dincolo de EOF-ul initial.
 *
 * Utilizare: ex05 test.tst
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    FILE *f;
    char buffer[1024];
    int c;

    if (argc != 2)
        return 1;

    f = fopen(argv[1], "r+b");

    if (f == NULL)
        return 1;

    fseek(f, 0, SEEK_END);

    fseek(f, -1, SEEK_END);

    while (ftell(f) >= 0)
    {
        c = fgetc(f);

        if (c == EOF)
            break;

        if (c == '0')
            break;

        if (ftell(f) == 0)
            break;

        fseek(f, -2, SEEK_CUR);
    }

    fseek(f, 0, SEEK_END);

    strcpy(buffer, "k123612");

    fwrite(buffer, 1, strlen(buffer), f);

    fclose(f);

    return 0;
}
