/*
 * Eric Sullivan and Elizabeth Willard
 * Microcontroller based calculator
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "lcd-st7302.h"
#include "keypad.h"
#include "calc.h"

void system_clock_init(void);
void LED_Init(void);

#define BUFFER_SIZE 33

/*
 * Entry Point
 */
int main(void)
{
    uint8_t key, map;
    uint8_t mode, ans;
    char exp_buffer[BUFFER_SIZE]; // Input expression buffer
    char post_fix[BUFFER_SIZE]; // Postfix expression buffer
    char *exp_p; // Expression pointer
    char *exp_start;
    double res;

    // Set expression pointer to buffer
    exp_p = exp_buffer;
    exp_start = exp_buffer;

    // Init System Clock
    system_clock_init();

    // Timer Init
    timer_init();

    // Init test LED
    LED_Init();

    // Init LCD
    lcd_init();

    // Init keypad
    init_keypad_pins();

    // Test LED that turns on when everything is set up correctly
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);

    // Set current map 0 and output mode to DEC
    map = 0;
    ans = 0;
    mode = DEC; // Start at integer math 

    while (1) {
      
        // Read key
        key = read_key();

        // Clear screen and expression buffer after writing new problem
        // Does not clear screen if no key is pressed or key is backlight
        // control
        if ((ans && key != 255) && (ans && key != 'l')) {
            lcd_clear();
            memset(exp_buffer, 0, sizeof(exp_buffer));
            exp_p = exp_buffer;
            exp_start = exp_buffer;
            ans = 0;

        }

        // Gey key from key map
        key = map_key(key, map);

        switch (key) {
            case 'c':
                // Clear screen
                lcd_clear();

                // Clear expression buffer
                memset(exp_buffer, 0, sizeof(exp_buffer));

                // Set expression pointers back to buffer
                exp_p = exp_buffer;
                exp_start = exp_buffer;
                break;

            case 'd':
                // Delete char from screen and buffer
                if (exp_p - exp_buffer > 0) {   
                    lcd_del();
                    *exp_p = 0;
                    exp_p--;

                    // Re-adjust screen
                    if (exp_p - exp_buffer > 14) {
                        exp_start--;
                        lcd_clear();
                        lcd_print("%s", exp_start);
                    }
                }

                break;

            case '=':
                // Calculate expression and print answer on next line
                infix_to_postfix(exp_buffer, post_fix);
                if (eval_postfix(post_fix, &res) != 0) {
                    // If expression is invalid, print error message to screen
                    lcd_clear();
                    lcd_print("INVALID");
                    lcd_set_cursor(0, 1);
                    lcd_print("SYNTAX!");
                    ans = 1;
                    res = 0;
                } else if (res > 4294967295 || res < -2147483647) {
                    // If answer is out of range, print error to screen
                    lcd_clear();
                    lcd_print("ANSWER OUT");
                    lcd_set_cursor(0, 1);
                    lcd_print("OF RANGE!");
                    ans = 1;
                    res = 0;
                } else {
                    // Print answer on next line of LCD screen
                    lcd_set_cursor(0, 1);
                    lcd_print_int_mode(res, mode);
                    ans = 1;
                }

                break;

            case 's':
                // Toggle current key map
                map ^= 1;
                break;

            case 'm':
                // Changes current mode the answer is printed
                mode++;

                // This cycles back to the first mode when
                // the max is reached
                if (mode > 4)
                    mode = 0;
                break;
            case 'l':
                // Toggle backlight for LCD
                HAL_GPIO_TogglePin(GPIOC, BACKLIGHT_GPIO);
          
            case 255:
                break;
            
            default:
                // Checks if expression is longer then char limit of LCD screen
                if ((exp_p - exp_buffer > 15) && (exp_p - exp_buffer < (BUFFER_SIZE - 1))) {

                    // Prints screen from new starting point to 'scroll' screen
                    lcd_clear();
                    exp_start++;
                    lcd_print("%s", exp_start);
                }

                if (exp_p - exp_buffer < (BUFFER_SIZE - 1)) {

                    // For special characters
                    if (key == '<')
                        lcd_write_char(0xFB);
                    else if (key == '>')
                        lcd_write_char(0xFC);
                    
                    // NOTE: NEED to add cutsom char to negative
                    else if (key == '_')
                        lcd_write_char(0x00);
                    else
                        // Write char normally
                        lcd_write_char(key);
                    
                    // Save key to expression buffer
                    *exp_p = key;
                    
                    // Move to next char of buffer
                    exp_p++;
                    // Set null character to end (current) end of buffer
                    *exp_p = '\0';
                }
        }

        if (key != 255)
            mdelay(400);
    }
}

/*
 * Initialize System Clock
 * (Need to work on error handling for this)
 */
void system_clock_init(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    RCC_PeriphCLKInitTypeDef per = {0};

    // Init MSI oscillator at 8 Mhz
    osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    osc.MSIState = RCC_MSI_ON;
    osc.MSICalibrationValue = 0;
    osc.MSIClockRange = RCC_MSIRANGE_7;
    osc.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) {
    }

    // Init CPU clock, AHB, APB1, APB2
    clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4) != HAL_OK) {
    }

    // Init perph clock for I2C1
    per.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    per.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&per) != HAL_OK) {
    }

    // Configure the main internal regulator output voltage
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    }
}

/*
 * Init Green LED on Nucleo borad.
 * For testing purposes
 */
void LED_Init(void)
{
    // Init PB13 for Green LED
    GPIO_InitTypeDef green;

    // Enable Clock for GPIOB
    __GPIOB_CLK_ENABLE();

    green.Pin = GPIO_PIN_13;
    green.Mode = GPIO_MODE_OUTPUT_PP; // Output, push-pull
    green.Speed = GPIO_SPEED_FREQ_MEDIUM; // Freq range for 5 MHz to 25 MHz
    green.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &green);
}
