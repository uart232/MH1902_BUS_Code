#include "mhscpu.h"
#include "delay.h"
__ASM void delayus(uint32_t usec)
{
    ALIGN
    MOV r1,#24
    MUL r0,r1
    NOP
    NOP
    NOP
loop
    SUBS r0,#1
    NOP
    BNE loop
    NOP
    BX lr
}

void delayms(uint32_t ms)
{
    if(ms==0)return;
    for(; ms>0; ms--)
    {
        delayus(1000);
    }
}








































