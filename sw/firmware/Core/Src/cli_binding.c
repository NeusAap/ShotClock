/*
 * cli_binding.c
 *
 *  Created on: Jul 7, 2023
 *      Author: NeusAap
 */

#include <stdio.h>
#include <strings.h>

#include "main.h"
#include "tim.h"
#include "cli_setup.h"
#include "cli_binding.h"

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
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        TIM1->CCR2 = 20000;
        return;
    }else if (strcasecmp(arg1, "off") == 0){
        cli_printf("Disable buzzer");
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        return;
    }
    // Make sure to check if 'args' != NULL, printf's '%s' formatting does not like a null pointer.
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
            .name = "get-led",
            .help = "Get led status",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = onGetLed
    };

    // Command binding for the set led command
    CliCommandBinding set_led_binding = {
            .name = "set-led",
            .help = "Set LEDs on/off",
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

    EmbeddedCli *cli = getCliPointer();
    embeddedCliAddBinding(cli, clear_binding);
    embeddedCliAddBinding(cli, get_led_binding);
    embeddedCliAddBinding(cli, set_led_binding);
    embeddedCliAddBinding(cli, set_buzzer_binding);
}
