//
// Created by MACHENIKE on 12/20/2020.
//

#include <io.h>
#include "ulib.h"

char *gets_ulib(char *buf, int max) {
    int i, cc;
    char c;
    //DEBUG to enter exec
    for (i = 0; i + 1 < max;) {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        buf[i++] = c;
        if (c == '\n' || c == '\r')
            break;
    }
    buf[i] = '\0';
    return buf;
}

uint16_t bit_check(uint32_t int_number) {
    uint16_t i;
    for (i = 0; i < 32; i++) {
        if ((int_number & (uint32_t) ((uint16_t) 1 << i)) == 0)
            return i;
    }
    return -1;
}

uint32_t bit_set(uint16_t bit_num, uint32_t int_number) {
    //range of bit_num:[0,31]
    return int_number ^ (uint32_t) ((uint16_t) 1 << bit_num);
}