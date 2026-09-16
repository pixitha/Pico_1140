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

`maker_pi_pico_sdio` is the default storage profile for development on the
Cytron Maker Pi Pico. It configures four-bit SDIO on GPIO 10--15. A compact
device using an external Adafruit SDIO breakout may retain that exact wiring
and select `-DPICO_FUZZBALL_STORAGE_PROFILE=external_sdio_gp10_15`; it is the
same electrical profile, explicitly named for the device build. A future
external-SPI profile will be added after its physical pin assignment is fixed.

For an RP2040 Pico W, use `-DPICO_BOARD=pico_w`. The 16 MiB third-party board
needs its exact SDK board definition; do not assume its flash geometry from the
generic Pico W target.

The resulting UF2 is only build evidence. A separate hardware record must
capture board model, SDK revision, UF2 hash, serial output, SD wiring, and the
copied disk image used for the boot attempt.

## Current build evidence

On 2026-09-16, this branch built successfully for `pico2_w` with Pico SDK
2.1.1 (`bddd20f928ce76142793bef434d4f75f4af6e433`) and Arm GNU Toolchain
15.3.1. The RP2350 build selects its supported 150 MHz system clock; the
legacy RP2040 build retains its 200 MHz setting.

The current configuration vendors upstream
`no-OS-FatFS-SD-SDIO-SPI-RPi-Pico` revision
`d5e453404cdbfaa55ab30d285b6ab0b730e84a05`. It selects the Maker Pi Pico
onboard socket's four-bit SDIO interface: CLK=GPIO 10, CMD=GPIO 11, and
D0--D3=GPIO 12--15. It uses a conservative 20,833,333 Hz initial baud rate.
The resulting ELF reported `278136` bytes of text and `237096` bytes of BSS
(`515232` bytes total); its UF2 SHA-256 was
`5f9754ce8ac01063600fea8957d38228719ffa1ce2d7c563600f1a67e1b24a40`.

This is a compile/link result only. It must be flashed only after the Pico 2 W
has been seated in the Maker Pi Pico board and a FAT32 microSD card is in the
onboard socket. Use a *copy* of the intended RT-11 disk image. The SDIO
library's current RP2350 timer implementation supplies an unset FAT timestamp
until a real time source has been set; this does not prevent read-only media
testing.

The preceding SPI-only baseline used the legacy driver and did execute on a
bare Pico 2 W: its USB console reached SPI initialization, then correctly
reported no response because no card was wired to its legacy SPI pins. It is
historical hardware evidence only; the current image uses SDIO and has not
been physically tested.
