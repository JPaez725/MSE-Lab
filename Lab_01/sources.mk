## sources.mk
## ---------------------------------------------------------------------------
## Build inputs: fuentes e includes del proyecto.
## Para agregar un archivo nuevo, añadirlo explícitamente a SRC.
## Para usar todos los .c automáticamente, comentar SRC y descomentar
## la línea con $(wildcard ...).
## ---------------------------------------------------------------------------

SRC_DIR         = sources
INCLUDE_DIR     = includes
CMSIS_CORE_DIR  = CMSIS/Core/include
CMSIS_STM32_DIR = CMSIS/STM32F4xx/include

## ── Source files (lista explícita – más legible y segura) ─────────────────
SRC =                               \
    $(SRC_DIR)/main.c               \
    $(SRC_DIR)/STM32_startup.c      \
    $(SRC_DIR)/system_stm32f4xx.c   \
    $(SRC_DIR)/gpio_driver.c        \
    $(SRC_DIR)/led.c                \
    $(SRC_DIR)/button.c

## Alternativa automática (descomentar si quieres recoger todos los .c):
## SRC = $(wildcard $(SRC_DIR)/*.c)

## ── Include paths ──────────────────────────────────────────────────────────
INCLUDES =              \
    -I$(INCLUDE_DIR)    \
    -I$(CMSIS_CORE_DIR) \
    -I$(CMSIS_STM32_DIR)
