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
of BSS (`481800` bytes total); the UF2 SHA-256 was
`31b909120dd7d11a1c9c007642e37eb52eefe743a947b7c4d2bbf560bd22f98e`.

This is a compile/link result only. It has not been flashed or run. On RP2350,
the legacy FatFs component has no compatible hardware RTC, so it deliberately
returns an unset FAT timestamp until a real board-specific time source is
implemented.
