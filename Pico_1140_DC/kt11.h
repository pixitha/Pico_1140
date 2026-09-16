// kt11.h — KT11-C MMU (Mills DCN compatible)
//
// Register layout:
//   Supervisor: PAR 172200-172216, PDR 172220-172236
//   Kernel:     PAR 172300-172316, PDR 172320-172336
//   User:       PAR 177600-177616, PDR 177620-177636
//   MMR0/SR0:   177572   MMR1/SR1: 177574   MMR2/SR2: 177576
//
// modes: pages[0]=kernel, pages[1]=supervisor, pages[2]=illegal, pages[3]=user

#pragma once
#include "avr11.h"
#include <array>
#include <stdint.h>
#include <stdio.h>

class KT11 {

  public:
    std::array<uint16_t, 4> SR;

    template <bool wr>
    inline uint32_t decode(const uint16_t a, const uint16_t mode) {
        if ((SR[0] & 1) == 0) {
            return a > 0157777 ? ((uint32_t)a) + 0600000 : a;
        }
        const auto i = (a >> 13);
 
        if (wr && !pages[mode][i].write()) {
            SR[0] = (1 << 13) | 1;
            SR[0] |= (a >> 12) & ~1;
            if (mode) {
                SR[0] |= (1 << 5) | (1 << 6);
            }
            //SR2 = cpu.PC;

            //printf("mmu::decode write to read-only page %06o\n", a);
            trap(0250); // intfault
        }
        if (!pages[mode][i].read()) {
            SR[0] = (1 << 15) | 1;
            SR[0] |= (a >> 12) & ~1;
            if (mode) {
                SR[0] |= (1 << 5) | (1 << 6);
            }
            // SR2 = cpu.PC;
            printf("mmu::decode read from no-access page %06o\n", a);
            trap(0250); // intfault
        }
        const auto block = (a >> 6) & 0177;
        const auto disp = a & 077;
        // if ((p.ed() && (block < p.len())) || (!p.ed() && (block > p.len())))
        // {
        if (pages[mode][i].ed() ? (block < pages[mode][i].len())
                                : (block > pages[mode][i].len())) {
            SR[0] = (1 << 14) | 1;
            SR[0] |= (a >> 12) & ~1;
            if (mode) {
                SR[0] |= (1 << 5) | (1 << 6);
            }
            // SR2 = cpu.PC;
            //printf("page length exceeded, address %06o (block %03o) is beyond "
            //       "length "
            //       "%03o\r\n",
            //       a, block, pages[mode][i].len());
            SR[0] |= 0200;
            trap(0250); // intfault
        }
        pages[mode][i].pdr |= PDR_A;
        if constexpr (wr) {
            pages[mode][i].pdr |= PDR_W;
        }
        const auto aa = ((pages[mode][i].addr() + block) << 6) + disp;
        // printf("decode: slow %06o -> %06o\n", a, aa);
        return aa;
    }

    uint16_t read16(uint32_t a);
    void write16(uint32_t a, uint16_t v);

    struct page {
        uint16_t par, pdr;

        inline uint32_t addr() { return par & 07777; }
        inline uint8_t len() { return (pdr >> 8) & 0x7f; }
        inline bool read() { return (pdr & 2) == 2; }
        inline bool write() { return (pdr & 6) == 6; };
        inline bool ed() { return pdr & 8; }
    };

    static constexpr uint16_t PDR_W = 0000100;
    static constexpr uint16_t PDR_A = 0000200;

    std::array<std::array<struct page, 16>, 4> pages;
    void dumppages();
};
