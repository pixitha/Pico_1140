// kt11.cxx — KT11-C MMU emulation (Mills DCN compatible layout)
//
// MMU register layout (KT11-C, matching Mills' SUP.MAC):
//   Supervisor: PAR at 172200-172216, PDR at 172220-172236
//   Kernel:     PAR at 172300-172316, PDR at 172320-172336
//   User:       PAR at 177600-177616, PDR at 177620-177636
//
// The upstream avr11 layout had PAR/PDR swapped (PDR at base, PAR at
// base+40), which is incompatible with the DCN supervisor.

#include "kt11.h"
#include <stdint.h>
#include <stdio.h>

uint16_t KT11::read16(const uint32_t a) {
    const auto i = ((a & 017) >> 1);
    switch (a & ~017) {
    case 0772200:                       // super PAR0-7
        return pages[01][i].par;
    case 0772220:                       // super PDR0-7
        return pages[01][i].pdr;
    case 0772300:                       // kernel PAR0-7
        return pages[00][i].par;
    case 0772320:                       // kernel PDR0-7
        return pages[00][i].pdr;
    case 0777600:                       // user PAR0-7
        return pages[03][i].par;
    case 0777620:                       // user PDR0-7
        return pages[03][i].pdr;
    default:
        printf("mmu::read16 invalid read from %06o\n", a);
        trap(004); // intbus
    }
}

void KT11::write16(const uint32_t a, const uint16_t v) {
    const auto i = ((a & 017) >> 1);
    switch (a & ~017) {
    case 0772200:                       // super PAR0-7
        pages[01][i].par = v & 07777;
        pages[01][i].pdr &= ~PDR_W;
        break;
    case 0772220:                       // super PDR0-7
        pages[01][i].pdr = v & 077416;
        break;
    case 0772300:                       // kernel PAR0-7
        pages[00][i].par = v & 07777;
        pages[00][i].pdr &= ~PDR_W;
        break;
    case 0772320:                       // kernel PDR0-7
        pages[00][i].pdr = v & 077416;
        break;
    case 0777600:                       // user PAR0-7
        pages[03][i].par = v & 07777;
        pages[03][i].pdr &= ~PDR_W;
        break;
    case 0777620:                       // user PDR0-7
        pages[03][i].pdr = v & 077416;
        break;
    default:
        printf("mmu::write16 write to invalid address %06o\n", a);
        trap(004); // intbus
    }
}
