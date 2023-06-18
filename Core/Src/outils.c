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

