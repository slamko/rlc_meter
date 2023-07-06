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

static bool milli_measure_trig = false;

void self_milli_measure_trig(void) {
	milli_measure_trig = true;
}

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
	milli_measure_trig = false;
	adc_ready = false;
}

extern "C" void selfmeter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Self self{};

		  if (!milli_measure_trig) {
			  return;
		  }

		  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true);

		  for (; val < 256; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time /= 10;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time *= 10;

			  if (sample_time >= 10s) {
				  //result(Self:)
				  return;
			  }
		  }

		  for (; val < 256; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time /= 5;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time *= 5;
		  }
/*
		  for (; val < 256; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time /= 2;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time *= 2;
		  }
*/
		  for (; val < 256; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time -= sample_time / 10;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true)) {
			  sample_time += sample_time / 10;
		  }

		  for (unsigned int i = 0; i < SAMPLE_COUNT; i++) {
			  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val, true);
			  self.set_val(self +
					  self_calc(sample_time, INPUT_RESISTANCE, init_val, val));
		  }

		  self.set_val(self / SAMPLE_COUNT);

		  result(self);
	  }
}
