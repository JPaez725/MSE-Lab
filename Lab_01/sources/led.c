/**
 ******************************************************************************
 * @file    led.c
 * @brief   Control de LD2 (PA5) en la Nucleo-F411RE usando registros CMSIS.
 *
 * Accesos directos al hardware (sin abstracción intermedia):
 *   RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;   habilita clock GPIOA
 *   GPIOA->MODER  → configura PA5 como salida (MODER=01)
 *   GPIOA->BSRR   → set/reset atómico (preferido sobre ODR para set/clear)
 *   GPIOA->ODR    → toggle vía XOR
 ******************************************************************************
 */

#include "led.h"
#include "stm32f4xx.h"   /* GPIOA, RCC, RCC_AHB1ENR_GPIOAEN – provistos por CMSIS */

#define LED_PIN     5U   /* PA5 = LD2 en la Nucleo-F411RE */

void led_init(void)
{
    /* Habilitar clock del bus AHB1 para GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA5 → modo salida de propósito general (MODER[11:10] = 01b)
     * Paso 1: borrar los 2 bits del campo para evitar glitch al escribir */
    GPIOA->MODER &= ~(3UL << (LED_PIN * 2U));
    /* Paso 2: escribir 01b (output) */
    GPIOA->MODER |=  (1UL << (LED_PIN * 2U));
}

void led_on(void)
{
    /* BSRR bits [15:0] → set (BS5 = bit 5) */
    GPIOA->BSRR = (1UL << LED_PIN);
}

void led_off(void)
{
    /* BSRR bits [31:16] → reset (BR5 = bit 21) */
    GPIOA->BSRR = (1UL << (LED_PIN + 16U));
}

void led_toggle(void)
{
    /* XOR: invierte solo el bit del LED */
    GPIOA->ODR ^= (1UL << LED_PIN);
}
