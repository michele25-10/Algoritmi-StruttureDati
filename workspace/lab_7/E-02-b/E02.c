#include <stdlib.h>
#include <stdio.h>

/**
 *     | 0 | 1 | 2
 *     ------------
 *   0 | 0   1   1
 *   1 | 1   0   0
 *   2 | 1   1   0
 *
 * Indici corrispondente all'ID
 *
 * Setto il match tra i due avversari:
 * arr[avv1][avv2] = 1
 * arr[avv2][avv1] = 1
 */
void add_match(short unsigned int **match, int id_boxer1, int id_boxer2)
{
    match[id_boxer1][id_boxer2] = 1;
    match[id_boxer2][id_boxer1] = 1;
}
/**
 * Dopo aver contato i pugili attivi controllo che tutti i boxer si scontrino tra di loro:
 * - se è così restituisco 0 (non posso creare gruppi di pugili)
 * - se non è così restituisco 1 (posso creare gruppi di pugili)
 */
unsigned short int can_create_group_of_boxer(short unsigned int **match, int len)
{
    // Conto i pugili attivi
    size_t i, j;
    int n_active_boxer = 0;
    for (i = 0; i < len; i++)
        for (j = 0; j < len; j++)
            if (match[i][j] == 1)
            {
                n_active_boxer++;
                break;
            }

    for (int i = 0; i < len; i++)
    {
        int count = 0;
        for (int j = 0; j < len; j++)
            if (i != j && match[i][j] == 1)
                count++;

        // Se è un pugile attivo e se non si è scontrato con tutti gli altri pugili allora potrò creare gruppetti
        if (count != 0 && count != n_active_boxer - 1)
            return 1;
    }
    // altrimenti significa che tutti si sono scontrati tra loro, di conseguenza non posso creare gruppetti
    return 0;
}

int main()
{
    FILE *in_file = fopen("input.txt", "r");

    if (in_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        return -1;
    }

    unsigned int n_boxer = 0, n_match = 0;
    int avv1 = 0, avv2 = 0;

    // Lettura del numero di row
    if (fscanf(in_file, "%d", &n_boxer) != 1)
    {
        fprintf(stderr, "Errore nella lettura del numero di pugili'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // Creo la matrice con tutti i match
    short unsigned int **match = malloc(n_boxer * sizeof(short unsigned int *));
    if (!match)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_boxer; i++)
    {
        match[i] = malloc(n_boxer * sizeof(short unsigned int));
        if (!match[i])
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
    }

    // inizializzo la matrice match settata tutta a 0
    for (int i = 0; i < n_boxer; i++)
        for (int j = 0; j < n_boxer; j++)
            match[i][j] = 0;

    // Lettura del numero di caratteri di len1
    if (fscanf(in_file, "%d", &n_match) != 1)
    {
        fprintf(stderr, "Errore nella lettura del numero di match'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // Inserisco i pugili che fanno almeno un match nella lista
    for (size_t i = 0; i < n_match; i++)
    {
        if (fscanf(in_file, "%d %d", &avv1, &avv2) != 2)
        {
            fprintf(stderr, "Errore nella lettura di 'avv1' o 'avv2'\n");
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        if (avv1 > n_boxer || avv2 > n_boxer || avv1 == avv2)
        {
            fprintf(stderr, "Id pugile fuori dal range definito dal file\n");
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        add_match(match, avv1, avv2);
    }

    // Chiudo file di input
    if (fclose(in_file) != 0)
    {
        printf("Errore durante la chiusura del file di input");
        exit(EXIT_FAILURE);
    }

    // Apro file di output
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        exit(EXIT_FAILURE);
    }

    // Eseguo il controllo se posso creare gruppi di boxer
    if (can_create_group_of_boxer(match, n_boxer))
        fprintf(out_file, "TRUE"); // se posso creare i due gruppi allora rispondo TRUE
    else
        fprintf(out_file, "FALSE"); // se si sono già scontrati tutti tra loro rispondo FALSE

    if (fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file output");
        exit(EXIT_FAILURE);
    }

    return 0;
}