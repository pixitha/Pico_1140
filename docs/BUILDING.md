# Building the Pico Fuzzball branch

The default target is `pico2_w`. The standard Pico W and a third-party RP2040
board can be selected at configure time without editing source.

## Prerequisites

- Raspberry Pi Pico SDK 2.1.1 or a confirmed-compatible later SDK, available
  outside this repository.
- ARM GNU toolchain providing `arm-none-eabi-gcc` and `arm-none-eabi-g++`.
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

