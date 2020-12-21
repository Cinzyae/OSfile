//
// Created by MACHENIKE on 12/20/2020.
//

#ifndef OSFILE_ULIB_H
#define OSFILE_ULIB_H

#include <stdint.h>

char *gets_ulib(char *buf, int max);

uint16_t bit_check(uint32_t int_number);

uint32_t bit_set(uint16_t bit_num, uint32_t int_number);

#endif //OSFILE_ULIB_H
