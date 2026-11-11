/*
 * Problema 2
 * Realizati programul cpd care muta cu acelasi nume toate fisierele din
 * director_sursa in director_destinatie, impreuna cu structura arborescenta.
 *   -a   se muta si fisierele ascunse
 *   -dx  se muta fisiere pana la nivelul x de imbricare (1 = fara subdirectoare)
 *   -lx  se muta doar fisierele cu lungimea <= x
 * Se verifica sa nu fie destinatia in arborele directorului sursa.
 *
 * Utilizare: cpd [fanion...] director_sursa director_destinatie
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <dos.h>
#include <dir.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    struct ffblk ff;
    union REGS inregs, outregs;
    struct SREGS sregs;

    char sursa[260], destinatie[260];
    char oldname[260], newname[260];
    char surse[500][260], desturi[500][260];
    char directoare[500][260];
    char cwd[260];

    int i, j, k, top = 0, nivel;
    int adancime = 1000, ascunse = 0;
    int nr_directoare = 0;
    long lungime = LONG_MAX;
    int index_sursa, index_destinatie;
    int atribut;

    getcwd(cwd, 260);

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
            ascunse = 1;
        else if (strncmp(argv[i], "-d", 2) == 0)
            adancime = atoi(argv[i] + 2);
        else if (strncmp(argv[i], "-l", 2) == 0)
            lungime = atol(argv[i] + 2);
    }

    index_sursa = argc - 2;
    index_destinatie = argc - 1;

    if (argc < 3)
        return 1;

    if (chdir(argv[index_sursa]) != 0)
        return 1;

    getcwd(sursa, 260);

    if (chdir(cwd) != 0)
        return 1;

    if (chdir(argv[index_destinatie]) != 0)
        return 1;

    getcwd(destinatie, 260);

    chdir(cwd);

    if (strnicmp(destinatie, sursa, strlen(sursa)) == 0 &&
        (destinatie[strlen(sursa)] == '\\' ||
         destinatie[strlen(sursa)] == '\0'))
        return 1;

    strcpy(surse[0], sursa);
    strcpy(desturi[0], destinatie);
    nivel = 1;

    while (top >= 0)
    {
        strcpy(sursa, surse[top]);
        strcpy(destinatie, desturi[top]);
        nivel = 1;

        for (i = 0; i < top; i++)
            nivel++;

        top--;

        chdir(sursa);

        atribut = FA_DIREC;

        if (ascunse)
            atribut |= FA_HIDDEN;

        if (findfirst("*.*", &ff, atribut) == 0)
        {
            do
            {
                if (strcmp(ff.ff_name, ".") != 0 &&
                    strcmp(ff.ff_name, "..") != 0)
                {
                    if (ff.ff_attrib & FA_DIREC)
                    {
                        if (nivel < adancime && top < 498)
                        {
                            strcpy(oldname, sursa);
                            strcat(oldname, "\\");
                            strcat(oldname, ff.ff_name);

                            strcpy(newname, destinatie);
                            strcat(newname, "\\");
                            strcat(newname, ff.ff_name);

                            mkdir(newname);

                            strcpy(surse[++top], oldname);
                            strcpy(desturi[top], newname);

                            strcpy(directoare[nr_directoare++], oldname);
                        }
                    }
                    else if (ff.ff_fsize <= lungime)
                    {
                        strcpy(oldname, sursa);
                        strcat(oldname, "\\");
                        strcat(oldname, ff.ff_name);

                        strcpy(newname, destinatie);
                        strcat(newname, "\\");
                        strcat(newname, ff.ff_name);

                        segread(&sregs);

                        inregs.h.ah = 0x56;
                        inregs.x.dx = FP_OFF(oldname);
                        inregs.x.di = FP_OFF(newname);

                        sregs.ds = FP_SEG(oldname);
                        sregs.es = FP_SEG(newname);

                        intdosx(&inregs, &outregs, &sregs);
                    }
                }
            } while (findnext(&ff) == 0);
        }
    }

    for (i = nr_directoare - 1; i >= 0; i--)
        rmdir(directoare[i]);

    return 0;
}
