#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// numero massimo di nodi
#define MAX_N 1000000
// attenzione ai pesi: non ci basta int
#define ull unsigned long long int

// DEFINIZIONE GRAFO (come lista di adiacenza)
// considera una lista di adiacenza L che mappa un certo nodo ai suoi vicini
// (che sono proprio i nodi di una lista);
// per ogni vicino dobbiamo sapere:
typedef struct adj_list_node
{
    // chi è
    int dest;

    // è un mulinello?
    int is_train;

    // quanto pesa l'arco per arrivarci
    ull weight;

    // poi ci servono i riferimenti agli altri vicini
    struct adj_list_node *next;
} adj_list_node;

// crea un nodo
adj_list_node *new_adj_list_node(int dest, ull weight, int is_train)
{
    adj_list_node *new_node = (adj_list_node *)malloc(sizeof(adj_list_node));

    new_node->dest = dest;
    new_node->is_train = is_train;
    new_node->weight = weight;
    new_node->next = NULL;

    return new_node;
}

// definizione del grafo
typedef struct adj_list
{
    adj_list_node *heads[MAX_N];
} adj_list;

// crea il grafo come lista di adiacenza
adj_list *init_adj_list()
{
    adj_list *graph = (adj_list *)malloc(sizeof(adj_list));

    for (int i = 0; i < MAX_N; i++)
    {
        graph->heads[i] = NULL;
    }

    return graph;
}

// inserisci un arco (u,v) con peso w nel grafo
void insert_edge(adj_list *graph, int u, int v, ull w, int is_train)
{
    adj_list_node *new_node = new_adj_list_node(v, w, is_train);

    new_node->next = graph->heads[u];
    graph->heads[u] = new_node;
}

// elimina il grafo in memoria
void free_graph(adj_list *graph)
{
    for (int i = 0; i < MAX_N; i++)
    {
        adj_list_node *node = graph->heads[i];

        while (node)
        {
            adj_list_node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(graph);
}

// DEFINIZIONE HEAP

// per ogni elemento nella heap, dobbiamo sapere:
typedef struct min_heap_node
{
    // chi è
    int v;

    // qual è la chiave associata; in questo ambito non è male chiamarla "peso"
    ull weight;
} min_heap_node;

// crea un nuovo elemento nella heap (sarà una min heap, ma in realtà per ora non importa)
min_heap_node *init_min_heap_node(int v, ull weight)
{
    min_heap_node *heap_node = (min_heap_node *)malloc(sizeof(min_heap_node));

    heap_node->v = v;
    heap_node->weight = weight;

    return heap_node;
}

// definizione della heap
typedef struct min_heap
{
    // numero elementi contenuti nella heap
    int size;

    // array per mappare l'id di un nodo alla sua posizione nella heap
    int *pos;

    // questa è l'effettiva heap, contenente un array di puntatori a "nodi heap" (vedi sopra)
    min_heap_node **array;
} min_heap;

// crea una min heap
min_heap *init_min_heap()
{
    min_heap *heap = (min_heap *)malloc(sizeof(min_heap));

    heap->pos = (int *)malloc(MAX_N * sizeof(int));
    heap->size = 0;

    heap->array = (min_heap_node **)malloc(MAX_N * sizeof(min_heap_node *));

    return heap;
}

// piccola uility per scambiare di posizione due elementi della heap
void swap_min_heap_nodes(min_heap_node **a, min_heap_node **b)
{
    min_heap_node *temp = *a;
    *a = *b;
    *b = temp;
}

// garantisce che la heap sia min heap
void min_heapify(min_heap *heap, int idx)
{
    int smallest = idx;
    int left = idx * 2 + 1;
    int right = idx * 2 + 2;

    if (left < heap->size && heap->array[left]->weight < heap->array[smallest]->weight)
    {
        smallest = left;
    }

    if (right < heap->size && heap->array[right]->weight < heap->array[smallest]->weight)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        min_heap_node *smallest_node = heap->array[smallest];
        min_heap_node *idx_node = heap->array[idx];

        heap->pos[smallest_node->v] = idx;
        heap->pos[idx_node->v] = smallest;

        swap_min_heap_nodes(&heap->array[smallest], &heap->array[idx]);
        min_heapify(heap, smallest);
    }
}

// estrai il minimo dalla heap e riarrangia i nodi mantenendo la proprietà di min heap
min_heap_node *extract_min(min_heap *heap)
{
    if (heap->size == 0)
    {
        return NULL;
    }

    min_heap_node *root = heap->array[0];
    min_heap_node *last_node = heap->array[heap->size - 1];

    heap->array[0] = last_node;
    heap->pos[root->v] = heap->size - 1;
    heap->pos[last_node->v] = 0;

    --heap->size;

    min_heapify(heap, 0);

    return root;
}

// aggiorna una chiave nella heap (preferisco usare weight anziché key)
void decrease_key(min_heap *heap, int v, ull weight)
{
    int i = heap->pos[v];
    heap->array[i]->weight = weight;

    while (i && heap->array[i]->weight < heap->array[(i - 1) / 2]->weight)
    {
        int parent_i = (i - 1) / 2;

        heap->pos[heap->array[i]->v] = parent_i;
        heap->pos[heap->array[parent_i]->v] = i;
        swap_min_heap_nodes(&heap->array[i], &heap->array[parent_i]);
        i = parent_i;
    }
}

// LOGICA PRINCIPALE
void dijkstra(adj_list *graph, int src, int *relaxed)
{
    //  tengo traccia della distanza migliore che ho impiegato per raggiungere un nodo del grafo
    ull dist[MAX_N];

    // la coda di priorità è vuota all'inizio
    min_heap *heap = init_min_heap();

    // per ogni nodo, ipotizzo che la distanza per raggiungerlo sia non abbordabile
    // (non potrà che migliorare); in più, lo aggiungo alla heap.
    for (int i = 0; i < MAX_N; i++)
    {
        dist[i] = ULLONG_MAX;
        heap->array[i] = init_min_heap_node(i, dist[i]);
        heap->pos[i] = i;
    }

    // il nodo di partenza è un'eccezione: la distanza per raggiungerlo è 0 ed è pertanto
    // la radice della heap.
    dist[src] = 0;
    heap->array[src] = init_min_heap_node(src, dist[src]);
    heap->pos[src] = src;
    heap->size = MAX_N;

    decrease_key(heap, src, dist[src]);

    // fin tanto che ho nodi nella heap, estraggo l'elemento più promettente (la radice);
    // ora mi chiedo: tra i miei vicini, chi conviene visitare?
    while (heap->size)
    {
        min_heap_node *heap_node = extract_min(heap);

        int u = heap_node->v;
        adj_list_node *neighbor = graph->heads[u];

        while (neighbor)
        {
            int v = neighbor->dest;
            ull weight = neighbor->weight;
            int is_train = neighbor->is_train;

            // Non è detto che convenga visitare un vicino "v", può darsi che si sia già
            // trovato un percorso in precedenza che rende "dist[v]" più conveniente
            // rispetto al passaggio per "u".
            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                decrease_key(heap, v, dist[v]);

                // aggiorna relaxed: 1 se arrivato con un mulinello, 0 altrimenti
                relaxed[v] = is_train;
            }
            else if (dist[u] + weight == dist[v])
            {
                // stesso tempo: se arrivo ora con un arco normale e prima era un mulinello, lo sovrascrivo
                if (relaxed[v] == 1 && is_train == 0)
                {
                    relaxed[v] = 0;
                }
            }

            neighbor = neighbor->next;
        }
    }

    free(heap->pos);
    free(heap->array);
    free(heap);
}

// DRIVER
void solve(FILE *input_file, FILE *output_file)
{
    int N, M, K;
    adj_list *graph = init_adj_list();

    if (fscanf(input_file, "%d %d %d", &N, &M, &K) != 3)
    {
        fprintf(stderr, "[ERROR] Impossibile leggere N, M, K\n");
        exit(1);
    }

    int *relaxed = malloc(MAX_N * sizeof(int));
    if (!relaxed)
    {
        printf("[ERROR] malloc fallita per relaxed\n");
        exit(1);
    }

    for (int i = 0; i < M; i++)
    {
        int u, v;
        ull w;

        if (fscanf(input_file, "%d %d %llu", &u, &v, &w) != 3)
        {
            fprintf(stderr, "[ERROR] Errore lettura arco %d\n", i);
            exit(1);
        }

        insert_edge(graph, u, v, w, 0);
        insert_edge(graph, v, u, w, 0);
        relaxed[i] = 0;
    }

    for (int i = 0; i < K; i++)
    {
        int s;
        ull w;

        if (fscanf(input_file, "%d %llu", &s, &w) != 2)
        {
            fprintf(stderr, "[ERROR] Errore lettura shortcut %d\n", i);
            exit(1);
        }

        insert_edge(graph, 0, s, w, 1);
        insert_edge(graph, s, 0, w, 1);
    }

    // sorgente sempre nodo 0
    dijkstra(graph, 0, relaxed);

    // Dobbiamo contare quanti dei K archi non vengono utilizzati
    // se l'arco k non viene utilizzato, è sempre inutile raggiungere la sua destinazione.
    int useful_trains = 0;
    for (int i = 0; i < N; i++)
        if (relaxed[i])
            useful_trains++;

    fprintf(output_file, "%d", K - useful_trains);

    free_graph(graph);
}

int main(void)
{
    FILE *input_file, *output_file;
    input_file = fopen("input.txt", "r");
    output_file = fopen("output.txt", "w");

    if (input_file == NULL || output_file == NULL)
    {
        fprintf(stdout, "IO Error!");
        return -1;
    }

    solve(input_file, output_file);

    if (fclose(input_file) != 0 || fclose(output_file) != 0)
    {
        fprintf(stdout, "IO Error!");
        return -1;
    }

    return 0;
}