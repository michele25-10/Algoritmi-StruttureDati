#include <stdlib.h>
#include <stdio.h>

#define MAX_NODES 1000000

typedef struct Node
{
    int dest;
    struct Node *next;
} Node;

typedef struct
{
    int num_nodes;
    Node *adj[MAX_NODES];
} Graph;

/**
 * Aggiungo un arco al grafo
 *
 *   g.adj[0]: -> 3 -> 2 -> 1
 *   g.adj[1]: -> 0
 *   g.adj[2]: -> 0
 *   g.adj[3]: -> 0
 */
void add_edge(Graph *g, int u, int v)
{
    // Se la coppia è già presente come collegamento allora la ignoro
    Node *curr = g->adj[u];
    while (curr != NULL)
    {
        if (curr->dest == v)
            return;
        curr = curr->next;
    }

    // Alloco il nuovo nodo e inserisco avv2 in avv1
    Node *node = malloc(sizeof(Node));
    node->dest = v;
    node->next = g->adj[u];
    g->adj[u] = node;

    // Alloco il nuovo nodo e inserisco avv1 in avv2
    node = malloc(sizeof(Node));
    node->dest = u;
    node->next = g->adj[v];
    g->adj[v] = node;
}

/*
 * Conto il numero di pugili attivi (che eseguono almeno un match)
 */
int count_active_boxer(Graph *g)
{
    int count = 0;
    for (int i = 0; i < g->num_nodes; i++)
        if (g->adj[i] != NULL)
            count++;

    return count;
}

/**
 * Verifico se il grafo è bipartito attraverso BFS
 * - return 0 non è bipartito
 * - return 1 è bipartito
 */
int is_bipartite(Graph *g)
{
    // Alloco in memoria un array di color
    short int color[g->num_nodes];
    size_t i;
    for (i = 0; i < g->num_nodes; i++)
        color[i] = -1; // NON COLORATO

    // Visito tutti i componenti connessi al grafo
    for (i = 0; i < g->num_nodes; i++)
    {
        // Se il pugile non ha scontri o se è già stato visitato allora lo salti
        if (g->adj[i] == NULL || color[i] != -1)
            continue;

        // Creo una coda
        int *queue = malloc(sizeof(int) * g->num_nodes);
        int front = 0, rear = 0;

        // Aggiungo il nodo in esame alla coda e lo coloro con di bianco (0)
        queue[rear++] = i;
        color[i] = 0;

        while (front < rear)
        {
            // prendo il primo elemento della coda
            int u = queue[front++];

            // Scorro la lista di adiacenza associata
            Node *curr = g->adj[u];
            while (curr != NULL)
            {
                int v = curr->dest;

                // Se non è ancora stato colorato lo coloro
                if (color[v] == -1)
                {
                    // Coloriamo con colore opposto
                    color[v] = 1 - color[u];
                    queue[rear++] = v;
                }

                else if (color[v] == color[u]) // Altrimenti sono due adiacenti con lo stesso colore QUINDI è non bipartito
                {
                    free(queue);
                    return 0;
                }

                curr = curr->next;
            }
        }

        free(queue);
    }
    // Se sono arrivato fino a qui significa che il grafo è bipartito
    return 1;
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
    unsigned int n_match = 0;
    int avv1 = 0, avv2 = 0;

    // Lettura del numero di row
    if (fscanf(in_file, "%d", &g.num_nodes) != 1)
    {
        fprintf(stderr, "Errore nella lettura del numero di pugili'\n");
        fclose(in_file);
        exit(EXIT_FAILURE);
    }

    // Inizializzo la lista:
    for (int i = 0; i < g.num_nodes; i++)
        g.adj[i] = NULL;

    // Lettura del numero di match
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

        // Aggiungo arco
        add_edge(&g, avv1, avv2);
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
    if (is_bipartite(&g))
        fprintf(out_file, "TRUE"); // se è bipartito posso creare i due gruppi allora rispondo TRUE
    else
        fprintf(out_file, "FALSE"); // se non è bipartito non posso creare gruppi rispondo FALSE

    if (fclose(out_file) != 0)
    {
        printf("Errore durante la chiusura del file output");
        exit(EXIT_FAILURE);
    }

    return 0;
}