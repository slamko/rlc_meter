/*
 * hfemeter.cpp
 *
 *  Created on: Jul 7, 2023
 *      Author: slamko
 */


#include "main.h"
#include <math.h>
#include <stdio.h>
#include "units.h"
#include <chrono>
#include <adc_controller.h>

extern "C" {
	#include "outils.h"
	#include "lcd.h"
}

using namespace std::chrono;

static long double hfe_calc(Res rcc, Res rbb, uint16_t vcc) {
	long double ic = ((3.3 * (long double)vcc) / (long double)4096) / rcc.get_ohm();
	long double ib = (3.3 - 0.7) / rbb.get_ohm();

	return ic / ib;
}

static inline uint16_t hfe_measure() {
	HAL_ADC_Start(&hadc2);

	if (HAL_ADC_PollForConversion(&hadc2, 1000) != HAL_OK) {
		return 0;
	}

	uint16_t val = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return val;
}

static void result(long double hfe) {
	lcd_clear();
	lcd_print("hfe=");
	print_num(hfe, 0);

	adc_ready = false;
}

void hfemeter(uint8_t key) {
	adc_select_ch(&hadc2, ADC_CHANNEL_1);

	uint16_t vcc = hfe_measure();
	long double hfe = hfe_calc(Res::ohm(20), Res::kohm(10), vcc);

	result(hfe);
}
