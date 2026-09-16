/*
 * Four-bit SDIO on GPIO 10-15.
 *
 * Development target: Cytron Maker Pi Pico's onboard microSD socket.
 * Compact-device target: an Adafruit SDIO breakout wired with the same map.
 *
 * CLK=10, CMD=11, D0-D3=12-15. The upstream PIO SDIO driver derives
 * CLK/D1-D3 from D0, so only CMD and D0 are configured here.
 */

#include "hw_config.h"

static sd_sdio_if_t sdio_gp10_15 = {
    .CMD_gpio = 11,
    .D0_gpio = 12,
    // Start conservatively; increase only after physical-card validation.
    .baud_rate = 125 * 1000 * 1000 / 6  // 20,833,333 Hz
};

static sd_card_t sd_card = {
    .type = SD_IF_SDIO,
    .sdio_if_p = &sdio_gp10_15,
};

size_t sd_get_num(void) {
    return 1;
}

sd_card_t *sd_get_by_num(size_t num) {
    return num == 0 ? &sd_card : NULL;
}
