/*
 * outils.c
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */


#include "main.h"
#include "outils.h"
#include "lcd.h"

void wait_us(unsigned long delay) {
	TIM2->CNT = 0;

	while(TIM2->CNT < delay);
}

char *_print_num_rec(unsigned int num, uint32_t *mul, char *str, size_t siz, int comma) {
	if (comma > 0 && comma == *mul - 1) {
    	str[siz - *mul - 1] = ',';
    	*mul += 1;
    	return _print_num_rec(num, mul, str, siz, comma);
    }

	if (num >= 10) {
        uint32_t div = (uint32_t)(num / 10);
        char c = num - (div * 10) + 48;
        str[siz - *mul - 1] = c;
        *mul += 1;
        return _print_num_rec(div, mul, str, siz, comma);
    } else {
        char c = num + 48;
        str[siz - *mul - 1] = c;
        return str + siz - *mul - 1;
    }

    return str;
}

void print_num(unsigned int num, int comma) {
    char str[16];
    uint32_t mul = 1;
    char *str_num = _print_num_rec(num, &mul, str, sizeof(str), comma);
    lcd_msg(str_num, mul);
}

void print_unit(long double num) {
	if (num > 100.0) {
		print_num((unsigned int)num, 0);
	} else if (num > 10.0) {
		print_num((unsigned int)(num * 10), 1);
	} else if (num > 1.0) {
		print_num((unsigned int)(num * 100), 2);
	}
}

