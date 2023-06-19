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


uint32_t sine_lookup_table[] = {
2048, 2148, 2249, 2349,
2448, 2546, 2643, 2738,
2832, 2924, 3013, 3101,
3186, 3268, 3347, 3423,
3496, 3565, 3631, 3693,
3751, 3805, 3854, 3899,
3940, 3976, 4008, 4035,
4057, 4074, 4086, 4094,
4096, 4094, 4086, 4074,
4057, 4035, 4008, 3976,
3940, 3899, 3854, 3805,
3751, 3693, 3631, 3565,
3496, 3423, 3347, 3268,
3186, 3101, 3013, 2924,
2832, 2738, 2643, 2546,
2448, 2349, 2249, 2148,
2048, 1948, 1847, 1747,
1648, 1550, 1453, 1358,
1264, 1172, 1083, 995,
910, 828, 749, 673,
600, 531, 465, 403,
345, 291, 242, 197,
156, 120, 88, 61,
39, 22, 10, 2,
0, 2, 10, 22,
39, 61, 88, 120,
156, 197, 242, 291,
345, 403, 465, 531,
600, 673, 749, 828,
910, 995, 1083, 1172,
1264, 1358, 1453, 1550,
1648, 1747, 1847, 1948};


constexpr microseconds DEFAULT_SAMPLE_TIME = 100000us;
constexpr unsigned int SAMPLE_COUNT = 5;
const Res INPUT_RESISTANCE = 10_Ohm;

Self self_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)vc0 / (double)vc);
	Self val = Self::h(((charge_time.count() * res.get_ohm()) / (ln * 1000 * 1000)));
	return val;
}

static inline void start_sine() {
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, sine_lookup_table,
			128, DAC_ALIGN_12B_R);
}

static inline void stop_sine() {
	HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
}

extern "C" void selfmeter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Self self{};

		  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val);

		  for (; val < 256; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val)) {
			  sample_time /= 10;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val)) {
			  sample_time *= 10;
		  }

		  for (unsigned int i = 0; i < SAMPLE_COUNT; i++) {
			  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val);
			  self.set_val(self +
					  self_calc(sample_time, INPUT_RESISTANCE, init_val, val));
		  }

		  self.set_val(self / SAMPLE_COUNT);

		  lcd_clear();
		  lcd_print("Self=");
		  print_unit(self.get_auto());
		  lcd_print(self.get_auto_unit());
		  adc_ready = false;
	  }
}
