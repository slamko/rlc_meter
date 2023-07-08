/*
 * controller.h
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#ifndef INC_ADC_CONTROLLER_H_
#define INC_ADC_CONTROLLER_H_

#include <chrono>

extern bool adc_ready;

constexpr uint16_t LOW_ADC_VAL = 312;
constexpr uint16_t HIGH_ADC_VAL = 4096 - 512;

int adc_select_ch(ADC_HandleTypeDef *adc, uint32_t channel, uint32_t sample_rate);

int measure(ADC_HandleTypeDef *adc, uint16_t supply_pin,
		std::chrono::microseconds sample_delay,
		uint16_t *v0, uint16_t *v, bool set_first);

#endif /* INC_ADC_CONTROLLER_H_ */
