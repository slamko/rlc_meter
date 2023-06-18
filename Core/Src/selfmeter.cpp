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

Self self_calc(microseconds charge_time, Res res, uint32_t vc0, uint32_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	return Self::h(((charge_time.count() * 1000 * 1000)/ (ln * res)));
}

extern "C" void selfmeter(void) {
	  if (adc_ready) {
		  uint32_t init_val, val;

		  measure(&hadc2, GPIO_PIN_4, 10000us, &init_val, &val);

		  Self self = self_calc(10000us, 10_Ohm, init_val, val);

		  lcd_clear();
		  lcd_ret_home();
		  print_num(capa.val / 1000000);
		  adc_ready = false;
	  }
}
