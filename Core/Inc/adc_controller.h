/*
 * controller.h
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#ifndef INC_ADC_CONTROLLER_H_
#define INC_ADC_CONTROLLER_H_

extern bool adc_ready;

int measure(ADC_HandleTypeDef *adc, uint16_t supply_pin,
		std::chrono::microseconds sample_delay,
		uint16_t *v0, uint16_t *v);

#endif /* INC_ADC_CONTROLLER_H_ */
