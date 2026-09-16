/*
 * Maker Pi Pico SDIO configuration.
 *
 * The onboard microSD socket exposes the native SDIO signals on GPIO 10-15:
 * CLK=10, CMD=11, and D0-D3=12-15. The upstream PIO SDIO driver derives
 * CLK/D1-D3 from D0, so only CMD and D0 are configured here.
 */

#include "hw_config.h"

static sd_sdio_if_t maker_pi_pico_sdio = {
    .CMD_gpio = 11,
    .D0_gpio = 12,
    // Start conservatively; increase only after physical-card validation.
    .baud_rate = 125 * 1000 * 1000 / 6  // 20,833,333 Hz
};

static sd_card_t maker_pi_pico_sd_card = {
    .type = SD_IF_SDIO,
    .sdio_if_p = &maker_pi_pico_sdio,
};

size_t sd_get_num(void) {
    return 1;
}

sd_card_t *sd_get_by_num(size_t num) {
    return num == 0 ? &maker_pi_pico_sd_card : NULL;
}
