/*
 * TLC59116.h
 *
 *  Created on: Nov 3, 2023
 *      Author: mats
 *
 *      Reference StackOverflow used: https://stackoverflow.com/questions/57796932/does-anyone-have-an-stm32l0xx-with-tlc59116-i2c-example-code
 */

#ifndef INC_TLC59116_TLC59116_H_
#define INC_TLC59116_TLC59116_H_

#include "i2c.h"

// TLC device address
#define TLC59116_ADDR_WRITE 0x61 << 1  // This is for board 1 /J1 shunted
#define TLC59116_ADDR_READ 0x63 << 1  // This is for board 1 /J1 shunted

// I2C device handler reference
#define I2C_DEV &hi2c2


// Register address definitions
#define TLC59116_MODE1_ADDR 0x00
#define TLC59116_MODE2_ADDR 0x01
#define TLC59116_PWM0_ADDR 0x02
#define TLC59116_PWM1_ADDR 0x03
#define TLC59116_PWM2_ADDR 0x04
#define TLC59116_PWM3_ADDR 0x05
#define TLC59116_PWM4_ADDR 0x06
#define TLC59116_PWM5_ADDR 0x07
#define TLC59116_PWM6_ADDR 0x08
#define TLC59116_PWM7_ADDR 0x09
#define TLC59116_PWM8_ADDR 0x0A
#define TLC59116_PWM9_ADDR 0x0B
#define TLC59116_PWM10_ADDR 0x0C
#define TLC59116_PWM11_ADDR 0x0D
#define TLC59116_PWM12_ADDR 0x0E
#define TLC59116_PWM13_ADDR 0x0F
#define TLC59116_PWM14_ADDR 0x10
#define TLC59116_PWM15_ADDR 0x11
#define TLC59116_GRPPWM_ADDR 0x12
#define TLC59116_GRPFREQ_ADDR 0x13
#define TLC59116_LEDOUT0_ADDR 0x14
#define TLC59116_LEDOUT1_ADDR 0x15
#define TLC59116_LEDOUT2_ADDR 0x16
#define TLC59116_LEDOUT3_ADDR 0x17
#define TLC59116_SUBADR1_ADDR 0x18
#define TLC59116_SUBADR2_ADDR 0x19
#define TLC59116_SUBADR3_ADDR 0x1A
#define TLC59116_ALLCALLADR_ADDR 0x1B
#define TLC59116_IREF_ADDR 0x1C
#define TLC59116_EFLAG1_ADDR 0x1D
#define TLC59116_EFLAG2_ADDR 0x1E

// Register default value definitions
#define TLC59116_MODE1_OFF_STATE 0x11
#define TLC59116_MODE1_ON_STATE 0x01
#define TLC59116_MODE2_DEFAULT 0x00

// TLC LEDOUT register value definitions
#define TLC59116_LEDOUT_OFF 0x00
#define TLC59116_LEDOUT_ON 0x01
#define TLC59116_LEDOUT_PWM 0x10
#define TLC59116_LEDOUT_PWM_GROUP 0x11

// Register address values
#define TLC59116_MODE1 0x00
#define TLC59116_MODE2 0x01


// Auto increment values
#define TLC59116_PWM0_AUTOINCR 0x82

void TLC59116_Init(void);
void TLC59116_LED(uint8_t led, uint8_t state);
static void TLC59116_WriteStream(uint8_t len, uint8_t *pData);

#endif /* INC_TLC59116_TLC59116_H_ */
