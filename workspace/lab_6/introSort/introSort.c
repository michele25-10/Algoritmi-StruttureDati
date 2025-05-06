#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HYBRID_QS_K 55

// struct configurazione: max_size, min_size, step, repetitions, seed
typedef struct
{
    int max_size;
    int min_size;
    int step;
    int repetitions;
    int seed;
} configuration;

void swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void max_heapify(int *arr, int n, int root)
{
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    // confronto tra radice e elemento sinistro
    if (left < n && arr[left] > arr[root])
    {
        largest = left;
    }

    // confronto tra radice e elmento destro
    if (right < n && arr[right] < arr[root])
    {
        largest = right;
    }

    // Se la radice ha dei figli più grandi
    // scambiala e richiama max_heapify
    if (largest != root)
    {
        swap(arr, largest, root);
        max_heapify(arr, n, largest);
    }
}

void heap_sort(int *arr, int start, int end)
{
    int n = end - start + 1;

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        // BuildMaxHeap
        // prossima slide
        // ne’ la dimensione dell'array;
        // i e’ l’indice della radice.
        max_heapify(arr, n, i);
    }

    // Estraila radice della heap n−1 volte
    // riapplicando heapify ogni volta .
    for (int i = n - 1; i > 0; i--)
    {
        swap(arr, start, start + i);
        // MaxHeapify dove l'indice 0 e’ la radice della heap
        // e i la dimensione che si sta riducendo.
        max_heapify(arr, i, 0);
    }
}

void insertionSort(int arr[], int size)
{
    for (int j = 1; j < size; j++)
    {
        int key = arr[j];
        int k = j - 1;

        while ((k >= 0) && (arr[k] > key))
        {
            arr[k + 1] = arr[k];
            k -= 1;
        }
        arr[k + 1] = key;
    }
}

int partition(int *arr, int start, int end)
{
    int pivot = arr[end];
    int i = start - 1;

    for (int j = start; j < end; j++)
    {
        if (arr[j] <= pivot)
        {
            swap(arr, ++i, j);
        }
    }

    swap(arr, ++i, end);
    return i;
}

int medianOfThree(int *arr, int a, int b, int c)
{
    if (arr[a] > arr[b])
    {
        if (arr[b] > arr[c])
            return b;
        else if (arr[a] < arr[c])
            return a;
        else
            return c;
    }
    else
    {
        if (arr[a] > arr[c])
            return a;
        else if (arr[b] < arr[c])
            return b;
        else
            return c;
    }
}

int motPartition(int *arr, int start, int end)
{
    // Cerca l'elemento mediano tra start, end e il loro centro (mid)
    int median = medianOfThree(
        arr, start, end, start + (end - start) / 2);

    // Scambia end con mid
    swap(arr, median, end);

    // Ritorna con partition
    return partition(arr, start, end);
}

void tailQuickSort(int arr[], int start, int end)
{
    if (end - start <= HYBRID_QS_K)
    {
        insertionSort(arr, end + 1);
        return;
    }

    // fintanto che start è minore di end
    while (start < end)
    {
        // ottieni il pivot con partition tra start ed end
        int q = motPartition(arr, start, end);

        // se la parte dell'array a sinistra del pivot è più lunga della parte destra
        if ((q - start) > (end - q))
        {
            // ordina la parte dell'array a sinistra del pivot
            tailQuickSort(arr, start, q - 1);
            // start diventa la posizione subito successiva al pivot
            start = q + 1;
        }
        else
        {
            // Ordina la parte dell'array a destra del pivot
            tailQuickSort(arr, q + 1, end);
            // end diventa la posizione subito precedente al pivot
            end = q - 1;
        }
    }
}

void _intro_sort(int *arr, int start, int end, int max_dep)
{
    // Switch ad InsertionSort
    // La costante si ottiene come nel caso di HybridMergeSort
    if (end - start <= HYBRID_QS_K)
    {
        insertion_sort(arr, start, end);
    }
    // switch a heapSort per evitare altre chiamate ricorsive
    else if (max_dep == 0)
    {
        heap_sort(arr, start, end);
    }
    // Logica del quickSort ottimizzato ma in
    // cui ogni chiamata viene sostituita con una ad _intro_sort
    else
    {
        tailQuickSort(arr, 0, start - end)
    }
}

// Fissa una soglia di chiamate ricorsive massime
// Poi chiama "il vero" IntroSort
void intro_sort(int *arr, int start, int end)
{
    return _intro_sort(arr, start, end, 2 * log(2 * (end - start + 1)));
}

/**
 * Sottoprocedura principale di `quick_sort`.
 */
int partition(int *arr, int start, int end)
{
    int pivot = arr[end];
    int i = start - 1;

    for (int j = start; j < end; j++)
    {
        if (arr[j] <= pivot)
        {
            swap(arr, ++i, j);
        }
    }

    swap(arr, ++i, end);
    return i;
}

int medianOfThree(int *arr, int a, int b, int c)
{
    if (arr[a] > arr[b])
    {
        if (arr[b] > arr[c])
            return b;
        else if (arr[a] < arr[c])
            return a;
        else
            return c;
    }
    else
    {
        if (arr[a] > arr[c])
            return a;
        else if (arr[b] < arr[c])
            return b;
        else
            return c;
    }
}

int motPartition(int *arr, int start, int end)
{
    // Cerca l'elemento mediano tra start, end e il loro centro (mid)
    int median = medianOfThree(
        arr, start, end, start + (end - start) / 2);

    // Scambia end con mid
    swap(arr, median, end);

    // Ritorna con partition
    return partition(arr, start, end);
}

/**
 * Algoritmo di ordinamento quicSort
 */
void tailQuickSort(int arr[], int start, int end)
{
    if (end - start <= HYBRID_QS_K)
    {
        insertionSort(arr, end + 1);
        return;
    }

    // fintanto che start è minore di end
    while (start < end)
    {
        // ottieni il pivot con partition tra start ed end
        int q = motPartition(arr, start, end);

        // se la parte dell'array a sinistra del pivot è più lunga della parte destra
        if ((q - start) > (end - q))
        {
            // ordina la parte dell'array a sinistra del pivot
            tailQuickSort(arr, start, q - 1);
            // start diventa la posizione subito successiva al pivot
            start = q + 1;
        }
        else
        {
            // Ordina la parte dell'array a destra del pivot
            tailQuickSort(arr, q + 1, end);
            // end diventa la posizione subito precedente al pivot
            end = q - 1;
        }
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
        tailQuickSort(arr, 0, size - 1);
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
    FILE *fd = fopen("introSort.csv", "w");
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