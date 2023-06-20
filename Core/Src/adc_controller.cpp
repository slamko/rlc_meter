/*
 * controller.cpp
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#include "main.h"
#include "adc_controller.h"
#include "selfmeter.h"
#include "capameter.h"
#include <chrono>

extern "C" {
#include "outils.h"
}

uint32_t adc1_channels[] = {
		ADC_CHANNEL_11,
		ADC_CHANNEL_12,
};

uint32_t adc2_channels[] = {
		ADC_CHANNEL_4,
};


bool adc_ready = false;

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	adc_ready = true;

	switch (pin) {
	case GPIO_PIN_10:
		capa_micro_measure_trig();
		break;
	case GPIO_PIN_2:
		capa_pico_measure_trig();
		break;
	case GPIO_PIN_12:
		self_milli_measure_trig();
		break;

	}
}

extern "C" void start_button_control(void) {
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) {
		adc_ready = true;
		capa_pico_measure_trig();
	}

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET) {
		adc_ready = true;
		capa_micro_measure_trig();
	}

	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) {
		adc_ready = true;
		self_milli_measure_trig();
	}
}

void adc_select_ch(ADC_HandleTypeDef *adc, uint32_t channel) {


}

int measure(ADC_HandleTypeDef *adc, uint16_t supply_pin,
		std::chrono::microseconds sample_delay,
		uint16_t *v0, uint16_t *v) {

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;
	  *v0 = HAL_ADC_GetValue(adc);

	  wait_us(sample_delay.count());

	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;

	  *v = HAL_ADC_GetValue(adc);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

	  wait_us(sample_delay.count() * 10);

	  return 0;
}

