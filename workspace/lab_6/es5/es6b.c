#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main()
{
    uint8_t str[33] = {
        0xa5, 0x99, 0xdc, 0x17, 0x58, 0x00, 0x55, 0x35,
        0x75, 0x1e, 0x76, 0x94, 0xfb, 0x32, 0x51, 0x3b,
        0xe0, 0x8a, 0x4e, 0x8f, 0x79, 0x8f, 0x31, 0x8c,
        0x6b, 0xf0, 0x52, 0x6d, 0x32, 0x8d, 0x05, 0xf0,
        0x00 // Null terminator per sicurezza
    };

    for (int i = 0; i < 32; i++)
    {
        uint8_t b = str[i];
        uint8_t rot = (b << 2) | (b >> 6);
        uint8_t temp = (uint8_t)~(((~((rot + i) - 7) << 7) | ((~((rot + i) - 7)) >> 1)) ^ 0x65);
        temp = ~(temp + 0x38) ^ i;
        temp = ~(temp + 0x2e) ^ 0x5e;
        str[i] = (temp ^ 0x75) + i;
    }

    str[32] = '\0'; // Assicuriamoci che sia null-terminated
    puts((char *)str);
    return 0;
}
