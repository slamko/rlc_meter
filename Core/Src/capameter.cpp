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

constexpr microseconds DEFAULT_SAMPLE_TIME = 1000us;

Capa capa_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	return Capa::f(((charge_time.count())/ (ln * res.get_ohm() * 1000)));
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  microseconds sample_time = DEFAULT_SAMPLE_TIME;
		  Capa capa{};

		  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val);

		  for (; val < LOW_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val)) {
			  sample_time *= 10;
		  }

		  for (; val > HIGH_ADC_VAL; measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val)) {
			  sample_time /= 10;
		  }

		  for (int i = 0; i < 5; i++) {
			  measure(&hadc2, GPIO_PIN_4, sample_time, &init_val, &val);
			  capa = capa + capa_calc(sample_time, 100_kOhm, init_val, val);
		  }

		  capa = capa / 5;

		  lcd_clear();
		  lcd_print("Capa=");
		  print_unit(capa.get_auto());
		  lcd_print(capa.get_auto_unit());
		  adc_ready = false;
	  }
}
