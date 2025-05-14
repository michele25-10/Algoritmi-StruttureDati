#include <stdlib.h>
#include <stdio.h>

/**
 *  implemento una struttura dati chiamata Fenwick Tree (o Binary Indexed Tree)
 *  estesa per supportare aggiornamenti su intervalli (range update) e query su intervalli (range query)
 *
 *  Materiale di studio:
 *      - https://www.geeksforgeeks.org/binary-indexed-tree-range-update-range-queries/
 *      - https://www.youtube.com/watch?v=uSFzHCZ4E-8
 *      - https://medium.com/carpanese/a-visual-introduction-to-fenwick-tree-89b82cac5b3c
 */

/**
 * Questa funzione aggiorna il Fenwick Tree bit
 * incrementando val in posizione idx, propagando
 * l'aggiornamento agli antenati nel tree.
 */
void update(long long *bit, int idx, long long val, int n)
{
    while (idx <= n)
    {
        // Aggiungi val al corrente nodo del tree
        bit[idx] += val;

        // idx & -idx: isola il bit meno significativo attivo di idx; aggiungendolo a idx, ci si sposta al successivo nodo che copre un intervallo che include idx
        idx += idx & -idx; // vai al parent
    }
}

/**
 * Questa funzione applica un range update: somma val a tutti gli elementi da a a b.
 * - Usa due Fenwick Tree, bit1 e bit2.
 */
void range_add(long long *bit1, long long *bit2, int a, int b, long long val, int n)
{
    update(bit1, a, val, n);      // propaga val in posizioni 1, 2, 4, ... , b
    update(bit1, b + 1, -val, n); // "stop" dopo b+1

    // Controlla l'errore di propagazione
    update(bit2, a, val * (a - 1), n);
    update(bit2, b + 1, -val * b, n);
}

/**
 * Calcola la somma prefissa da 1 a idx in un singolo BIT.
 * Viene usata sia da prefix_sum() che internamente per la logica di range_sum()
 */
long long query(long long *bit, int idx)
{
    long long sum = 0;
    while (idx > 0)
    {
        sum += bit[idx];
        idx -= idx & -idx;
    }
    return sum;
}

/**
 * Ritorna la somma da indice 1 a x dell'array virtuale, cioè quello ottenuto dagli aggiornamenti
 */
long long prefix_sum(long long *bit1, long long *bit2, int x)
{
    return query(bit1, x) * x - query(bit2, x);
}

/**
 * Ritorna la somma degli elementi nell'intervallo [a, b] facendo la differenza tra due somme prefisse
 */
long long range_sum(long long *bit1, long long *bit2, int l, int r)
{
    /**
     * [bit1 effetto cumulativo]  * x -  [bit2 correzione moltiplicativa]
     */
    return prefix_sum(bit1, bit2, r) - prefix_sum(bit1, bit2, l - 1);
}

int main()
{
    FILE *in_file = fopen("input.txt", "r");

    if (in_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        return -1;
    }

    char operation;
    int n, w, start, end, value;
    size_t i;

    // Lettura del numero di N e W
    if (fscanf(in_file, "%d %d", &n, &w) != 2)
    {
        fprintf(stderr, "Errore della lunghezza array e del numero di operazioni'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // bit1: Memorizza l'effetto diretto degli aggiornamenti su un intervallo.
    //       Quando fai update(bit1, a, +val) e update(bit1, b + 1, -val), significa:
    //       "Aggiungi val a tutti gli elementi da a a b".
    long long bit1[n + 1];

    // bit2 Serve a correggere un errore introdotto da bit1 quando calcoli la somma prefissa
    // FORMULA FINALE PER OTTENERE LA SOMMA IN UN INTERVALLO:
    //      sum[1:1:x] = bit1[x] * x − bit2[x]

    long long bit2[n + 1];
    for (i = 0; i <= n + 1; i++)
        bit1[i] = bit2[i] = 0;

    for (i = 1; i <= n; i++)
    {
        int val;
        // Lettura del numero di row
        if (fscanf(in_file, "%d", &val) != 1)
        {
            fprintf(stderr, "Errore lettura elemento i dell'array: %zu'\n", i);
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        range_add(bit1, bit2, i, i, val, n);
    }

    // Apro file di output
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        exit(EXIT_FAILURE);
    }

    // Analisi ed esecuzione delle operazioni:
    for (i = 0; i < w; i++)
    {
        if (fscanf(in_file, " %c", &operation) != 1)
        {
            fprintf(stderr, "Errore nella lettura dell'operazione'\n");
            fclose(in_file);
            fclose(out_file);
            exit(EXIT_FAILURE);
        }

        if (operation == 'A')
        {
            if (fscanf(in_file, "%d %d %d", &start, &end, &value) != 3)
            {
                fprintf(stderr, "Errore nella lettura dell'operazione'\n");
                fclose(in_file);
                fclose(out_file);
                exit(EXIT_FAILURE);
            }

            // Aggiunge +1 agli indici perché il Fenwick Tree è 1-based
            range_add(bit1, bit2, start + 1, end + 1, value, n);
        }
        else //(operation == 'Q')
        {
            if (fscanf(in_file, "%d %d", &start, &end) != 2)
            {
                fprintf(stderr, "Errore nella lettura dell'operazione'\n");
                fclose(in_file);
                fclose(out_file);
                exit(EXIT_FAILURE);
            }

            // Aggiunge +1 agli indici perché il Fenwick Tree è 1-based
            long long result = range_sum(bit1, bit2, start + 1, end + 1);
            fprintf(out_file, "%lld\n", result);
        }
    }

    // Chiudo file di input
    if (fclose(in_file) != 0 || fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file di input");
        exit(EXIT_FAILURE);
    }

    return 0;
}