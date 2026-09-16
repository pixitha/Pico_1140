# Building the Pico Fuzzball branch

The default target is `pico2_w`. The standard Pico W and a third-party RP2040
board can be selected at configure time without editing source.

## Prerequisites

- Raspberry Pi Pico SDK 2.1.1 or a confirmed-compatible later SDK, available
  outside this repository.
- Complete Arm GNU toolchain providing `arm-none-eabi-gcc`,
  `arm-none-eabi-g++`, newlib, and `nosys.specs`. The smaller Homebrew
  `arm-none-eabi-gcc` formula alone is insufficient for Pico linking; the
  complete `gcc-arm-embedded` cask provides these runtime pieces.
- CMake and Ninja.

The SDK and all build products are local dependencies. Do not commit them.

## Configure and build

```bash
cd /Users/kduren/projects/pdp11_rt11/Pico_1140-worktrees/fuzzball
cmake -S Pico_1140_DC -B build/pico2w -G Ninja \
  -DPICO_SDK_PATH=/absolute/path/to/pico-sdk \
  -DPICO_BOARD=pico2_w
cmake --build build/pico2w
```

For an RP2040 Pico W, use `-DPICO_BOARD=pico_w`. The 16 MiB third-party board
needs its exact SDK board definition; do not assume its flash geometry from the
generic Pico W target.

The resulting UF2 is only build evidence. A separate hardware record must
capture board model, SDK revision, UF2 hash, serial output, SD wiring, and the
copied disk image used for the boot attempt.

## Current build evidence

On 2026-09-16, this branch built successfully for `pico2_w` with Pico SDK
2.1.1 (`bddd20f928ce76142793bef434d4f75f4af6e433`) and Arm GNU Toolchain
15.3.1. The resulting ELF reported `244936` bytes of text and `236864` bytes
of BSS (`481800` bytes total).  The RP2350 build selects its supported
150 MHz system clock; the legacy RP2040 build retains its 200 MHz setting.
The corrected Pico 2 W UF2 SHA-256 was
`d0a8fed459af38c5995147429f751f7a7a612f9456354ddde48269fc63fce719`.

This image was flashed to a Raspberry Pi Pico 2 W in BOOTSEL mode.  It
enumerated a USB serial device (`/dev/cu.usbmodem101`) and reached the SD SPI
initializer.  With no responding card on the configured legacy SPI pins, it
reported `No response CMD:0`, then halted with `f_mount error` 3 (physical
drive cannot work).  This establishes firmware execution and USB console
operation only; it is not an RT-11 or Fuzzball boot result.

The current inherited SD configuration is SPI1 with GPIO 12 (MISO), GPIO 15
(MOSI), GPIO 14 (SCK), and GPIO 9 (card select).  Before the next test, wire
a 3.3 V-compatible SPI SD adapter to those GPIOs and ground, then use a FAT32
card containing a *copy* of the intended RT-11 disk image.  Do not connect a
5 V SD adapter directly to Pico GPIO.

On RP2350, the legacy FatFs component has no compatible hardware RTC, so it
deliberately returns an unset FAT timestamp until a real board-specific time
source is implemented.
