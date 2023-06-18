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

Capa capa_calc(std::chrono::milliseconds charge_time, Res res, uint32_t vc0, uint32_t vc) {
	double ln = log((double)(E - vc0) / (double)(E - vc));
	return Capa::f(((charge_time.count() * 1000)/ (ln * res.get_ohm())));
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint32_t init_val, val;

		  HAL_ADC_Start(&hadc2);
		  if (HAL_ADC_PollForConversion(&hadc2, 1000) != HAL_OK) return;
		  init_val = HAL_ADC_GetValue(&hadc2);

		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		  HAL_Delay(10);
		  HAL_ADC_Start(&hadc2);
		  if (HAL_ADC_PollForConversion(&hadc2, 1000) != HAL_OK) return;

		  val = HAL_ADC_GetValue(&hadc2);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		  Capa capa = capa_calc(10ms, 10_kOhm, init_val, val);

		  lcd_clear();
		  lcd_ret_home();
		  print_num(capa.get_uf());
		  adc_ready = false;
	  }
}
