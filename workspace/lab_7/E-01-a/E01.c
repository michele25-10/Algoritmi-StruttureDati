#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 101

/**
 * Funzione che elimina o inserisce lettere in modo intelligente:
 * Esegue una ricerca di un pattern di almeno due caratteri presente sia in str1 che in str2
 * Quando lo trova se il pattern in str2 è:
 *  - Spostato verso destra rispetto a str1 eseguo una eliminazione intelligente fino a che la posizione del pattern combacia con quella in str1
 *  - Spostato verso sinistra rispetto a str1 eseguo un inserimento intelligente fino a che la posizione del pattern combacia con quella in str1
 *
 * Esegue il return del numero di inserimenti, modifiche, eliminazioni effettuate.
 */
size_t intelligent_delete_or_insert(const char *str1, char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    unsigned int count = 0;

    for (size_t i = 0; i < len2; i++)
    {
        for (size_t j = 0; j < len1; j++)
        {
            // Eseguo il naive pattern matching
            if (str2[i] == str1[j])
            {
                int unsigned k = 1;
                while (i + k < len2 && j + k < len1 && str2[i + k] == str1[j + k])
                {
                    k++;
                }

                printf("\t%d %zu %zu", k, i, j);

                if (k >= 2 && i != j)
                {
                    /*
                    Se i è maggiore di j vuol dire che in str2 il pattern è spostato di k posizioni a destra (ELIMINAZIONE INTELLIGENTE)
                    Se i è minored di j vuol dire che in str2 il pattern è spostato di k posizioni a sinistra (INSERIMENTO INTELLIGENTE)
                    */
                    if (i > j) // Eliminazione intelligente
                    {
                        // Elimino le lettere antecedenti al pattern trovato
                        size_t shift = i - j;
                        for (int l = i; l < len2; l++)
                        {
                            str2[l - shift] = str2[l];
                        }
                        // Modifico lunghezza str2 e conto il numero di eliminazioni
                        len2 -= shift;
                        str2[len2] = '\0';
                        count += shift;
                        break;
                    }
                    else // Inserimento intelligente
                    {
                        size_t shift = j - i;
                        // Sposta in avanti per fare spazio
                        for (int l = len2 - 1; l >= i + k; l--)
                        {
                            str2[l + shift] = str2[l];
                        }

                        // Copia i caratteri da str1
                        for (int l = len2 - 1; l >= i + k; l--)
                        {
                            str2[l + shift] = str2[l];
                        }

                        len2 += shift;
                        str2[len2] = '\0';
                        count += shift;
                    }
                }
            }
        }
    }

    return count;
}

/**
 * Rimuove le lettere extra in str2 fino a raggiungere la stessa dimensione di str1, quindi:
 * Fino a che strlen(str1) == strlen(str2)
 *
 * Esegue il return del numero di eliminazioni effettuate
 */
size_t delete_extra_letters(const char *str1, char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 > len2)
    {
        fprintf(stderr, "delete_extra_letters: errore - str1 più lunga di str2\n");
        exit(3);
    }

    if (len2 > len1)
    {
        str2[len1] = '\0';
        return len2 - len1;
    }

    return 0; // Nesuna modifica necessaria
}

/**
 * Inserisce le lettere extra in str2 fino a raggiungere la stessa dimensione di str1, quindi:
 * Fino a che strlen(str1) == strlen(str2)
 *
 * Esegue il return del numero di inserimenti effettuati.
 */
size_t insert_extra_letters(const char *str1, char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 < len2)
    {
        fprintf(stderr, "insert_extra_letters: errore - str1 più corta di str2\n");
        exit(2);
    }

    size_t count = 0;
    for (size_t i = len2; i < len1; i++)
    {
        str2[i] = str1[i];
        count++;
    }
    str2[len1] = '\0';

    return count;
}

/**
 * Funzione che modifica le lettere in modo opportuno fino a rendere str1 e str2 uguali.asm
 * Il prerequisito è che strlen(str1) == strlen(str2)
 *
 * Esegue il return del numero di modifiche effettuate.
 */
size_t transform_strings_into_equals(const char *str1, char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 != len2)
    {
        fprintf(stderr, "transform_strings_into_equals: errore - le stringhe non hanno la stessa lunghezza\n");
        exit(1);
    }

    size_t count = 0;
    for (size_t i = 0; i < len1; i++)
    {
        if (str1[i] != str2[i])
        {
            str2[i] = str1[i];
            count++;
        }
    }

    return count;
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
    char str2[MAX_LEN];
    int min_dist = -1;
    size_t n = 0;

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
    str1[len1] = '\0';

    // Inizio calcolo distanze
    for (size_t i = 0; i < row; i++)
    {
        n = 0;

        if (fscanf(in_file, "%d", &len2) != 1)
        {
            fprintf(stderr, "Errore nella lettura di 'len2'\n");
            exit(EXIT_FAILURE);
        }

        if (fscanf(in_file, "%s", str2) != 1)
        {
            fprintf(stderr, "Errore nella lettura di 'str2'\n");
            exit(EXIT_FAILURE);
        }
        str2[len2] = '\0';

        printf("\nStringa1: %s; Stringa2: %s\t\t", str1, str2);

        if (len1 != strlen(str2))
        {
            n += intelligent_delete_or_insert(str1, str2);

            // Nel caso in cui io avessi già superato la minima distanza vado all'iterazione successiva
            if (n >= min_dist && min_dist != -1)
                continue;

            if (len1 > strlen(str2)) // Aggiungo le lettere mancanti
                n += insert_extra_letters(str1, str2);
            else if (len1 < strlen(str2)) // Elimino le lettere extra
                n += delete_extra_letters(str1, str2);

            // Nel caso in cui io avessi già superato la minima distanza vado all'iterazione successiva
            if (n >= min_dist && min_dist != -1)
                continue;
        }

        n += transform_strings_into_equals(str1, str2);

        printf("%d, %zu\t\t", min_dist, n);

        // Sostituisco la distanza minima
        if (min_dist == -1 || n < min_dist)
            min_dist = (int)n;

        printf("%s == %s distanza: %d", str1, str2, min_dist);
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