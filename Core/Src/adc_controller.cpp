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
#include "hfemeter.h"
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
uint8_t key_mask;

typedef void(*meter_f)(uint8_t);

meter_f meter_table[] = {
		&pico_capameter,
		&nano_capameter,
		&micro_capameter,
		&selfmeter,
		&hfemeter,
};

extern "C" void start_button_control(void) {
	uint8_t cur_key_mask = 0;
	cur_key_mask |= (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) << 0);
	cur_key_mask |= (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) << 1);
	cur_key_mask |= (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)  << 2);

	key_mask |= cur_key_mask;

	if (!cur_key_mask && key_mask) {
		meter_table[key_mask - 1](key_mask);
		key_mask = 0;
	}
}

int adc_select_ch(ADC_HandleTypeDef *adc, uint32_t channel) {
	ADC_ChannelConfTypeDef ch_conf = {0};

	ch_conf.Channel = channel;
	ch_conf.Rank = ADC_REGULAR_RANK_1;
	ch_conf.SingleDiff = ADC_SINGLE_ENDED;
	ch_conf.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
	ch_conf.OffsetNumber = ADC_OFFSET_NONE;
	ch_conf.Offset = 0;
	if (HAL_ADC_ConfigChannel(adc, &ch_conf) != HAL_OK)
	{
	  return 1;
	}

	return 0;
}

int measure(ADC_HandleTypeDef *adc, uint16_t supply_pin,
		std::chrono::microseconds sample_delay,
		uint16_t *v0, uint16_t *v, bool set_first)
{
	  if (set_first) {
		  HAL_GPIO_WritePin(GPIOB, supply_pin, GPIO_PIN_SET);
	  }

	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;
	  *v0 = HAL_ADC_GetValue(adc);

	  if (!set_first) {
		  HAL_GPIO_WritePin(GPIOB, supply_pin, GPIO_PIN_SET);
	  }

	  wait_us(sample_delay.count());

	  HAL_ADC_Start(adc);
	  if (HAL_ADC_PollForConversion(adc, 1000) != HAL_OK) return 1;

	  *v = HAL_ADC_GetValue(adc);
	  HAL_GPIO_WritePin(GPIOB, supply_pin, GPIO_PIN_RESET);
	  //HAL_ADC_Stop(&hadc1);

	  wait_us(sample_delay.count() * 10);
	 // HAL_Delay(1000);

	  return 0;
}

extern "C" void adc_init(void) {
	capainit();
}

