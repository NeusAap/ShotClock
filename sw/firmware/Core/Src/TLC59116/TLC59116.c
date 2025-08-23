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
#include "cli_setup.h"


// Define the addresses for each display
const DisplayAddresses display_addresses[] = {
    {0xC2, 0xC3},   // Display 1
    {0xC4, 0xC5},   // Display 2
    {0xC8, 0xC9},   // Display 3
    {0xD0, 0xD1}    // Display 4
};
// Function to get the write address based on display number
uint8_t GetWriteAddress(DisplayNumber display) {
    if (display >= DISPLAY_1 && display <= DISPLAY_4) {
        return display_addresses[display - 1].write_addr;
    } else {
        // Handle invalid display number
        return 0x00; // or any other error handling mechanism
    }
}

// Function to get the read address based on display number
uint8_t GetReadAddress(DisplayNumber display) {
    if (display >= DISPLAY_1 && display <= DISPLAY_4) {
        return display_addresses[display - 1].read_addr;
    } else {
        // Handle invalid display number
        return 0x00; // or any other error handling mechanism
    }
}


void TLC59116_Init(void)
{

    // Set all values to 0x00 (OFF)
    uint8_t aTXBuffer[] = {
            TLC59116_PWM0_AUTOINCR,
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00
    };
    for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
    // Set default register values
        TLC59116_WriteReg(TLC59116_MODE1_ADDR, TLC59116_MODE1_ON_STATE, displayNumber);
        TLC59116_WriteReg(TLC59116_MODE2_ADDR, TLC59116_MODE2_DEFAULT, displayNumber);


        TLC59116_WriteStream(sizeof(aTXBuffer), aTXBuffer, displayNumber);

    }
}


void I2C_Scan() {
    for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
        for (uint8_t address = 0x00; address <= 0xFF; address++) {
            // Try writing a dummy register to the current address of the current display
            uint8_t aTXBuffer[] = { 0x00, 0x00 };
            HAL_I2C_Master_Transmit(I2C_DEV, GetWriteAddress(displayNumber), aTXBuffer, sizeof(aTXBuffer), 100); // If no error, a device responded at this address
            if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
                // Handle the found address as needed
                // You can print it or do anything else with it
                cli_printf("Device found at address 0x%02X on display %d\n", address, displayNumber);
            }
        }
    }
}


void TLC59116_WriteReg(uint8_t reg, uint8_t val, uint8_t displayNumber) {
    uint8_t aTXBuffer[] = { reg, val };
    while(HAL_I2C_Master_Transmit(I2C_DEV, GetWriteAddress(displayNumber), aTXBuffer, sizeof(aTXBuffer), 100) != HAL_OK) {
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
            Error_Handler();
        }
    }
}

uint8_t TLC59116_ReadReg(uint8_t reg, uint8_t displayNumber) {
    uint8_t received_data;

    // Write the register address to read from
    while(HAL_I2C_Master_Transmit(I2C_DEV, GetWriteAddress(displayNumber), &reg, sizeof(reg), 100) != HAL_OK) {
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
            Error_Handler();
        }
    }

    // Read data from the specified register
    while(HAL_I2C_Master_Receive(I2C_DEV, GetReadAddress(displayNumber), &received_data, sizeof(received_data), 100) != HAL_OK) {
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
            Error_Handler();
        }
    }

    return received_data;
}

void TLC59116_WriteStream(uint8_t len, uint8_t *pData, uint8_t displayNumber) {
    if (len == 0) {
        Error_Handler();
    }
    while(HAL_I2C_Master_Transmit(I2C_DEV, GetWriteAddress(displayNumber), pData, len, 100) != HAL_OK) {
        if (HAL_I2C_GetError(I2C_DEV) != HAL_I2C_ERROR_AF) {
            Error_Handler();
        }
    }
}

int TLC59116_SetLED(LEDControlParams params, uint8_t displayNumber) {
    uint8_t regAddress, regValue, currentRegValue;

    // Determine register address based on LED number
    if (params.ledNumber >= 1 && params.ledNumber <= 16) {
        // Calculate the LED register address
        regAddress = TLC59116_LEDOUT0_ADDR + ((params.ledNumber - 1) / 4);

        // Read the current value of the register for the specified display
        currentRegValue = TLC59116_ReadReg(regAddress, displayNumber); // Example usage

        // Calculate the bit shift for the LED control value
        uint8_t shift = ((params.ledNumber - 1) % 4) * 2;

        // Prepare the new register value with the control value shifted to the appropriate position
        regValue = (currentRegValue & ~(0x03 << shift)) | (params.controlValue << shift);

        // Update the LED control register with the new value for the specified display
        TLC59116_WriteReg(regAddress, regValue, displayNumber); // Example usage
        return 0;
    } else {
        // LED number out of range
        cli_printf("LED number out of range (1-16)\n");
        return -1;
    }
}


void TLC59116_TurnOffAllLEDs(uint8_t displayNumber) {
    // Iterate through all LED numbers and turn them off
    for (int i = 1; i <= 16; i++) {
        LEDControlParams params;
        params.ledNumber = i;
        params.controlValue = LED_OFF;
        TLC59116_SetLED(params, displayNumber);
    }
}

void TLC59116_TurnOnAllLEDs(uint8_t displayNumber, uint8_t excludeDot) {
    // Iterate through all LED numbers and turn them off
    for (int i = 1; i <= 16; i++) {
        // Skip the dot segments if bool is true, since they stop the showing of all other segments
        if (excludeDot && (i == 8 || i == 16 ))
        {
            continue;
        }
        LEDControlParams params;
        params.ledNumber = i;
        params.controlValue = LED_FULL_ON;
        TLC59116_SetLED(params, displayNumber);
    }
}

// Set brightness of a single LED (0–255)
void TLC59116_SetBrightness(uint8_t ledNumber, uint8_t brightness, uint8_t displayNumber, uint8_t excludeDot) {
    if (ledNumber < 1 || ledNumber > 16) {
        cli_printf("LED number out of range (1–16)\n");
        return;
    }
    if (excludeDot && (ledNumber == 8 || ledNumber == 16 ))
    {
        return;
    }
    // Put LED in PWM control mode
    LEDControlParams params = {
        .ledNumber = ledNumber,
        .controlValue = LED_PWM_CONTROL
    };
    TLC59116_SetLED(params, displayNumber);

    // Map LED number to PWM register
    uint8_t pwmReg = TLC59116_PWM0_ADDR + (ledNumber - 1);

    // Write brightness
    TLC59116_WriteReg(pwmReg, brightness, displayNumber);
}

// Set brightness of all 16 LEDs in one I²C transfer
void TLC59116_SetAllBrightness(uint8_t *values, uint8_t displayNumber) {
    if (values == NULL) {
        Error_Handler(); // invalid input
    }

    uint8_t buffer[17];
    buffer[0] = TLC59116_PWM0_AUTOINCR; // start at PWM0 with auto-increment
    for (int i = 0; i < 16; i++) {
        buffer[i + 1] = values[i]; // copy brightness values
    }

    TLC59116_WriteStream(sizeof(buffer), buffer, displayNumber);

    // Ensure all LEDs are in PWM mode
    for (int i = 1; i <= 16; i++) {
        LEDControlParams params = { .ledNumber = i, .controlValue = LED_PWM_CONTROL };
        TLC59116_SetLED(params, displayNumber);
    }
}


// Set global brightness scaling (0–255)
// All LEDs must be set to LED_PWM_GROUP mode for this to take effect
void TLC59116_SetGlobalBrightness(uint8_t brightness, uint8_t displayNumber) {
    TLC59116_WriteReg(TLC59116_GRPPWM_ADDR, brightness, displayNumber);
}


// Set all LEDs (1–16) on a display to the same mode
void TLC59116_SetAllToMode(LEDControl mode, uint8_t displayNumber) {
    if (mode > LED_PWM_GROUP) {
        cli_printf("Invalid LED mode\n");
        return;
    }

    // Each LEDOUTx register controls 4 LEDs (2 bits per LED)
    // So we need to write 4 registers (LEDOUT0..LEDOUT3)
    uint8_t value = 0;
    for (int i = 0; i < 4; i++) {
        value |= (mode & 0x03) << (i * 2);
    }

    for (int reg = TLC59116_LEDOUT0_ADDR; reg <= TLC59116_LEDOUT3_ADDR; reg++) {
        TLC59116_WriteReg(reg, value, displayNumber);
    }
}

void TLC59116_Sleep(void)
{
    for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_WriteReg(TLC59116_MODE1, TLC59116_MODE1_OFF_STATE, displayNumber);
        }
}

void TLC59116_Wake(void) {
    for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
        TLC59116_WriteReg(TLC59116_MODE1, TLC59116_MODE1_ON_STATE, displayNumber);
    }
}
