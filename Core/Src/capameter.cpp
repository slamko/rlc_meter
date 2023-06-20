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

bool pico_measure_trig = false;
bool micro_measure_trig = false;

void capa_pico_measure_trig(void) {
	pico_measure_trig = true;
}

void capa_micro_measure_trig(void) {
	micro_measure_trig = true;
}

Capa capa_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	return Capa::f(((charge_time.count())/ (ln * res.get_ohm() * 1000 * 1000)));
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Res resistor {};
		  Capa capa {};

		  if (pico_measure_trig) {
			  adc_select_ch(&hadc1, ADC_CHANNEL_12);
			  resistor = 100_kOhm;
		  } else if (micro_measure_trig){
			  adc_select_ch(&hadc1, ADC_CHANNEL_11);
			  resistor = 100_Ohm;
		  } else {
			  return;
		  }

		  measure(&hadc1, GPIO_PIN_4, sample_time, &init_val, &val, false);

		  for (; val < 312; measure(&hadc1, GPIO_PIN_4, sample_time, &init_val, &val, false)) {
			  sample_time *= 10;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc1, GPIO_PIN_4, sample_time, &init_val, &val, false)) {
			  sample_time /= 10;
		  }

		  for (int i = 0; i < 5; i++) {
			  measure(&hadc1, GPIO_PIN_4, sample_time, &init_val, &val, false);
			  capa.set_val(capa + capa_calc(sample_time, resistor, init_val, val));
		  }

		  capa.set_val(capa / 5);

		  lcd_clear();
		  lcd_print("Capa=");
		  print_unit(capa.get_auto());
		  lcd_print(capa.get_auto_unit());
		  adc_ready = false;
		  pico_measure_trig = false;
		  micro_measure_trig = false;
	  }
}
