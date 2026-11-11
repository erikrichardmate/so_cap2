/*
 * Problema 8
 * Realizati o aplicatie care sorteaza continutul unui fisier folosind
 * algoritmul Bubble Sort. Nu se accepta folosirea unui tablou,
 * ci numai folosirea functiei lseek.
 *
 * Utilizare: ex08 fisier
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
    int c, anterior, schimbat;
    long poz;

    if (argc != 2)
        return 1;

    f = fopen(argv[1], "r+b");

    if (f == NULL)
        return 1;

    fseek(f, 0, SEEK_END);
    poz = ftell(f);

    do
    {
        schimbat = 0;
        fseek(f, 0, SEEK_SET);

        anterior = fgetc(f);

        while ((c = fgetc(f)) != EOF)
        {
            if (anterior > c)
            {
                poz = ftell(f);

                fseek(f, -2, SEEK_CUR);
                fputc(c, f);
                fputc(anterior, f);

                fseek(f, poz, SEEK_SET);
                schimbat = 1;
            }

            anterior = fgetc(f);

            if (anterior == EOF)
                break;

            fseek(f, -1, SEEK_CUR);
        }

    } while (schimbat);

    fclose(f);

    return 0;
}
