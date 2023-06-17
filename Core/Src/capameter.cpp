#include "main.h"
#include <math.h>
#include <stdio.h>
#include "units.h"
#include <chrono>

using namespace std::chrono;

bool adc_ready = false;

Capa capa_calc(std::chrono::milliseconds charge_time, Res res, uint32_t vc0, uint32_t vc) {
	double ln = log((double)(4096 - vc0) / (double)(4096 - vc - vc0));
	return Capa(((charge_time.count() * 1000 )/ (ln * res)) * 1000 * 1000);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == GPIO_PIN_10) {
		GPIO_PinState pin_val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
		if (pin_val == GPIO_PIN_RESET) {
			adc_ready = true;
		}
	}
}

extern "C" void capameter(void) {
	  if (adc_ready) {
		  uint32_t init_val, val;

		  adc_ready = false;
		  HAL_ADC_Start(&hadc2);
		  if (HAL_ADC_PollForConversion(&hadc2, 1000) != HAL_OK) return;
		  init_val = HAL_ADC_GetValue(&hadc2);

		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_Delay(10);
		  HAL_ADC_Start(&hadc2);
		  if (HAL_ADC_PollForConversion(&hadc2, 1000) != HAL_OK) return;

		  val = HAL_ADC_GetValue(&hadc2);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		  Capa capa = capa_calc(10ms, 10_kOhm, init_val, val);
		  printf("Capacitance: %d", (unsigned int)(capa));
	  }
}
