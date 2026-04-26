/**
 ******************************************************************************
 * @file    gpio_driver.h
 * @brief   Thin GPIO driver for STM32F411RE.
 *
 * Con CMSIS integrado, este header ya NO necesita definir GPIO_TypeDef,
 * RCC_TypeDef, ni las direcciones base: stm32f411xe.h los provee todos.
 *
 * Lo que sí vive aquí:
 *   - Constantes de modo/pull legibles (MODE_INPUT, PULLUP, …)
 *   - Prototipos de la mini-API que abstrae la manipulación de bits.
 *
 * Uso directo (estilo CMSIS, sin pasar por la API):
 *   GPIOA->ODR    ^= (1UL << 5U);          // toggle PA5
 *   GPIOC->IDR    &  (1UL << 13U);         // leer PC13
 *   GPIOB->BSRR   =  (1UL << 7U);          // set   PB7
 *   GPIOB->BSRR   =  (1UL << (7U + 16U));  // reset PB7
 ******************************************************************************
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "stm32f4xx.h"   /* GPIO_TypeDef, GPIOA..GPIOH, RCC, RCC_AHB1ENR_GPIOxEN */

/* ── Constantes de modo (campo MODER, 2 bits por pin) ──────────────────── */
#define GPIO_MODE_INPUT     0x00U   /**< Pin como entrada digital             */
#define GPIO_MODE_OUTPUT    0x01U   /**< Pin como salida de propósito general */
#define GPIO_MODE_AF        0x02U   /**< Función alternativa (UART, SPI, …)  */
#define GPIO_MODE_ANALOG    0x03U   /**< Modo analógico (ADC/DAC)            */

/* ── Constantes de pull-up/down (campo PUPDR, 2 bits por pin) ──────────── */
#define GPIO_NOPULL         0x00U   /**< Sin resistencia interna             */
#define GPIO_PULLUP         0x01U   /**< Pull-up interno                     */
#define GPIO_PULLDOWN       0x02U   /**< Pull-down interno                   */

/* ── API ────────────────────────────────────────────────────────────────── */

/**
 * @brief  Habilita el clock del puerto y configura modo y pull de un pin.
 * @param  GPIOx  Puntero CMSIS al puerto: GPIOA, GPIOB, GPIOC, …
 * @param  pin    Número de pin (0–15).
 * @param  mode   GPIO_MODE_INPUT | GPIO_MODE_OUTPUT | …
 * @param  pull   GPIO_NOPULL | GPIO_PULLUP | GPIO_PULLDOWN
 */
void    gpio_init   (GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode, uint32_t pull);

/**
 * @brief  Escribe un estado lógico en un pin de salida usando BSRR
 *         (operación atómica, sin read-modify-write).
 */
void    gpio_write  (GPIO_TypeDef *GPIOx, uint32_t pin, uint8_t state);

/**
 * @brief  Lee el estado lógico del pin desde IDR.
 * @return 1 si el pin está en HIGH, 0 si está en LOW.
 */
uint8_t gpio_read   (GPIO_TypeDef *GPIOx, uint32_t pin);

/**
 * @brief  Invierte el estado actual del pin vía XOR sobre ODR.
 */
void    gpio_toggle (GPIO_TypeDef *GPIOx, uint32_t pin);

#endif /* GPIO_DRIVER_H */
