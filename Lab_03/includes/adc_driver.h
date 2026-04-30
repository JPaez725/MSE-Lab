/**
 * @file    adc_driver.h
 * @brief   Low-level ADC driver for STM32F4xx (CMSIS / bare-metal).
 *
 * Covers SRS FR-1 through FR-9:
 *   FR-1  adc_init                    – clock enable, default state
 *   FR-2  adc_enable                  – ADON bit
 *   FR-3  adc_set_channel             – regular sequence channel
 *   FR-4  adc_set_injected_channel    – injected sequence channel
 *   FR-5  adc_start_single            – one-shot SWSTART
 *   FR-6  adc_start_continuous        – CONT mode
 *   FR-7  adc_start_injected          – JSWSTART
 *   FR-8  adc_read_data               – DR register (regular)
 *   FR-9  adc_read_injected_data      – JDR1–JDR4 (injected)
 *
 * Hardware assumptions:
 *   - STM32F411RE, ADC1 only (single ADC device on this MCU)
 *   - 12-bit resolution (default, CR1.RES = 00)
 *   - Right-aligned data (CR2.ALIGN = 0)
 *   - Software trigger for regular and injected conversions
 *   - Single channel, single regular sequence (SQR1.L = 0)
 *   - ADC clock = PCLK2 / 4 = 16 MHz / 4 = 4 MHz (within spec)
 */

#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include "stm32f411xe.h"
#include <stdint.h>
#include <stddef.h>

/* ====================================================================
 * Constants
 * ==================================================================== */

/** @brief Maximum valid regular/injected channel number on STM32F411. */
#define ADC_MAX_CHANNEL         (18U)

/** @brief Maximum injected channel rank (1–4). */
#define ADC_MAX_INJECTED_RANK   (4U)

/** @brief Full-scale value for 12-bit resolution. */
#define ADC_FULL_SCALE          (4095U)

/* ====================================================================
 * Enumerations
 * ==================================================================== */

typedef enum { ADC_OK = 0U, ADC_ERROR = 1U } ADC_Status_t;

/** @brief Sample time options (written to SMPRx, 3 bits per channel). */
typedef enum
{
    ADC_SAMPLETIME_3   = 0U,
    ADC_SAMPLETIME_15  = 1U,
    ADC_SAMPLETIME_28  = 2U,
    ADC_SAMPLETIME_56  = 3U,
    ADC_SAMPLETIME_84  = 4U,
    ADC_SAMPLETIME_112 = 5U,
    ADC_SAMPLETIME_144 = 6U,
    ADC_SAMPLETIME_480 = 7U
} ADC_SampleTime_t;

/* ====================================================================
 * Public API  (FR-1 … FR-9)
 * ==================================================================== */

/**
 * FR-1 – Initialise the ADC subsystem.
 *
 * Steps:
 *  1. Enable ADC1 clock on APB2.
 *  2. Set ADC common prescaler to /4 (CCR.ADCPRE = 01).
 *  3. Configure CR1: 12-bit resolution (RES = 00), scan mode off.
 *  4. Configure CR2: right-align, software trigger (SWSTART/JSWSTART),
 *     single conversion mode (CONT = 0).
 *  5. Set regular sequence length to 1 (SQR1.L = 0).
 *
 * @param  adc  ADC peripheral pointer (e.g. ADC1).
 */
ADC_Status_t adc_init(ADC_TypeDef *adc);

/**
 * FR-2 – Enable the ADC (set CR2.ADON).
 *        Requires a stabilisation delay before the first conversion;
 *        the caller is responsible for waiting (~1 µs / a few NOPs).
 */
ADC_Status_t adc_enable(ADC_TypeDef *adc);

/**
 * FR-3 – Configure the first slot of the regular conversion sequence.
 *
 * @param  channel      Channel number 0–18.
 * @param  sample_time  Sample time selection (ADC_SAMPLETIME_x).
 */
ADC_Status_t adc_set_channel(ADC_TypeDef *adc, uint8_t channel,
                              ADC_SampleTime_t sample_time);

/**
 * FR-4 – Configure an injected channel slot.
 *
 * @param  channel      Channel number 0–18.
 * @param  rank         Injected rank 1–4.
 * @param  sample_time  Sample time selection.
 */
ADC_Status_t adc_set_injected_channel(ADC_TypeDef *adc, uint8_t channel,
                                      uint8_t rank,
                                      ADC_SampleTime_t sample_time);

/**
 * FR-5 – Start a single regular conversion (CR2.SWSTART = 1).
 *         Clears EOC before starting so the caller can poll it cleanly.
 */
ADC_Status_t adc_start_single(ADC_TypeDef *adc);

/**
 * FR-6 – Start continuous regular conversions (CR2.CONT = 1, then SWSTART).
 *         Conversions repeat until adc_stop_continuous() disables CONT.
 */
ADC_Status_t adc_start_continuous(ADC_TypeDef *adc);

/** @brief Stop continuous mode (clear CR2.CONT and CR2.ADON). */
ADC_Status_t adc_stop_continuous(ADC_TypeDef *adc);

/**
 * FR-7 – Start an injected conversion (CR2.JSWSTART = 1).
 */
ADC_Status_t adc_start_injected(ADC_TypeDef *adc);

/**
 * FR-8 – Read the regular conversion result from DR.
 *         Polls EOC (blocking) then returns the 12-bit value.
 *
 * @return 12-bit ADC result (0–4095).
 */
uint16_t adc_read_data(ADC_TypeDef *adc);

/**
 * FR-9 – Read the result of an injected channel from JDR1–JDR4.
 *         Polls JEOC (blocking) then returns the result for @p rank.
 *
 * @param  rank  Injected rank 1–4.
 * @return 12-bit ADC result (0–4095), or 0 on invalid rank.
 */
uint16_t adc_read_injected_data(ADC_TypeDef *adc, uint8_t rank);

#endif /* ADC_DRIVER_H */