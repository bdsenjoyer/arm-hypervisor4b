# Lightweight ARM Hypervisor

Bare-metal Type-1 hypervisor for AArch64, written in C and ARM assembly. Made as my bachelor's thesis at NAU Kyiv.

Runs on QEMU and tested on Raspberry Pi 4.

---

## What it does

- Boots at EL2 (hypervisor exception level)
- Sets up Stage-2 address translation to isolate guest memory
- Handles exceptions via custom EL2 vector table
- Simple hypercall interface (HVC) for guest-hypervisor communication
- VM creation and basic lifecycle management
- PL011 UART driver for early debug output
- CPU and memory info reading at runtime

---

## Structure

```
src/
  boot/entry.S            — assembly entry, EL2 init, stack setup
  arch/arm64/exceptions.S — exception vectors
  core/main.c             — C entry point
  core/hypervisor.c       — core logic
  core/vm.c               — vm create/run/destroy
  core/hypercall.c        — HVC handler
  core/uart.c             — UART driver (polling)
  core/cpu_info.c         — read CPU registers/features
  core/memory_info.c      — physical memory layout
  core/guest.S            — guest bootstrap
include/                  — headers
hypervisor.ld             — linker script
rpi_test/                 — prebuilt image for RPi 4
```

---

## Build and run

You need `aarch64-linux-gnu-gcc` and `qemu-system-aarch64`.

```bash
make        # build
make run    # run on QEMU
make clean  # clean build artifacts
```

Should print something like:
```
Hypervisor started!
EL2 initialized
...
```

### Raspberry Pi 4

Copy `rpi_test/kernel8.img` and `rpi_test/config.txt` to a FAT32 SD card. Connect UART on GPIO 14/15 (115200 baud). See `rpi_test/README.txt`.

---

## Background

ARM has 4 privilege levels (EL0–EL3). This runs at EL2 which is the hypervisor level — above the OS but below secure monitor. Stage-2 translation lets the hypervisor control what physical memory guests can see.

Main motivation was embedded/IoT use cases where you want isolation between tasks on one chip without a full OS like Linux.

---

## Author

[Your Name], NAU Kyiv, 2025
