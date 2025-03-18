#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struct configurazione: max_size, min_size, step, repetitions, seed
typedef struct
{
    int max_size;
    int min_size;
    int step;
    int repetitions;
    int seed;
} configuration;

/**
 * Procedura interna a merge_sort
 */
void merge(int arr[], int start, int mid, int end)
{
    int n1 = mid - start + 1;
    int n2 = end - mid;

    int L[n1];
    int R[n2];

    // array L[start:mid]
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[start + i];
    }

    // array R[mid+1:end]
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + j + 1];
    }

    int i = 0;
    int j = 0;

    // Essendo L e R già ordinati posso andare ad eseguire il confronto
    for (int k = start; k <= end; k++)
    {
        if (i < n1)
        {
            if (j < n2)
            {
                if (L[i] <= R[j])
                {
                    arr[k] = L[i];
                    ++i;
                }
                else
                {
                    arr[k] = R[j];
                    ++j;
                }
            }
            else
            {
                arr[k] = L[i];
                ++i;
            }
        }
        else
        {
            arr[k] = R[j];
            ++j;
        }
    }
}
/**
 * Ordina il mio array
 */
void mergeSort(int arr[], int start, int end)
{
    // Se l'array contiene almeno due elementi
    if (start < end)
    {
        int mid = start + (end - start) / 2;
        // richiama ricorsivamente sulle due metà dell'array
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);

        // dopo la chiamata, le due metà sono ordinate e
        // possono essere unite con una procedura "merge"
        merge(arr, start, mid, end);
    }
}

/**
 * controlla che l'ordinamento sia avvenuto correttamente e in caso contrario restituisci un errore
 */
void checkSort(int arr[], int size)
{
    printf("\n\nSto controllando se sono ordianti {array size: %d}....", size);
    for (int k = 1; k < size; k++)
    {
        // printf("\n%d", arr[k - 1]);
        if (arr[k - 1] > arr[k])
        {
            printf("\nNON è ordinato\n");
            exit(-1);
        }
    }
    // printf("\n%d", arr[size - 1]);
}

/**
 * Fa partire n test di un algortimo e restituisce il suo tempo medio
 * per ogni risultato inserisce i dati in un file csv
 */
double run(int size, int repetitions)
{
    double time = 0.0;
    // itera da 0 a repetitions
    for (int i = 0; i < repetitions; i++)
    {
        // per ogni ripetizione crea un array arr di dimensione size e inizializzalo usando rand
        int arr[size];
        for (int k = 0; k < size; k++)
        {
            arr[k] = rand() % 101; // Genera un numero tra 0 e 200
        }

        // esegui l'algoritmo di ordinamento su arr e misura il tempo di esecuzione, accumulandolo in una variabile
        clock_t start = clock();
        mergeSort(arr, 0, size - 1);
        clock_t stop = clock();

        // Controllo se l'array è realmente ordinato
        checkSort(arr, size);

        double elapsed_time = (double)(stop - start) / CLOCKS_PER_SEC;
        printf("\nelapsedTime: %f", elapsed_time);
        time += elapsed_time;
    }

    // ritorna al chiamante il tempo medio di esecuzione
    return time / repetitions;
}

/**
 * Manager degli esperimenti
 */
void run_experiments(configuration config)
{
    // Scrivo su file i risultati
    FILE *fd = fopen("mergeSortResult.csv", "w");
    fprintf(fd, "size,elapsed_time\n");

    // itera da min_size a max_size con passo step
    for (int size = config.min_size; size <= config.max_size; size += config.step)
    {
        // inizializza srand con seed
        srand(config.seed);

        // esegui run con la dimensione specificata, e salva il tempo ottenuto in una variabile
        double time = run(size, config.repetitions);

        // stampa il tempo ottenuto
        printf("\nArray size %d time: %d", size, time);
        fprintf(fd, "%d,%f\n", size, time);
        fflush(fd);
        // incrementa di 1 seed
        config.seed++;
    }
    fclose(fd);
}

int main(void)
{
    // Ottieni la parametrizzazione degli esperimenti, in input
    configuration config;
    config.max_size = 5000;
    config.min_size = 10;
    config.step = 25;
    config.repetitions = 10;
    config.seed = 250;

    // esegui gli esperimenti chiamando run_experiments e termina
    run_experiments(config);

    return 0;
}