/*
 * Problema 3
 * Realizati un program de cautare de fisier, specificat printr-un nume
 * extins (poate contine ?, *), pornind de la directorul curent.
 * Pentru fiecare fisier gasit se afiseaza calea completa si numele.
 *
 * Utilizare: ex03 tipar   (ex: ex03 *.txt)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct ffblk ff;
    char cale[260];
    int nr = 0;

    if (argc != 2)
        return 1;

    if (findfirst(argv[1], &ff, FA_ARCH | FA_HIDDEN | FA_RDONLY | FA_SYSTEM) == 0)
    {
        do
        {
            if (!(ff.ff_attrib & FA_DIREC))
            {
                nr++;
                printf("%s%s\n", getcwd(cale, 260), ff.ff_name);
            }
        } while (findnext(&ff) == 0);
    }

    printf("Numar fisiere: %d\n", nr);

    return 0;
}
