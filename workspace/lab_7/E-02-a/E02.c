#include <stdlib.h>
#include <stdio.h>

/**
 * return del numero di pugili che fanno almeno un incontro
 */
int get_number_active_boxer(int *array, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (array[i] == 1)
            count++;
    }
    return count;
}

/**
 * Array: 0, 1, 2, 3
 *
 * Indici corrispondente all'ID
 *
 * arr[id_boxer] = 1 //Allora ha fatto almeno un match
 * arr[id_boxer] = 0 //Allora non ha fatto nessun match
 */
void add_boxer_if_absent(int *array, int boxer)
{
    if (array[boxer] == 0)
        array[boxer] = 1;
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

    // Creo l'array, visualizzando gli esempi gli indici dell'array possono essere ugauli agli id dei pugili
    int active_boxer[n_boxer];

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

        if (avv1 > n_boxer || avv2 > n_boxer)
        {
            fprintf(stderr, "Id pugile fuori dal range definito dal file\n");
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        add_boxer_if_absent(active_boxer, avv1);
        add_boxer_if_absent(active_boxer, avv2);
    }

    // Chiudo file di input
    if (fclose(in_file) != 0)
    {
        printf("Errore durante la chiusura del file di input");
        exit(EXIT_FAILURE);
    }

    // conto il numero di pugili che fanno almeno un match
    int n_active_boxer = get_number_active_boxer(active_boxer, n_boxer);

    // calcolo il numero totale di match da fare per far si che tutti i
    // pugili si siano scontrati tra loro almeno una volta
    //      tot = n(n-1)/2
    int full_match_number = (n_active_boxer * (n_active_boxer - 1)) / 2;

    // Apro file di output
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        exit(EXIT_FAILURE);
    }

    // Scrivo nel file output il risultato finale
    if (full_match_number == n_match)
        fprintf(out_file, "FALSE"); // se si sono già scontrati tutti tra loro non posso creare gruppi
    else
        fprintf(out_file, "TRUE"); // altrimenti posso creare i due gruppi

    if (fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file output");
        exit(EXIT_FAILURE);
    }

    return 0;
}