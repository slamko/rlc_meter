/*
 * lcd.c
 *
 *  Created on: Jun 18, 2023
 *      Author: slamko
 */

#include "main.h"
#include "outils.h"
#include "lcd.h"

static void db_nibble_write(uint8_t val) {
	d4_set(val & (1 << 0));
	d5_set(val & (1 << 1));
	d6_set(val & (1 << 2));
	d7_set(val & (1 << 3));

	en_set(GPIO_PIN_SET);
	HAL_Delay(1);
	en_set(GPIO_PIN_RESET);
	HAL_Delay(1);
}

static void cmd_nibble_write(uint8_t data) {
	rs_set(0);
	rw_set(0);
	db_nibble_write(data & 0xF);
}

static void cmd_write(uint8_t data) {
	rs_set(0);
	rw_set(0);
	db_nibble_write(data & 0x0F);
	db_nibble_write((data & 0xF0) >> 4);
}

static void cur_shift_right(void) {
	cmd_write(0x00);
}

static void data_write(char data) {
	rs_set(1);
	rw_set(0);
	db_nibble_write(data & 0x0F);
	db_nibble_write((data & 0xF0) >> 4);
}

void lcd_msg(const char *msg, size_t len) {
	for (unsigned int i  = 0; msg[i] && i < len; i++) {
		data_write(msg[i]);
		HAL_Delay(3);
	}
}

void lcd_clear(void) {
	cmd_write(0x00);
}

void lcd_init(void) {
	HAL_Delay(100);
	cmd_write(0x03);

	HAL_Delay(5);
	cmd_write(0x03);

	HAL_Delay(1);
	cmd_write(0x03);

	HAL_Delay(10);
	cmd_write(0x02);

	HAL_Delay(10);
	cmd_write(0x82);
	HAL_Delay(3);
	cmd_write(0x80);
	HAL_Delay(3);
	cmd_write(0x10);
	HAL_Delay(3);
	cmd_write(0x40);
	HAL_Delay(3);

	//cur_shift_right();
	HAL_Delay(10);
	//lcd_msg("hhello");
	//lcd_clear();

}


