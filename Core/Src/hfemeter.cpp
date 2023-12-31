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

static inline long double dac(uint16_t digital) {
	return ((3.3 * (long double)digital) / (long double)4096);
}

static long double hfe_calc(Res rcc, Res rbb, uint16_t vcc, uint16_t vbb) {
	long double ic = ((long double)3.3 - dac(vcc)) / rcc.get_ohm();
	long double ib = (3.3 - dac(vbb)) / rbb.get_ohm();

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
	adc_select_ch(&hadc2, ADC_CHANNEL_1, 0);
	uint16_t vcc = hfe_measure();

	adc_select_ch(&hadc2, ADC_CHANNEL_4, 0);
	uint16_t vbb = hfe_measure();
	long double hfe = hfe_calc(Res::ohm(10), Res::kohm(100), vcc, vbb);

	result(hfe);
}
