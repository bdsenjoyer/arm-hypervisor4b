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

### QEMU output

```
=================================
ARM Hypervisor v0.1
=================================
--- System Information ---
Exception Level: EL2
Processor MPIDR: 0x0000000080000000
Stack Pointer: 0x0000000040004FC0
--------------------------
[OK] Running in EL2
[INIT] Configuring hypervisor...
  CPTR_EL2: 0 (no traps)
  CNTHCTL_EL2: 3 (EL1 timer access)
  HCR_EL2: 0x0000000080002039
[OK] Hypervisor configured
  64-bit VM: Enabled
  WFI trap: Enabled
  Exception routing: Enabled
  HVC: Enabled
[OK] Exceptions ready
[VM] Guest context allocated
  Size: 288 bytes
[PERF] Boot time: 5118 microseconds
=== Hypercall Interface Test ===
[API] HVC_VERSION -> 0x0000000000010000
[API] HVC_GET_CPU_ID -> CPU0
[OK] Hypercall API verified (7 services)
*** HYPERVISOR INITIALIZED ***
Ready to accept VM in EL1
Hypercall interface operational
```

### Raspberry Pi 4 output

```
=================================
ARM Hypervisor v0.1
=================================
--- System Information ---
Exception Level: EL2
Processor MPIDR: 0x0000000080000000
Stack Pointer: 0x0000000000097FC0
--------------------------
[OK] Running in EL2
[INIT] Configuring hypervisor...
  HCR_EL2: 0x0000000080002038
[OK] Hypervisor configured
[OK] Exceptions ready
[VM] Guest context allocated
  Size: 288 bytes
[PERF] Boot time: 81261 microseconds
[VM] ERET -> EL1 guest at 0x0000000000081000
[HVC] VERSION -> 0x00010000
[HVC] GET_CPU_ID -> CPU0
[HVC] EXIT - guest requested shutdown
=== Hypercall Statistics ===
Total calls: 5
============================
*** HYPERVISOR INITIALIZED ***
Guest exited cleanly from EL1
Trap-and-emulate cycle complete
```

Connect UART on GPIO 14/15 (115200 baud). See `rpi_test/README.txt` for flashing instructions.

---


### Raspberry Pi 4

Copy `rpi_test/kernel8.img` and `rpi_test/config.txt` to a FAT32 SD card. Connect UART on GPIO 14/15 (115200 baud). See `rpi_test/README.txt`.

---

## Background

ARM has 4 privilege levels (EL0–EL3). This runs at EL2 which is the hypervisor level — above the OS but below secure monitor. Stage-2 translation lets the hypervisor control what physical memory guests can see.

Main motivation was embedded/IoT use cases where you want isolation between tasks on one chip without a full OS like Linux.

---

## Author

@bdsenjoyer, Kyiv, 2026
