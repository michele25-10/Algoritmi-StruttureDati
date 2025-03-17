#include <stdlib.h>
#include <stdio.h>

int pasathon(int m, int n, int i, int j, int k)
{
    if (k <= 0)
    {
        return 0;
    }
    k -= 1;

    if (i == 0)
    {
        if (j == 0) // Angolo in alto a sinistra
            return 3 + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i + 1, j + 1, k);
        else if (j == (n - 1)) // Angolo in alto a destra
            return 3 + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i + 1, j - 1, k);
        else // corpo centrale della prima riga
            return 5 + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i + 1, j - 1, k) + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i + 1, j + 1, k);
    }
    else if (i == (m - 1))
    {
        if (j == 0)
            return 3 + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i - 1, j + 1, k);
        else if (j == (n - 1))
            return 3 + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i - 1, j - 1, k);
        else
            return 5 + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i - 1, j - 1, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i - 1, j + 1, k);
    }
    else
    {
        if (j == 0)
            return 5 + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i + 1, j + 1, k) + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i - 1, j + 1, k);
        else if (j == (n - 1))
            return 5 + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i + 1, j - 1, k) + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i - 1, j - 1, k);
        else
            return 8 + pasathon(m, n, i, j - 1, k) + pasathon(m, n, i, j + 1, k) + pasathon(m, n, i + 1, j - 1, k) + pasathon(m, n, i + 1, j, k) + pasathon(m, n, i + 1, j + 1, k) + pasathon(m, n, i - 1, j - 1, k) + pasathon(m, n, i - 1, j, k) + pasathon(m, n, i - 1, j + 1, k);
    }
}

int main(int argc, char **argv)
{
    int m, n, i, j, k;
    srand(1);

    printf("\nInserisci M righe: \n");
    scanf("%d", &m);

    printf("\nInserisci N colonne: \n");
    scanf("%d", &n);

    printf("\nInserisci il numero di step K: \n");
    scanf("%d", &k);

    int A[m][n];

    // genero coordinate i, j;
    i = rand() % (m + 1);
    j = rand() % (n + 1);

    printf("\n\ni, j: %d %d", i, j);

    int tot = pasathon(m, n, i, j, k);

    printf("\n\nIl totale di combinazioni sono: %d\n", tot);

    return 0;
}