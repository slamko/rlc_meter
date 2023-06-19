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
	d4_set((val & (1 << 0)) >> 0);
	d5_set((val & (1 << 1)) >> 1);
	d6_set((val & (1 << 2)) >> 2);
	d7_set((val & (1 << 3)) >> 3);

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
	db_nibble_write((data & 0xF0) >> 4);
	db_nibble_write(data & 0x0F);
}

static void cur_shift_right(void) {
	cmd_write(0x00);
}

static void data_write(char data) {
	rs_set(1);
	rw_set(0);
	db_nibble_write((data & 0xF0) >> 4);
	db_nibble_write(data & 0x0F);
}

void lcd_msg(const char *msg, size_t len) {
	for (unsigned int i  = 0; msg[i] && i < len; i++) {
		data_write(msg[i]);
		HAL_Delay(3);
	}
}

void lcd_print(const char *msg) {
	for (unsigned int i  = 0; msg[i]; i++) {
		data_write(msg[i]);
		HAL_Delay(3);
	}
}

void lcd_ret_home(void) {
	cmd_write(0x02);
	HAL_Delay(1);
}

void lcd_clear(void) {
	cmd_write(0x01);
	HAL_Delay(1);
}

void lcd_shift_left(void) {
	cmd_write(0x18);
	wait_us(60);
}

void lcd_shift_right(void) {
	cmd_write(0x1C);
	wait_us(60);
}

void lcd_init(void) {
	cmd_nibble_write(0x0);

	HAL_Delay(150);
	cmd_nibble_write(0x3);

	HAL_Delay(5);
	cmd_nibble_write(0x3);

	HAL_Delay(1);
	cmd_nibble_write(0x3);

	HAL_Delay(1);
	cmd_nibble_write(0x2);

	HAL_Delay(10);
	cmd_write(0x28);
	HAL_Delay(3);
	cmd_write(0x0C);
	HAL_Delay(3);
	cmd_write(0x06);
	HAL_Delay(3);
	cmd_write(0x01);
	HAL_Delay(3);
}


