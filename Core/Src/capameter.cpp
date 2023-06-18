#include "main.h"
#include <math.h>
#include <stdio.h>
#include "units.h"
#include <chrono>
extern "C" {
	#include "lcd.h"
}

using namespace std::chrono;

bool adc_ready = false;

Capa capa_calc(std::chrono::milliseconds charge_time, Res res, uint32_t vc0, uint32_t vc) {
	double ln = log((double)(4096 - vc0) / (double)(4096 - vc - vc0));
	return Capa::uf(((charge_time.count() * 1000 )/ (ln * res)) * 1000 * 1000);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == GPIO_PIN_10) {
		//GPIO_PinState pin_val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
		//if (pin_val == GPIO_PIN_RESET) {
		adc_ready = true;
		//}
	}
}

char *_print_num_rec(unsigned int num, uint32_t *mul, char *str, size_t siz) {
    if (num >= 10) {
        uint32_t div = (uint32_t)(num / 10);
        char c = num - (div * 10) + 48;
        str[siz - *mul - 1] = c;
        *mul += 1;
        return _print_num_rec(div, mul, str, siz);
    } else {
        char c = num + 48;
        str[siz - *mul - 1] = c;
        return str + siz - *mul - 1;
    }

    return str;
}

extern "C" void fb_print_num(unsigned int num) {
    char str[16];
    uint32_t mul = 1;
    char *str_num = _print_num_rec(num, &mul, str, sizeof(str));
    lcd_msg(str_num, mul);
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
		  //printf("Capacitance: %d", (unsigned int)(capa));
		 //lcd_msg("Hello", 5);
		  fb_print_num(capa.val / 1000000);
		  adc_ready = false;
	  }
}
