/*
 * Problema 4
 * Realizati un program care primeste in linia de comanda numele a doua
 * fisiere, sursa si dest, si copiaza sursa in dest.
 * Se verifica sa nu coincida sursa si dest.
 * Se lucreaza fara redirectari si duplicari.
 *
 * Utilizare: ex04 sursa dest
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat st1, st2;
    char cale[260];

    if (argc != 3)
        return 1;

    if (stat(argv[1], &st1) != 0 || stat(argv[2], &st2) != 0)
        return 1;

    if (strcmp(argv[1], argv[2]) == 0)
        return 1;

    if (rename(argv[1], argv[2]) != 0)
    {
        FILE *fin = fopen(argv[1], "rb");
        FILE *fout = fopen(argv[2], "wb");

        if (fin == NULL || fout == NULL)
            return 1;

        int c;
        while ((c = fgetc(fin)) != EOF)
            fputc(c, fout);

        fclose(fin);
        fclose(fout);
        remove(argv[1]);
    }

    return 0;
}
