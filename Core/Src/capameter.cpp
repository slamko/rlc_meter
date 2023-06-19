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

Capa capa_calc(microseconds charge_time, Res res, uint16_t vc0, uint16_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	return Capa::f(((charge_time.count())/ (ln * res.get_ohm() * 1000)));
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint16_t init_val, val;
		  Capa capa;

		  for (int i = 0; i < 5; i++) {
			  measure(&hadc2, GPIO_PIN_4, 10000us, &init_val, &val);
			  capa = capa + capa_calc(10000us, 10_kOhm, init_val, val);
			  wait_us(100000);
		  }

		  capa = capa / 5;

		 // Capa mean_capa =

		  lcd_clear();
		  lcd_print("Capa=");
		  print_unit(capa.get_uf());
		  lcd_print("uF");
		  adc_ready = false;
	  }
}
