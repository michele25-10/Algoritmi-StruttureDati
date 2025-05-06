#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main()
{
    uint8_t str[9] = {0};                               // 8 byte + null terminator
    memcpy(str, "\xef\x2d\x7f\xce\x7b\x2d\x51\x69", 8); // Little endian version of 0x69512d7bce7f2def

    for (int i = 0; i < 8; i++)
    {
        int8_t b = -(str[i]);
        uint8_t x = ((~(-(~((-((((b * 2) | ((uint8_t)b >> 7)) - 0x16)) + 1) ^ 0x8f)) - i) - 0x63) << 3) |
                    ((~(-(~((-((((b * 2) | ((uint8_t)b >> 7)) - 0x16)) + 1) ^ 0x8f)) - i) - 0x63) >> 5);

        str[i] = ((x + i) ^ 0x6e) - 0x17;
    }

    str[8] = '\0'; // null terminate

    puts((char *)str);
    return 0;
}
