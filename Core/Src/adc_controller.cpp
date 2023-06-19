/*
 * controller.cpp
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#include "main.h"
#include "adc_controller.h"
#include <chrono>

extern "C" {
#include "outils.h"
}

bool adc_ready = false;

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == START_BUTTON_PIN) {
		adc_ready = true;
	}
}

int measure(ADC_HandleTypeDef *adc, uint16_t supply_pin,
		std::chrono::microseconds sample_delay,
		uint16_t *v0, uint16_t *v) {

	  HAL_GPIO_WritePin(GPIOB, supply_pin, GPIO_PIN_SET);
	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;
	  *v0 = HAL_ADC_GetValue(adc);
//	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

	  wait_us(sample_delay.count());

	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;

	  *v = HAL_ADC_GetValue(adc);
	  HAL_GPIO_WritePin(GPIOB, supply_pin, GPIO_PIN_RESET);
	  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

	  wait_us(sample_delay.count() * 10);

	  return 0;
}

