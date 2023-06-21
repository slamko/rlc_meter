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

static constexpr microseconds PICO_SAMPLE_TIME 	= 50us;
static constexpr microseconds NANO_SAMPLE_TIME 	= 10000us;
static constexpr microseconds MICRO_SAMPLE_TIME = 10000us;

static const Res PICO_RESISTOR = 1000_kOhm;
static const Res NANO_RESISTOR = 100_kOhm;
static const Res MICRO_RESISTOR = 100_Ohm;

static const uint32_t ADC_PICO_CHANNEL = ADC_CHANNEL_4;
static const uint32_t ADC_NANO_CHANNEL = ADC_CHANNEL_12;
static const uint32_t ADC_MICRO_CHANNEL = ADC_CHANNEL_11;
static const uint32_t KOJINE_FACTOR_LIMIT = 6;
static const uint32_t CAPA_SAMPLE_COUNT = 5;

bool pico_measure_trig = false;
bool nano_measure_trig = false;
bool micro_measure_trig = false;

long double pico_calibration;

void capa_pico_measure_trig(void) {
	pico_measure_trig = true;
}

void capa_nano_measure_trig(void) {
	nano_measure_trig = true;
}

void capa_micro_measure_trig(void) {
	micro_measure_trig = true;
}

Capa capa_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	long double kojine_factor = (vc0 < KOJINE_FACTOR_LIMIT ? 1 : vc0);
	long double capa_pure = ((charge_time.count())/ (ln * res.get_ohm() * 1000 * 1000));
	return Capa::f(capa_pure);
}

void result(Capa capa) {
	lcd_clear();
	lcd_print("Capa=");
	print_unit(capa.get_auto());
	lcd_print(capa.get_auto_unit());

	adc_ready = false;
	nano_measure_trig = false;
	pico_measure_trig = false;
	micro_measure_trig = false;
}

static inline void capa_measure(microseconds sample_time, uint16_t *v0, uint16_t *v) {
	measure(&hadc1, GPIO_PIN_4, sample_time, v0, v, false);
}

Capa sample_measure(microseconds sample_time, Res resistor) {
	Capa capa;
	uint16_t init_val, val;

	for (unsigned int i = 0; i < CAPA_SAMPLE_COUNT; i++) {
		capa_measure(sample_time, &init_val, &val);
		capa.set_val(capa + capa_calc(sample_time, resistor, init_val, val));
	}

	capa.set_val(capa / CAPA_SAMPLE_COUNT);
	return capa;
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time;
		  Res resistor {};
		  //Capa capa[5] ;
		  Capa capa{};

		  if (pico_measure_trig) {
			  adc_select_ch(&hadc1, ADC_PICO_CHANNEL);
			  resistor = PICO_RESISTOR;
			  sample_time = PICO_SAMPLE_TIME;
		  } else if (nano_measure_trig) {
			  adc_select_ch(&hadc1, ADC_NANO_CHANNEL);
			  resistor = NANO_RESISTOR;
			  sample_time = NANO_SAMPLE_TIME;
		  } else if (micro_measure_trig){
			  adc_select_ch(&hadc1, ADC_MICRO_CHANNEL);
			  resistor = MICRO_RESISTOR;
			  sample_time = MICRO_SAMPLE_TIME;
		  } else {
			  return;
		  }

		  capa_measure(sample_time, &init_val, &val);

		  for (; val < LOW_ADC_VAL;
				 capa_measure(sample_time, &init_val, &val)) {
			  sample_time *= 10;

			  if (sample_time >= 10s) {
				  result(Capa::nil());
				  return;
			  }
		  }

		  for (; val > HIGH_ADC_VAL; capa_measure(sample_time, &init_val, &val)) {
			  sample_time /= 10;

			  if (sample_time.count() == 0) {
				  result(Capa::nil());
				  return;
			  }
		  }

		  capa = sample_measure(sample_time, resistor);

		  if (pico_measure_trig) {
			  capa.set_val(Capa::pf(capa.get_pf() - pico_calibration));
		  }

		  result(capa);
	  }
}

void pico_channel_calibration() {
	adc_select_ch(&hadc1, ADC_PICO_CHANNEL);
	Res resistor = PICO_RESISTOR;
	microseconds sample_time = PICO_SAMPLE_TIME;

	Capa init_pico = sample_measure(sample_time, resistor);

	pico_calibration = init_pico.get_pf();
}

void capainit() {
	pico_channel_calibration();
}
