/**
 ******************************************************************************
 * @file    gpio_driver.c
 * @brief   Implementación de la mini-API GPIO usando registros CMSIS.
 *
 * Todos los accesos a periféricos usan los punteros y máscaras que provee
 * stm32f411xe.h (incluido a través de gpio_driver.h → stm32f4xx.h):
 *
 *   GPIOA, GPIOB, GPIOC …    →  GPIO_TypeDef *
 *   RCC->AHB1ENR              →  registro de habilitación de clock
 *   RCC_AHB1ENR_GPIOAEN       →  máscara para GPIOA  (bit 0)
 *   RCC_AHB1ENR_GPIOCEN       →  máscara para GPIOC  (bit 2)
 *   …
 ******************************************************************************
 */

#include "gpio_driver.h"

/* ── Función auxiliar: habilita el clock AHB1 del puerto dado ───────────── */
static void rcc_gpio_clk_enable(GPIO_TypeDef *GPIOx)
{
    if      (GPIOx == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; }
    else if (GPIOx == GPIOB) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; }
    else if (GPIOx == GPIOC) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; }
    else if (GPIOx == GPIOD) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; }
    else if (GPIOx == GPIOE) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; }
    /* F411RE solo expone hasta GPIOE en sus pines físicos */
}

/* ─────────────────────────────────────────────────────────────────────────── */

void gpio_init(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode, uint32_t pull)
{
    /* 1. Clock del puerto */
    rcc_gpio_clk_enable(GPIOx);

    /* 2. MODER[2*pin+1 : 2*pin] = mode
     *    Ejemplo pin=5, mode=OUTPUT(01):
     *      GPIOx->MODER &= ~(11b << 10)   → borra los 2 bits
     *      GPIOx->MODER |=  (01b << 10)   → escribe modo salida  */
    GPIOx->MODER &= ~(3UL << (pin * 2U));
    GPIOx->MODER |=  (mode << (pin * 2U));

    /* 3. PUPDR[2*pin+1 : 2*pin] = pull */
    GPIOx->PUPDR &= ~(3UL << (pin * 2U));
    GPIOx->PUPDR |=  (pull << (pin * 2U));
}

void gpio_write(GPIO_TypeDef *GPIOx, uint32_t pin, uint8_t state)
{
    /* BSRR: bits [15:0]  → set individual   (BS)
     *       bits [31:16] → reset individual (BR)
     * Operación atómica: no necesita deshabilitar interrupciones. */
    if (state) { GPIOx->BSRR =  (1UL << pin);          }   /* set   */
    else       { GPIOx->BSRR =  (1UL << (pin + 16U));  }   /* reset */
}

uint8_t gpio_read(GPIO_TypeDef *GPIOx, uint32_t pin)
{
    return ((GPIOx->IDR & (1UL << pin)) != 0U) ? 1U : 0U;
}

void gpio_toggle(GPIO_TypeDef *GPIOx, uint32_t pin)
{
    /* XOR sobre ODR: solo afecta el bit del pin */
    GPIOx->ODR ^= (1UL << pin);
}
