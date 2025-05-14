#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/**
 *  Soluzione inserisco tutti i mulinelli in un array dal momento che so che portano tutti i nodi da sorgente a un punto 0,1,....,n
 *  se il mulinello è presente allora avrò il costo all'interno dell'array altrimenti avrò INF (simile ad infinito)
 *
 *  Creo la lista di adiacenza del mio grafo
 *
 *  Con dijikstra calcolo il peso per andare in ogni nodo del mio grafo (Per farlo mi appoggio alla struttura dati min-heap avente come key il weight per raggiungere il nodo dest e come indice dest stesso)
 *  Questo mi consente di ottimizzare l'algoritmo.
 *
 *  Infine scorro l'array delle distanze e quello dei mulinelli tutte le volte che un mulinello costa di più deegli altri archi allora faccio un count++
 *
 *  Infine inserisco il valore del conteggio nel file output.txt
 */

#define MAX_NODES 1000000
#define INF INT_MAX

// GRAPH
typedef struct Node
{
    int dest;
    int weight;
    struct Node *next;
} Node;

typedef struct
{
    int num_nodes;
    Node *adj[MAX_NODES];
} Graph;

// MINHEAP
typedef struct
{
    int dest;
    int weight;
} MinHeapNode;

typedef struct
{
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
} MinHeap;

/**
 * Alloca una MinHeap avente dimensione capacity
 */
MinHeap *createMinHeap(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->pos = (int *)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (MinHeapNode **)malloc(capacity * sizeof(MinHeapNode *));
    return heap;
}

/**
 * Aggiunge un nodo alla minHeap
 */
MinHeapNode *newMinHeapNode(int dest, int weight)
{
    MinHeapNode *node = (MinHeapNode *)malloc(sizeof(MinHeapNode));
    node->dest = dest;
    node->weight = weight;
    return node;
}

/**
 * Effettua lo swap di due nodi all'interno di una minHeap
 */
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b)
{
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

/**
 * Serve a mantenere la proprietà di Min Heap (cioè che il nodo padre è sempre più piccolo o uguale ai suoi figli) dopo una modifica nella heap
 */
void minHeapify(MinHeap *heap, int idx)
{
    int smallest = idx;      // il nodo corrente sia il più piccolo
    int left = 2 * idx + 1;  // indice del figlio sinistro nel min heap
    int right = 2 * idx + 2; // indice del figlio destro nel min heap

    // Se il figlio sinistro esiste e se il valore weight è minore di quello del nodo corrente (smallest)
    if (left < heap->size &&
        heap->array[left]->weight < heap->array[smallest]->weight)
        smallest = left; // aggiorna smallest con left

    // Se il figlio destro esiste e se il valore weight è minore di quello del nodo corrente (smallest)
    if (right < heap->size &&
        heap->array[right]->weight < heap->array[smallest]->weight)
        smallest = right; // aggiorna smallest con right

    //  Se è stato trovato un figlio con weight più piccolo
    if (smallest != idx)
    {
        // Memorizzo i due nodi coinvolti
        MinHeapNode *smallestNode = heap->array[smallest];
        MinHeapNode *idxNode = heap->array[idx];

        // La posizione di ciascun nodo (v) nella heap viene aggiornata nel vettore pos[]
        heap->pos[smallestNode->dest] = idx;
        heap->pos[idxNode->dest] = smallest;

        // Eseguo lo swap dei due nodi
        swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);

        // Chiamata ricorsiva
        minHeapify(heap, smallest);
    }
}

/**
 * Controlla se l'heap è stata svuotata
 */
int isEmpty(MinHeap *heap)
{
    return heap->size == 0;
}

/**
 * Estraggo il nodo dest avente il costo minimo e presente nel minheap (l'elemento in cima)
 */
MinHeapNode *extractMin(MinHeap *heap)
{
    // Se è vuoto non ho un minimo
    if (isEmpty(heap))
        return NULL;

    MinHeapNode *root = heap->array[0]; // Il nodo in cima (indice 0) è il minimo (root)

    MinHeapNode *lastNode = heap->array[heap->size - 1]; // Ultimo nodo nell’array heap (heap è completa fino a size-1)
    heap->array[0] = lastNode;                           // Sposto l’ultimo nodo in cima (indice 0)

    // Aggiorno le posizioni nel vettore `pos` usato per l'accesso rapido
    heap->pos[root->dest] = heap->size - 1; // Dico che il nodo estratto ora è "fuori" dalla heap (sarà scartato)
    heap->pos[lastNode->dest] = 0;          // Il nuovo nodo in cima è ora il `lastNode`

    --heap->size; // Riduco la dimensione della heap (effettivamente rimuovo root)

    minHeapify(heap, 0); // Riordino la heap a partire dalla radice (indice 0) per mantenere proprietà min-heap

    return root; // Ritorno il nodo minimo originario (quello che era in cima)
}

/**
 * Decrementare la distanza di un nodo
 */
void decreaseKey(MinHeap *heap, int dest, int weight)
{
    // prendo la posizione della mia key
    int i = heap->pos[dest];

    heap->array[i]->weight = weight; // Aggiorno il valore della distanza del nodo v

    // "Risalgo" la heap spostando il nodo verso l’alto finché la proprietà di min-heap è rispettata
    while (i && heap->array[i]->weight < heap->array[(i - 1) / 2]->weight)
    {
        // Aggiorno la posizione del nodo corrente e del suo genitore nel vettore pos[]
        heap->pos[heap->array[i]->dest] = (i - 1) / 2; // Nuova posizione del nodo corrente
        heap->pos[heap->array[(i - 1) / 2]->dest] = i; // Nuova posizione del genitore

        // Scambio i due nodi nel min-heap
        swapMinHeapNode(&heap->array[i], &heap->array[(i - 1) / 2]);

        // Aggiorno l’indice corrente e continuo a salire nell’albero
        i = (i - 1) / 2;
    }
}
/**
 * Controlla se l'elemento è presente nell'heap
 */
int isInMinHeap(MinHeap *heap, int dest)
{
    return heap->pos[dest] < heap->size;
}

/**
 * Aggiunge un arco al grafo
 */
void add_edge(Graph *g, int u, int dest, int w)
{
    // Se la coppia è già presente come collegamento allora la ignoro
    Node *curr = g->adj[u];
    while (curr != NULL)
    {
        if (curr->dest == dest)
            return;
        curr = curr->next;
    }

    // Alloco il nuovo nodo e lo inserisco v come vicino di u
    Node *node = malloc(sizeof(Node));
    node->dest = dest;
    node->weight = w;
    node->next = g->adj[u];
    g->adj[u] = node;

    // Alloco il nuovo nodo e inserisco u come vicino di v
    node = malloc(sizeof(Node));
    node->dest = u;
    node->weight = w;
    node->next = g->adj[dest];
    g->adj[dest] = node;
}

/*
 *  Algoritmo di Dijkstra per il calcolo del cammino
 *  di costo minimo dalla sorgente s a tutte le altre destinazione
 */
void dijkstra(Graph *graph, int src, int dist[])
{
    int V = graph->num_nodes;

    MinHeap *heap = createMinHeap(V);

    for (int v = 0; v < V; ++v)
    {
        dist[v] = INF;
        heap->array[v] = newMinHeapNode(v, dist[v]);
        heap->pos[v] = v;
    }

    heap->array[src] = newMinHeapNode(src, 0);
    heap->pos[src] = src;
    dist[src] = 0;
    heap->size = V;
    decreaseKey(heap, src, dist[src]);

    while (!isEmpty(heap))
    {
        MinHeapNode *minNode = extractMin(heap);
        int u = minNode->dest;

        Node *curr = graph->adj[u];
        while (curr != NULL)
        {
            int v = curr->dest;

            if (isInMinHeap(heap, v) && dist[u] != INF &&
                curr->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + curr->weight;
                decreaseKey(heap, v, dist[v]);
            }
            curr = curr->next;
        }
    }

    // Print distanze
    for (int i = 0; i < V; ++i)
    {
        if (dist[i] == INF)
            printf("Distanza da %d a %d: INF\n", src, i);
        else
            printf("Distanza da %d a %d: %d\n", src, i, dist[i]);
    }
}

int count_when_mulinello_is_worst(int *dist, int *mulinelli, int n)
{
    printf("\n\nCONTROLLO MULINNELLI: \n");
    int count = 0;
    for (int i = 1; i < n; i++)
    {
        // Se il mulinello esiste e (se passare per il mulinello è sconveniente oppure se non esiste un collegamento diretto con il nodo i)
        if (mulinelli[i] != INF && (mulinelli[i] >= dist[i] || dist[i] == INF))
            printf("mulinelli[i]: %d >= DIST[i]: %d;\n", mulinelli[i], dist[i]), // DEBUG
                count++;                                                         // Aumento il conteggio
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

    Graph g;
    int i;
    int n, m, k;
    int u, v, w;
    int s, y;

    // Lettura del numero di N nodi e M archi e K mulinelli
    if (fscanf(in_file, "%d %d %d", &n, &m, &k) != 3)
    {
        fprintf(stderr, "ERR: Lettura prima riga file");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    g.num_nodes = n;
    int mulinelli[n]; // costo del mulinello da sorgente a i-esima posizione
    int dist[n];      // costo minimo per andare da sorgente a i-esima posizione senza usare un mulinello

    // costo per raggiungere se stessi è 0
    dist[0] = mulinelli[0] = 0;

    // All'inizializzazione non esiste nessun mulinello e nessuna dist calcolata
    for (i = 1; i < n; i++)
        mulinelli[i] = dist[i] = INF;

    // Lettura degli archi e popolo il grafo
    for (i = 0; i < m; i++)
    {
        // Lettura archi
        if (fscanf(in_file, "%d %d %d", &u, &v, &w) != 3)
        {
            fprintf(stderr, "Errore lettura arco: %d\n", i);
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        add_edge(&g, u, v, w);
    }

    // Lettura dei mulinelli e dei loro costi
    for (i = 0; i < k; i++)
    {
        if (fscanf(in_file, "%d %d", &s, &y) != 2)
        {
            fprintf(stderr, "Errore lettura mulinello: %d", 1);
            fclose(in_file);
            exit(EXIT_FAILURE);
        }

        mulinelli[s] = y;
    }

    // Eseguo l'algoritmo di dijkstra che mi popolerà i costi all'interno di dist
    dijkstra(&g, 0, dist);

    // Conto il numero di volte in cui i mulinelli sono la scelta peggiore
    int result = count_when_mulinello_is_worst(dist, mulinelli, n);

    // Apro file di output
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("ERRORE APERTURA FILE");
        exit(EXIT_FAILURE);
    }

    fprintf(out_file, "%d", result);

    // Chiudo file di input
    if (fclose(in_file) != 0 || fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file di input");
        exit(EXIT_FAILURE);
    }

    return 0;
}