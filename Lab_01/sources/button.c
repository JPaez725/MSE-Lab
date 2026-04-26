/**
 ******************************************************************************
 * @file    button.c
 * @brief   Lectura con debounce de B1 (PC13) en la Nucleo-F411RE.
 *
 * Hardware
 * ────────
 *   B1 → PC13, active-LOW (externo 4.7 kΩ pull-up, sin pull interna).
 *   IDR = 0 cuando el botón está PRESIONADO.
 *   IDR = 1 cuando el botón está SUELTO.
 *
 * Debounce
 * ────────
 *   Registro de desplazamiento de 8 bits (history):
 *   - Se rellena con la lectura cruda en cada llamada.
 *   - 0x00 (8 muestras seguidas en LOW)  → BUTTON_PRESSED
 *   - 0xFF (8 muestras seguidas en HIGH) → BUTTON_RELEASED
 *   - Cualquier patrón mixto             → estado sin cambio (ruido)
 *
 * La función usa gpio_driver para inicializar el pin; la lectura
 * cruda accede directamente a GPIOC->IDR vía gpio_read().
 ******************************************************************************
 */

#include "button.h"
#include "gpio_driver.h"   /* gpio_init, gpio_read; incluye stm32f4xx.h */

#define BUTTON_PIN  13U    /* PC13 = B1 */

void button_init(void)
{
    /* PC13: entrada digital, sin pull interna (la placa ya tiene pull-up externo) */
    gpio_init(GPIOC, BUTTON_PIN, GPIO_MODE_INPUT, GPIO_NOPULL);
}

uint8_t button_get_state(void)
{
    /* Historial: 0xFF = suelto (todas las muestras en HIGH) */
    static uint8_t history = 0xFFU;
    static uint8_t state   = BUTTON_RELEASED;

    /* Desplazar 1 bit a la izquierda y meter la lectura actual en el LSB.
     * gpio_read devuelve 0 cuando el botón está presionado (active-low). */
    history = (uint8_t)((history << 1U) | gpio_read(GPIOC, BUTTON_PIN));

    if      (history == 0x00U) { state = BUTTON_PRESSED;  }
    else if (history == 0xFFU) { state = BUTTON_RELEASED; }
    /* Si history es mixto, el estado no cambia → inmune a rebotes */

    return state;
}
