#include "TLC59116.h"
#include "PCB_REV2.h"
#include "cli_setup.h"
#include "tim.h"


int cntDownNumber = 0;
int buzzer_on = 0;

// Function to display a number on the 7-segment display between 00 and 99
int SegmentDisp_DisplayNumber(int number, uint8_t displayNumber) {
    // Validate input number
    if (number < 0 || number > 99) {
        cli_printf("Error: Number out of range (0-99)\n");
        return -1;
    }

    // Extract digits
    int digit1 = number / 10;
    int digit2 = number % 10;

    // Set LED configurations for the first digit
    for (int i = 0; i < 7; ++i) {
        LEDControlParams params;
        params.ledNumber = i + 1; // LED 1 to 7
        params.controlValue = digitConfigREV2[digit1][i] ? LED_FULL_ON : LED_OFF;
        TLC59116_SetLED(params, displayNumber);
    }

    // Set LED configurations for the second digit
    for (int i = 0; i < 7; ++i) {
        LEDControlParams params;
        params.ledNumber = i + 9; // LED 9 to 15
        params.controlValue = digitConfigREV2[digit2][i] ? LED_FULL_ON : LED_OFF;
        TLC59116_SetLED(params, displayNumber);
    }
    return 0;
}

int SegmentDisp_SetCountdown(int number, uint8_t displayNumber){
    // Validate input number
    if (number < 0 || number > 99) {
        cli_printf("Error: Number out of range (0-99)\n");
        return -1;
    }

    if (number == 0){
        HAL_TIM_Base_Stop_IT(&htim6);
        for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_TurnOffAllLEDs(displayNumber);
        }
        return 0;
    }
    MX_TIM6_Init();
    //__HAL_TIM_SetCounter(&htim6, 9999);
    cntDownNumber = number + 1;
    SegmentDisp_DisplayNumber(cntDownNumber, displayNumber);
    HAL_TIM_Base_Start_IT(&htim6);
    return 0;
}

// Timer interrupt callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        // Decrement the number
        if (cntDownNumber >= 1) {
            cntDownNumber--;
            for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
                SegmentDisp_DisplayNumber(cntDownNumber, displayNumber);
            }
        }
        if(cntDownNumber == 0){
            if (buzzer_on == 1){
                buzzer_on = 0;
                cli_printf("Disable buzzer");
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
                HAL_TIM_Base_Stop_IT(&htim6);
                for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
                    TLC59116_TurnOffAllLEDs(displayNumber);
                }
                cli_printf("Finished count-down!");
            }else{
                buzzer_on = 1;
                TIM1->CCR2 = 50;
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
                cli_printf("Enable buzzer");
            }
        }
    }
}
