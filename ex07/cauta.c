/*
 * Problema 7
 * Realizati programul cauta care cauta, recursiv, fisierele cu extensia
 * ext care contin sirul text (in continutul fisierului, nu in nume).
 *   -cx  textul trebuie sa apara de cel putin x ori in fisier
 *
 * Utilizare: cauta [fanion] ext text
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct ffblk ff;
    char directoare[500][260];
    char cale[260], nume[260], extensie[30];
    char linie[1024];
    char *p;
    FILE *f;
    int top = 0, i, x = 1, nr, gasite;
    int opt = 1;

    if (argc < 3)
        return 1;

    if (strncmp(argv[1], "-c", 2) == 0)
    {
        x = atoi(argv[1] + 2);
        opt = 2;
    }

    if (argc - opt < 2)
        return 1;

    strcpy(extensie, argv[opt]);
    if (extensie[0] != '.')
    {
        memmove(extensie + 1, extensie, strlen(extensie) + 1);
        extensie[0] = '.';
    }

    strcpy(directoare[0], ".");

    while (top >= 0)
    {
        strcpy(cale, directoare[top--]);
        chdir(cale);

        if (findfirst("*.*", &ff, FA_DIREC | FA_ARCH | FA_HIDDEN | FA_RDONLY | FA_SYSTEM) == 0)
        {
            do
            {
                if (strcmp(ff.ff_name, ".") == 0 ||
                    strcmp(ff.ff_name, "..") == 0)
                    continue;

                if (ff.ff_attrib & FA_DIREC)
                {
                    if (top < 498)
                    {
                        sprintf(directoare[++top], "%s\\%s", cale, ff.ff_name);
                    }
                }
                else
                {
                    p = strrchr(ff.ff_name, '.');

                    if (p != NULL && stricmp(p, extensie) == 0)
                    {
                        f = fopen(ff.ff_name, "r");

                        if (f != NULL)
                        {
                            nr = 0;

                            while (fgets(linie, sizeof(linie), f) != NULL)
                            {
                                p = linie;

                                while ((p = strstr(p, argv[argc - 1])) != NULL)
                                {
                                    nr++;
                                    p++;
                                }
                            }

                            fclose(f);

                            if (nr >= x)
                            {
                                printf("%s\\%s\n", cale, ff.ff_name);
                            }
                        }
                    }
                }

            } while (findnext(&ff) == 0);
        }

        chdir("..");
    }

    return 0;
}
