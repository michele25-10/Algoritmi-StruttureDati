#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    // Anti-debug trick: lettura da GS:0x14 (non riproducibile direttamente in C standard)
    // int32_t anti_debug_val = *(uint32_t *)((char *)gsbase + 0x14); // <-- ignorato qui

    char input[8] = "ABCDEFGH";
    char result[9]; // 8 + 1 per null-terminatore

    // XOR simmetrico e aggiunta 0x40
    for (int i = 0; i <= 7; i++)
    {
        result[i] = (input[7 - i] ^ input[i]) + 0x40;
    }

    result[8] = '\0'; // null-termina la stringa (anche se nel codice originale non viene stampata)

    // Per debug: puoi stampare il risultato
    printf("Result: %s\n", result);

        return 0;
}
