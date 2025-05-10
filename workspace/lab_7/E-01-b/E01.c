#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 101

/**
 * Trova il minimo tra il valore di replace, insert e delete;
 * -----------------------------
 * | replace (a)  | insert (c) |
 * | delete  (b)  | i'm here   |
 * -----------------------------
 */
int min(int a, int b, int c)
{
    if (a <= b && a <= c)
        return a;
    else if (b <= a && b <= c)
        return b;
    else
        return c;
}

/**
 * Calcola la distanza di Levenshtein tra due stringhe, cioè il numero minimo di
 * operazioni (inserimento, cancellazione, sostituzione) per trasformare una stringa nell'altra.
 */
int levenshtein_distance(char *str1, char *str2, int len1, int len2)
{
    int i, j;

    // Creazione dinamica della matrice
    int matrice[len1 + 1][len2 + 1];

    // Sistemo i valori della prima colonna
    for (i = 0; i <= len1; i++)
        matrice[i][0] = i;

    // Sistemo i valori della prima riga
    for (j = 0; j <= len2; j++)
        matrice[0][j] = j;

    for (i = 1; i <= len1; i++)
    {
        for (j = 1; j <= len2; j++)
        {
            matrice[i][j] = min(
                matrice[i - 1][j - 1] + (str1[i - 1] == str2[j - 1] ? 0 : 1), // replace
                matrice[i][j - 1] + 1,                                        // delete
                matrice[i - 1][j] + 1                                         // insert
            );
        }
    }

    return matrice[len1][len2];
}

int main()
{
    FILE *in_file = fopen("input.txt", "r");

    if (in_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        return -1;
    }

    unsigned int row = 0, len1 = 0, len2 = 0;
    char *str1 = NULL;
    char *str2 = NULL;
    int min_dist = -1;
    int n = 0;

    // Lettura del numero di row
    if (fscanf(in_file, "%d", &row) != 1)
    {
        fprintf(stderr, "Errore nella lettura di 'row'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // Lettura del numero di caratteri di len1
    if (fscanf(in_file, "%d", &len1) != 1)
    {
        fprintf(stderr, "Errore nella lettura di 'len1'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // Alloca dinamicamente esattamente len1 + 1 caratteri
    str1 = malloc(len1 + 1);
    if (fscanf(in_file, "%s", str1) != 1)
    {
        fprintf(stderr, "Errore nella lettura della sequenza str1\n");
        fclose(in_file);
        free(str1);
        exit(EXIT_FAILURE);
    }

    // Inizio calcolo distanze
    for (size_t i = 0; i < row; i++)
    {
        n = 0;

        if (fscanf(in_file, "%d", &len2) != 1)
        {
            fprintf(stderr, "Errore nella lettura di 'len2'\n");
            fclose(in_file);
            free(str1);
            exit(EXIT_FAILURE);
        }

        str2 = malloc(len2 + 1);
        if (fscanf(in_file, "%s", str2) != 1)
        {
            fprintf(stderr, "Errore nella lettura di 'str2'\n");
            fclose(in_file);
            free(str1);
            free(str2);
            exit(EXIT_FAILURE);
        }

        n = levenshtein_distance(str1, str2, len1, len2);

        // Sostituisco la distanza minima
        if (min_dist == -1 || n < min_dist)
            min_dist = (int)n;

        printf("\n%s == %s distanza attuale: %d, min_dist: %d", str1, str2, n, min_dist);

        free(str2);
    }

    printf("\n");

    free(str1);

    // Scrivo output
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        fclose(in_file);
        exit(4);
    }

    fprintf(out_file, "%d", min_dist);

    if (fclose(in_file) != 0 || fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file");
        exit(5);
    }

    return 0;
}