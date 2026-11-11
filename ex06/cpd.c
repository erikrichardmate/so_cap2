/*
 * Problema 6
 * Realizati programul cpd care copiaza arborele de fisiere din sursa in dest.
 *   -a   se copiaza si fisierele ascunse
 *   -o   nu se suprascriu fisierele existente in destinatie
 *   -u   se sterg fisierele din sursa
 *   -dx  se copiaza fisierele pana la nivelul x de imbricare
 * Daca dest este in arborele de subdirectoare al lui sursa, programul
 * se opreste cu mesaj de eroare.
 *
 * Utilizare: cpd [fanion...] sursa dest
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
    struct _finddata_t fileinfo[100];
    intptr_t handle[100];
    char sursa[100][260], dest[100][260];
    char cale[260], caleDest[260];
    char buffer[8192];
    FILE *fin, *fout;
    int i, nivel, maxNivel = 100;
    int ascunse = 0, sterge = 0;
    int optiuni = 1;
    size_t n;

    if (argc < 3)
        return 1;

    while (optiuni < argc - 2)
    {
        if (strcmp(argv[optiuni], "-a") == 0)
            ascunse = 1;
        else if (strcmp(argv[optiuni], "-u") == 0)
            sterge = 1;
        else if (strncmp(argv[optiuni], "-d", 2) == 0)
            maxNivel = atoi(argv[optiuni] + 2);

        optiuni++;
    }

    strcpy(sursa[0], argv[argc - 2]);
    strcpy(dest[0], argv[argc - 1]);

    _fullpath(sursa[0], sursa[0], 260);
    _fullpath(dest[0], dest[0], 260);

    if (strnicmp(sursa[0], dest[0], strlen(sursa[0])) == 0 &&
        (dest[0][strlen(sursa[0])] == '\\' ||
         dest[0][strlen(sursa[0])] == '/'))
        return 1;

    _mkdir(dest[0]);

    nivel = 1;

    {
        char directoare[100][260];
        int top = 0;

        strcpy(directoare[0], sursa[0]);

        while (top >= 0)
        {
            strcpy(sursa[nivel - 1], directoare[top]);
            top--;

            strcpy(cale, sursa[nivel - 1]);
            strcat(cale, "\\*.*");

            handle[nivel - 1] = _findfirst(cale, &fileinfo[nivel - 1]);

            if (handle[nivel - 1] == -1)
                continue;

            do
            {
                if (strcmp(fileinfo[nivel - 1].name, ".") == 0 ||
                    strcmp(fileinfo[nivel - 1].name, "..") == 0)
                    continue;

                if (!ascunse && (fileinfo[nivel - 1].attrib & _A_HIDDEN))
                    continue;

                strcpy(cale, sursa[nivel - 1]);
                strcat(cale, "\\");
                strcat(cale, fileinfo[nivel - 1].name);

                strcpy(caleDest, dest[nivel - 1]);
                strcat(caleDest, "\\");
                strcat(caleDest, fileinfo[nivel - 1].name);

                if (fileinfo[nivel - 1].attrib & _A_SUBDIR)
                {
                    _mkdir(caleDest);

                    if (nivel < maxNivel && top < 98)
                    {
                        strcpy(directoare[++top], cale);
                        strcpy(dest[top], caleDest);
                    }
                }
                else
                {
                    fin = fopen(cale, "rb");
                    fout = fopen(caleDest, "wb");

                    if (fin != NULL && fout != NULL)
                    {
                        while ((n = fread(buffer, 1, sizeof(buffer), fin)) > 0)
                            fwrite(buffer, 1, n, fout);

                        fclose(fin);
                        fclose(fout);

                        if (sterge)
                            remove(cale);
                    }
                    else
                    {
                        if (fin != NULL)
                            fclose(fin);

                        if (fout != NULL)
                            fclose(fout);
                    }
                }

            } while (_findnext(handle[nivel - 1], &fileinfo[nivel - 1]) == 0);

            _findclose(handle[nivel - 1]);
        }
    }

    return 0;
}
