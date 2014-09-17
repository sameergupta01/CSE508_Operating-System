#include <defs.h>
#include <io.h>
#include <stdio.h>
#include <kbc.h>
#include <kbc_scancodes.h>

void kbc_irq()
{
    static uint8_t shift = 0;

    uint8_t b = inb(0x60);

    switch(b)
    {
        case KBCSC_LEFTSHIFT:
            shift |= 1;
            break;
        case KBCSC_RIGHTSHIFT:
            shift |= 2;
            break;
        case (KBCSC_LEFTSHIFT | 0x80):
            shift &= ~1;
            break;
        case (KBCSC_RIGHTSHIFT | 0x80):
            shift &= ~2;
        default:
            break;
    }

    uint8_t c = kbcsc_tochar(b, shift);
    if(c != 0)
    {
        print_cx_cy(52,24,"%c", c);
        printf("%c", c);		//for echo
    }
}


