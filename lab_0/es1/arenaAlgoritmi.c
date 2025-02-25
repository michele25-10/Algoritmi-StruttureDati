#include <stdio.h>
#include <stdlib.h>

#define mod 100000007

int fast_expo(int base, int exp)
{
    if (exp == 0)
    {
        return 1;
    }

    if (exp == 1)
    {
        return base;
    }

    int t = fast_expo(base, exp / 2);

    if (exp % 2)
        return t;
    else
        return ((base % mod) * t) % mod;
    // Da finire...
}

int main(void)
{
    // Apri il file di lettura e scrittura
    FILE *in_file;
    FILE *out_file;
    int n;

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

    int steps = 0;
    for (int i = 0; i < n; i++)
    {
        int base, exp;
        fscanf(in_file, "%d %d", &base, &exp);

        char tmp[10];
        sprintf(tmp, "%d\n", fast_expo(base, exp));
        fprintf(out_file, tmp);
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
