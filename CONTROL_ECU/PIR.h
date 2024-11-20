/*
 *  Module: PIR
 *
 *  Created on: Oct 25, 2024
 *
 *  Author: Mahmoud Hashish
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/* Function to initialize the PIR driver */
void PIR_init(void);
/* Function to return PIR State */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
