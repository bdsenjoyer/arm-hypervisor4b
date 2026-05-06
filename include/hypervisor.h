#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <stdint.h>

uint64_t read_hcr_el2(void);
void write_hcr_el2(uint64_t hcr);
void hypervisor_init(void);
void show_virtualization_status(void);

#endif
