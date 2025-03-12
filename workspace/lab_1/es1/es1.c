#include <stdio.h>
#include <stdlib.h>

void insertion_sort(int digits[], int length)
{
    for (int j = 1; j < length; j++)
    {
        int key = digits[j];
        int k = j - 1;

        while ((k >= 0) && (digits[k] > key))
        {
            digits[k + 1] = digits[k];
            k -= 1;
        }
        digits[k + 1] = key;
    }
}

void make_asc_desc(int n, int *asc, int *desc)
{
    int digits[4];
    for (int k = 3; k >= 0; k--)
    {
        digits[k] = n % 10;
        n /= 10;
    }

    insertion_sort(digits, 4);
    *asc = digits[0] * 1000 + digits[1] * 100 + digits[2] * 10 + digits[3];
    *desc = digits[3] * 1000 + digits[2] * 100 + digits[1] * 10 + digits[0];
}

int kaprekar(int n)
{
    int steps = 0;
    int asc, desc;

    // ripeti fino a quando non ottinei il numero 6174
    while (n != 6174)
    {
        // posiziona cifre in ordine decrescente e poi in ordine crescente in
        // modo da ottenere due numeri di quattro cifre eventualmente con degli zeri all'iniziio
        make_asc_desc(n, &asc, &desc);

        // Sottrai il numero minore dal maggiore
        n = desc - asc;

        ++steps;
    }

    return steps;
}

int main(void)
{
    // Apri il file di lettura e scrittura
    FILE *in_file;
    FILE *out_file;
    int n;

    // apro file in cui leggere
    in_file = fopen("input.txt", "r");
    if (in_file == NULL)
    {
        printf("Errore apertura file lettura");
        return -1;
    }
    fscanf(in_file, "%d", &n);

    out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("Errore apertura file scrittura");
        return -2;
    }

    for (int i = 0; i < n; i++)
    {
        // numero letto ha solo 4 cifre
        int number;
        fscanf(in_file, "%d", &number);
        int steps = kaprekar(number);
        fprintf(out_file, "%d\n", steps);
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}