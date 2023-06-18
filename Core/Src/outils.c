/*
 * outils.c
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */


#include "main.h"
#include "outils.h"

void wait_us(unsigned long delay) {
	TIM2->CNT = 0;

	while(TIM2->CNT < delay);
}

char *_print_num_rec(unsigned int num, uint32_t *mul, char *str, size_t siz) {
    if (num >= 10) {
        uint32_t div = (uint32_t)(num / 10);
        char c = num - (div * 10) + 48;
        str[siz - *mul - 1] = c;
        *mul += 1;
        return _print_num_rec(div, mul, str, siz);
    } else {
        char c = num + 48;
        str[siz - *mul - 1] = c;
        return str + siz - *mul - 1;
    }

    return str;
}

void print_num(unsigned int num) {
    char str[16];
    uint32_t mul = 1;
    char *str_num = _print_num_rec(num, &mul, str, sizeof(str));
    lcd_msg(str_num, mul);
}

