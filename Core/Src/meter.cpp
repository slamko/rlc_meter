#include "main.h"
#include <math.h>
#include <stdio.h>
#include <chrono>

using namespace std::chrono;

bool adc_ready = false;

double capa_calc(std::chrono::milliseconds charge_time, uint32_t res, uint32_t vc) {
	double ln = log((double)4096 / (double)(4096 - vc));
	return ((charge_time.count() * 1000 )/ (ln * res));
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == GPIO_PIN_10) {
		GPIO_PinState pin_val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
		if (pin_val == GPIO_PIN_RESET) {
			adc_ready = true;
		}
	}
}

extern "C" void loop(void) {
	  if (adc_ready) {
		  adc_ready = false;
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_Delay(100);
		  HAL_ADC_Start(&hadc1);

		  uint32_t val = HAL_ADC_GetValue(&hadc1);
		  double cap = capa_calc(1ms, 1000, val);
		  printf("Capacitance: %lu", cap);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	  }
}
