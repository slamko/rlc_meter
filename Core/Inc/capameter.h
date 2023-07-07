/*
 * meter.h
 *
 *  Created on: Jun 14, 2023
 *      Author: slamko
 */

#ifndef INC_CAPAMETER_H_
#define INC_CAPAMETER_H_

#include <stdint.h>

void capa_pico_measure_trig(void);

void capa_nano_measure_trig(void);

void capainit();

void capa_micro_measure_trig(void);

void pico_capameter(uint8_t key);

void nano_capameter(uint8_t key);

void micro_capameter(uint8_t key);


#endif /* INC_CAPAMETER_H_ */
