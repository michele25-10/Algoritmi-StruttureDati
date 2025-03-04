#include <stdio.h>
#include <stdlib.h>

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
    fclose(in_file);

    int steps = 0;
    while (n != 1)
    {
        if (n % 2 == 0)
        {
            // Il numero è pari
            n = n / 2;
        }
        else
        {
            // Il numero è dispari
            n = (n * 3) + 1;
        }

        steps++;
        printf("%d\n", steps);
    }

    out_file = fopen("output.txt", "w");
    if (out_file == NULL)
    {
        printf("Errore apertura file scrittura");
        return -2;
    }
    char tmp[4] = "";
    sprintf(tmp, "%d\n", steps);
    fprintf(out_file, tmp, sizeof(tmp));
    fclose(out_file);

    return 0;
}
