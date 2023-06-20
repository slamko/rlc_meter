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

static constexpr microseconds DEFAULT_SAMPLE_TIME = 10000us;

static const Res PICO_RESISTOR = 100_kOhm;
static const Res MICRO_RESISTOR = 100_Ohm;

static const uint32_t ADC_PICO_CHANNEL = ADC_CHANNEL_12;
static const uint32_t ADC_MICRO_CHANNEL = ADC_CHANNEL_11;

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

void result(Capa capa) {
	lcd_clear();
	lcd_print("Capa=");
	print_unit(capa.get_auto());
	lcd_print(capa.get_auto_unit());

	adc_ready = false;
	pico_measure_trig = false;
	micro_measure_trig = false;
}

static inline void capa_measure(microseconds sample_time, uint16_t *v0, uint16_t *v) {
	measure(&hadc1, GPIO_PIN_4, sample_time, v0, v, false);
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Res resistor {};
		  Capa capa {};

		  if (pico_measure_trig) {
			  adc_select_ch(&hadc1, ADC_PICO_CHANNEL);
			  resistor = PICO_RESISTOR;
		  } else if (micro_measure_trig){
			  adc_select_ch(&hadc1, ADC_MICRO_CHANNEL);
			  resistor = MICRO_RESISTOR;
		  } else {
			  return;
		  }

		  capa_measure(sample_time, &init_val, &val);

		  for (; val < LOW_ADC_VAL;
				 capa_measure(sample_time, &init_val, &val)) {
			  sample_time *= 10;

			  if (sample_time >= 10s) {
				  result(Capa::nil());
			  }
		  }

		  for (; val > HIGH_ADC_VAL; capa_measure(sample_time, &init_val, &val)) {
			  sample_time /= 10;

			  if (sample_time.count() == 0) {
				  result(Capa::nil());
			  }
		  }

		  for (int i = 0; i < 5; i++) {
			  capa_measure(sample_time, &init_val, &val);
			  capa.set_val(capa + capa_calc(sample_time, resistor, init_val, val));
		  }

		  capa.set_val(capa / 5);

		  result(capa);
	  }
}
