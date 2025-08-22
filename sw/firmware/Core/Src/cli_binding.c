/*
 * cli_binding.c
 *
 *  Created on: Jul 7, 2023
 *      Author: NeusAap
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "main.h"
#include "tim.h"
#include "cli_setup.h"
#include "cli_binding.h"
#include "TLC59116.h"
#include "segment_display.h"
#include "adc.h"

void onClearCLI(EmbeddedCli *cli, char *args, void *context) {
    cli_printf("\33[2J");
}

void onGetLed(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1);
    const char *arg2 = embeddedCliGetToken(args, 2);
    if (arg1 == NULL || arg2 == NULL) {
        cli_printf("usage: get-led [arg1] [arg2]");
        return;
    }
    // Make sure to check if 'args' != NULL, printf's '%s' formatting does not like a null pointer.
    cli_printf("LED with args: %s and %s", arg1, arg2);
}

void onSetLed(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1);
    const char *arg2 = embeddedCliGetToken(args, 2);
    if (arg1 == NULL || arg2 == NULL) {
        goto usage;
        return;
    }

    if (strcasecmp(arg1, "0") == 0){
        if (strcasecmp(arg2, "on") == 0){
            cli_printf("Enabled LED 0.");
            HAL_GPIO_WritePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, GPIO_PIN_SET);
            return;
        }else if(strcasecmp(arg2, "off") == 0){
            cli_printf("Disabled LED 0.");
            HAL_GPIO_WritePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, GPIO_PIN_RESET);
            return;
        }else{
            goto usage;
            return;
        }
    }
    usage:
        cli_printf("usage: set-led [led-num] [off/on]");
}

void onBuzzer(EmbeddedCli *cli, char *args, void *context) {
    const char *arg1 = embeddedCliGetToken(args, 1);
    const char *arg2 = embeddedCliGetToken(args, 2);
    if (arg1 == NULL || arg2 != NULL) {
        cli_printf("usage: buzzer [off/on]");
        return;
    }
    if (strcasecmp(arg1, "on") == 0){
        cli_printf("Enable buzzer");
        TIM1->CCR2 = 50;
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        return;
    }else if (strcasecmp(arg1, "off") == 0){
        cli_printf("Disable buzzer");
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        return;
    }
}

void onI2CSetReg(EmbeddedCli *cli, char *args, void *context) {
    const char *displayNumberStr = embeddedCliGetToken(args, 1);
    const char *address = embeddedCliGetToken(args, 2);
    const char *value = embeddedCliGetToken(args, 3);
    const char *null_check = embeddedCliGetToken(args, 4);
    if (displayNumberStr == NULL || address == NULL || value == NULL || null_check != NULL) {
        cli_printf("usage: i2c-set [disp-number(1-4)] [address] [value]");
        return;
    }
    uint8_t displayNumber = (uint8_t)atoi(displayNumberStr);
    uint8_t i2cAddress = (uint8_t)strtol(address, NULL, 16); // Convert to hexadecimal
    uint8_t i2cValue = (uint8_t)strtol(value, NULL, 16); // Convert to hexadecimal

    TLC59116_WriteReg(i2cAddress, i2cValue, displayNumber);
}

void onI2CGetReg(EmbeddedCli *cli, char *args, void *context) {
    const char *displayNumberStr = embeddedCliGetToken(args, 1);
    const char *address = embeddedCliGetToken(args, 2);
    const char *null_check = embeddedCliGetToken(args, 3);
    if (displayNumberStr == NULL || address == NULL || null_check != NULL) {
        cli_printf("usage: i2c-get [disp-number(1-4)] [reg-address]");
        return;
    }
    uint8_t displayNumber = (uint8_t)atoi(displayNumberStr);
    uint8_t regAddress = (uint8_t)strtol(address, NULL, 16); // Convert to hexadecimal
    uint8_t result = TLC59116_ReadReg(regAddress, displayNumber);
    cli_printf("Read reg at addr: 0x%x. Value: 0x%x for display: %d", regAddress, result, displayNumber);

}

void setSegment(EmbeddedCli *cli, char *args, void *context){
    const char *displayNumberStr = embeddedCliGetToken(args, 1);
    const char *onOffStr = embeddedCliGetToken(args, 2);
    const char *segmentStr = embeddedCliGetToken(args, 3);
    const char *null_check = embeddedCliGetToken(args, 4);
    if (displayNumberStr == NULL || onOffStr == NULL || segmentStr == NULL || null_check != NULL) {
        cli_printf("usage: seg [disp-number(1-4)] [on/off] [0-16]");
        return;
    }
    uint8_t displayNumber = (uint8_t)atoi(displayNumberStr);
    LEDControlParams params;
    uint8_t led_num = (uint8_t)atoi(segmentStr);
    params.ledNumber = led_num;
    if (strcasecmp(onOffStr, "on") == 0){
        cli_printf("Enabled segment %d for display: %d", led_num, displayNumber);
        params.controlValue = LED_FULL_ON;
    }else if (strcasecmp(onOffStr, "off") == 0){
        cli_printf("Disabled segment %d for display: %d", led_num, displayNumber);
        params.controlValue = LED_OFF;
    }
    TLC59116_SetLED(params, displayNumber);

}

void setDigits(EmbeddedCli *cli, char *args, void *context){
    const char *displayNumberStr = embeddedCliGetToken(args, 1);
    const char *onOffStr = embeddedCliGetToken(args, 2);
    const char *null_check = embeddedCliGetToken(args, 3);

    if (displayNumberStr == NULL || onOffStr == NULL || null_check != NULL) {
        cli_printf("usage: dig [disp-number(1-4)] [0-99/off/on]");
        return;
    }
    uint8_t displayNumber = (uint8_t)atoi(displayNumberStr);

    if (strcasecmp(onOffStr, "off") == 0){
        cli_printf("Turning all digits off.");
        for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_TurnOffAllLEDs(displayNumber);
        }
        return;
    }else if(strcasecmp(onOffStr, "on") == 0){
        cli_printf("Turning all digits on.");
        for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_TurnOnAllLEDs(displayNumber, 0);
        }
        return;
    }

    uint8_t digit_to_display = (uint8_t)atoi(onOffStr);
    int did_display = SegmentDisp_DisplayNumber(digit_to_display, displayNumber);
    if (did_display == 0){
        cli_printf("Set the digits: %d", digit_to_display);
    }
}

void setCountdown(EmbeddedCli *cli, char *args, void *context){
    const char *displayNumberStr = embeddedCliGetToken(args, 1);
    const char *onOffStr = embeddedCliGetToken(args, 2);
    const char *null_check = embeddedCliGetToken(args, 3);
    if (displayNumberStr == NULL || onOffStr == NULL || null_check != NULL) {
        cli_printf("usage: cnt [disp-number(1-4)] [0-99/off]");
        return;
    }
    uint8_t displayNumber = (uint8_t)atoi(displayNumberStr);

    if (strcasecmp(onOffStr, "off") == 0){
        cli_printf("Turning all digits off and stopping count-down.");
        SegmentDisp_SetCountdown(0, displayNumber);
        return;
    }
    uint8_t count_down_amt = (uint8_t)atoi(onOffStr);
    int did_start = SegmentDisp_SetCountdown(count_down_amt, displayNumber);
    if (did_start == 0){
        cli_printf("Set count-down timer to start at: %d", count_down_amt);
    }
}

void getBatteryVoltage(EmbeddedCli *cli, char *args, void *context) {
    float voltage = GetBatteryVoltage();
    int voltage_int = (int)(voltage * 100); // Scale and convert to integer
    int voltage_decimal = voltage_int % 100; // Extract decimal part
    int voltage_whole = voltage_int / 100; // Extract whole part
    cli_printf("Voltage: %d.%02dV", voltage_whole, voltage_decimal);
}

void setAllDisplaysSegments(EmbeddedCli *cli, char *args, void *context)
{
    const char *onOffStr = embeddedCliGetToken(args, 1);
    const char *null_check = embeddedCliGetToken(args, 2);
    if (onOffStr == NULL || null_check != NULL) {
        cli_printf("usage: seg-all [on/off]");
        return;
    }

    if (strcasecmp(onOffStr, "off") == 0){
        cli_printf("Turning all segments off.");
        for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_TurnOffAllLEDs(displayNumber);
        }
        return;
    }if (strcasecmp(onOffStr, "on") == 0)
    {
        cli_printf("Turning all digits on.");
        for (uint8_t displayNumber = 1; displayNumber <= 4; displayNumber++) {
            TLC59116_TurnOnAllLEDs(displayNumber, 1);
        }
    }
}


void initCliBinding() {
    // Define bindings as local variables, so we don't waste static memory

    // Command binding for the clear command
    CliCommandBinding clear_binding = {
            .name = "clear",
            .help = "Clears the console",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onClearCLI
    };

    // Command binding for the get led command
    CliCommandBinding get_led_binding = {
            .name = "get-onboard-led",
            .help = "Get LEDs on the PCB status",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onGetLed
    };

    // Command binding for the set led command
    CliCommandBinding set_onboard_led_binding = {
            .name = "set-onboard-led",
            .help = "Set LEDs on the PCB on/off",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onSetLed
    };

    CliCommandBinding set_buzzer_binding = {
            .name = "buzzer",
            .help = "Set buzzer on/off",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onBuzzer
    };

    CliCommandBinding set_i2c_binding = {
            .name = "i2c-set",
            .help = "Write an i2c register @ 0x61 (hex unit)",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onI2CSetReg
    };

    CliCommandBinding get_i2c_binding = {
            .name = "i2c-get",
            .help = "Read an i2c register @ 0x61 (hex unit)",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onI2CGetReg
    };

    CliCommandBinding set_segment_binding = {
            .name = "seg",
            .help = "Enable a single segment 0-16",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = setSegment
    };

    CliCommandBinding set_digits_binding = {
            .name = "dig",
            .help = "Sets digits between 0-99 or off",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = setDigits
    };

    CliCommandBinding set_countdown_binding = {
            .name = "cnt",
            .help = "Sets count-down between 0-99 seconds or off",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = setCountdown
    };

    CliCommandBinding get_battvolt_binding = {
            .name = "batt",
            .help = "Gets the battery voltage from ADC",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = getBatteryVoltage
    };

    CliCommandBinding set_all_segments_binding = {
            .name = "seg-all",
            .help = "Sets all the segments on/off on all displays",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = setAllDisplaysSegments
    };


    EmbeddedCli *cli = getCliPointer();
    embeddedCliAddBinding(cli, clear_binding);
    embeddedCliAddBinding(cli, get_led_binding);
    embeddedCliAddBinding(cli, set_onboard_led_binding);
    embeddedCliAddBinding(cli, set_buzzer_binding);
    embeddedCliAddBinding(cli, set_i2c_binding);
    embeddedCliAddBinding(cli, get_i2c_binding);
    embeddedCliAddBinding(cli, set_segment_binding);
    embeddedCliAddBinding(cli, set_digits_binding);
    embeddedCliAddBinding(cli, set_countdown_binding);
    embeddedCliAddBinding(cli, get_battvolt_binding);
    embeddedCliAddBinding(cli, set_all_segments_binding);
}
