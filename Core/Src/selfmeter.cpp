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

Self self_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)vc0 / (double)vc);
	return Self::h(((charge_time.count() * 1000 * 1000 * res) / ln));
}

extern "C" void selfmeter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;

		  measure(&hadc2, GPIO_PIN_4, 10000us, &init_val, &val);

		  Self self = self_calc(10000us, 10_Ohm, init_val, val);

		  if (self < 0) {
			  // un condo
		  }

		 // lcd_clear();
		 // lcd_ret_home();
		 // print_num(self.val / 1000000);
		  adc_ready = false;
	  }
}
