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

static bool npn_measure_trig = false;

void hfe_npn_measure_trig(void) {
	npn_measure_trig = true;
}

extern "C" void hfemeter(void) {
	if (!npn_measure_trig) {
		return;
	}

	adc_select_ch(&hadc2, ADC_CHANNEL_1);

	npn_measure_trig = false;

}
