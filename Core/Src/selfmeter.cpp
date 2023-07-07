/*
 * selfmeter.cpp
 *
 *  Created on: Jun 18, 2023
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

constexpr microseconds DEFAULT_SAMPLE_TIME = 10000us;
constexpr unsigned int SAMPLE_COUNT = 5;
const Res INPUT_RESISTANCE = 10_Ohm;

Self self_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)vc0 / (double)vc);
	Self val = Self::h(((charge_time.count() * res.get_ohm()) / (ln * 1000 * 1000)));
	return val;
}

void result (Self self) {
	lcd_clear();
	lcd_print("Self=");
	print_unit(self.get_auto());
	lcd_print(self.get_auto_unit());
	adc_ready = false;
}

static inline void self_measure(microseconds sample_time, uint16_t *v0, uint16_t *v) {
	measure(&hadc2, GPIO_PIN_7, sample_time, v0, v, true);
}

void calibrate_mult(microseconds &sample_time, unsigned int mult, uint16_t *val) {
	uint16_t init_val;
	for (; *val < 256; self_measure(sample_time, &init_val, val)) {
		 sample_time /= mult;

		 if (sample_time.count() <= 1) {
			 result(Self::nil());
		 }
	 }

	 for (; *val > HIGH_ADC_VAL; self_measure(sample_time, &init_val, val)) {
		 sample_time *= 10;

		 if (sample_time >= 10s) {
			 result(Self::nil());
			 return;
		 }
	 }
}

void calibrate_add(microseconds &sample_time, unsigned int mult, uint16_t *val) {
	uint16_t init_val;
	for (; *val < 256; self_measure(sample_time, &init_val, val)) {
		 sample_time -= sample_time / mult;

		 if (sample_time.count() <= 1) {
			 result(Self::nil());
			 return;
		 }
	 }

	 for (; *val > HIGH_ADC_VAL; self_measure(sample_time, &init_val, val)) {
		 sample_time += sample_time / 10;

		 if (sample_time >= 10s) {
			 result(Self::nil());
			 return;
		 }
	 }
}

void selfmeter(uint8_t key) {
	  if (true) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Self self{};

		  adc_select_ch(&hadc2, ADC_CHANNEL_4);
		  self_measure(sample_time, &init_val, &val);

		  if (val > init_val) {
			  result(Self::nil());
			  return;
		  }

		  calibrate_mult(sample_time, 10, &val);

		  calibrate_mult(sample_time, 5, &val);

		  calibrate_add(sample_time, 10, &val);

		  for (unsigned int i = 0; i < SAMPLE_COUNT; i++) {
			  self_measure(sample_time, &init_val, &val);
			  self.set_val(self +
					  self_calc(sample_time, INPUT_RESISTANCE, init_val, val));
		  }

		  self.set_val(self / SAMPLE_COUNT);

		  result(self);
	  }
}
