/*
 * lcd.h
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"

#define d4_set(state) HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, (state));
#define d5_set(state) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (state));
#define d6_set(state) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, (state));
#define d7_set(state) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (state));

#define rs_set(state) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (state));
#define rw_set(state) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, (state));
#define en_set(state) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, (state));

void lcd_init(void);

void lcd_msg(const char *msg, size_t len);
void lcd_clear(void);
void lcd_ret_home(void);

#endif /* INC_LCD_H_ */
