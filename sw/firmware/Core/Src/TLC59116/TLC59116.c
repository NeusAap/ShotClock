/*
 * TLC59116.c
 *
 *  Created on: Nov 3, 2023
 *      Author: mats
 *
 *      Reference StackOverflow used: https://stackoverflow.com/questions/57796932/does-anyone-have-an-stm32l0xx-with-tlc59116-i2c-example-code
 */

#include "main.h"
#include "TLC59116.h"

static void TLC59116_WriteReg(uint8_t reg, uint8_t val);

static void TLC59116_WriteStream(uint8_t len, uint8_t *pData);

void TLC59116_Init(void)
{
    // Set default register values
    TLC59116_WriteReg(TLC59116_MODE1_ADDR, TLC59116_MODE1_ON_STATE);
    TLC59116_WriteReg(TLC59116_MODE2_ADDR, TLC59116_MODE2_DEFAULT);

    // Set all PWM values to 0x00 (OFF)
    uint8_t aTXBuffer[] = {
            TLC59116_PWM0_AUTOINCR,
            TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF,
            TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF,
            TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF,
            TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF,
            TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF, TLC59116_LEDOUT_OFF,
            TLC59116_LEDOUT_OFF
    };

    TLC59116_WriteStream(sizeof(aTXBuffer), aTXBuffer);

    // Set all LEDs to PWM Control
    TLC59116_WriteReg(TLC59116_LEDOUT0_ADDR, TLC59116_LEDOUT_ON);
    TLC59116_WriteReg(TLC59116_LEDOUT1_ADDR, TLC59116_LEDOUT_ON);
    TLC59116_WriteReg(TLC59116_LEDOUT2_ADDR, TLC59116_LEDOUT_ON);
    TLC59116_WriteReg(TLC59116_LEDOUT3_ADDR, TLC59116_LEDOUT_ON);

}


void I2C_Scan() {
    for (uint8_t address = 0x00; address <= 0xFF; address++) {
        // Try writing a dummy register to the current address
        uint8_t aTXBuffer[] = { 0x00, 0x00 };
        HAL_I2C_Master_Transmit(I2C_DEV, address << 1, aTXBuffer, sizeof(aTXBuffer), 100);        // If there is no error, it means a device responded at this address
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
            // Handle the found address as needed
            // You can print it or do anything else with it
            printf("Device found at address: 0x%02X\n", address);
        }
    }
}


static void TLC59116_WriteReg(uint8_t reg, uint8_t val)
{
    uint8_t aTXBuffer[] = { reg, val };
    while(HAL_I2C_Master_Transmit(I2C_DEV, TLC59116_ADDR_WRITE, aTXBuffer, sizeof(aTXBuffer), 100) != HAL_OK)
        {
            if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF)
            {
                Error_Handler();
            }
        }
}

static void TLC59116_WriteStream(uint8_t len, uint8_t *pData)
{
    if (len == 0)
    {
        Error_Handler();
    }
    while(HAL_I2C_Master_Transmit(I2C_DEV, TLC59116_ADDR_WRITE, pData, len, 100) != HAL_OK)
    {
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF)
        {
            Error_Handler();
        }
    }
}

void TLC59116_LED(uint8_t led, uint8_t state)
{
    if ((led < 0) || (led > 15)) {
        Error_Handler();
    } else {
        TLC59116_WriteReg(TLC59116_LEDOUT0_ADDR, state);
    }
}

void TLC59116_SLEEP(void)
{
    TLC59116_WriteReg(TLC59116_MODE1, TLC59116_MODE1_OFF_STATE);
}

void TLC59116_WAKE(void)
{
    TLC59116_WriteReg(TLC59116_MODE1, TLC59116_MODE1_ON_STATE);
    HAL_Delay(5);
}
