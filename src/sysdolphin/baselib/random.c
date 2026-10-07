#include "random.h"

#ifdef PORT
// PORT: --trace-rng notes who drew, so a consumer the port has and the
// console does not can be found. Slippi resets the seed every frame, so the
// draw's index decides its value. See docs/design/observers.md, "RNG trace".
#include <port/rng_trace.h>
#define PORT_RNG_NOTE() port_rng_note(__builtin_return_address(0))
// PORT: HSD_Randi() wraps HSD_Rand(), so it attributes rather than counts:
// counting it would put two indices on one draw of the generator.
#define PORT_RNG_VIA() port_rng_via(__builtin_return_address(0))
#endif
static u32 seed = 1;
u32* HSD_RandSeedPtr = &seed;

s32 HSD_Rand(void)
{
#ifdef PORT
    PORT_RNG_NOTE();
#endif
    *HSD_RandSeedPtr = *HSD_RandSeedPtr * 214013 + 2531011;
    return *HSD_RandSeedPtr >> 0x10;
}

f32 HSD_Randf(void)
{
#ifdef PORT
    PORT_RNG_NOTE();
#endif
    *HSD_RandSeedPtr = *HSD_RandSeedPtr * 214013 + 2531011;
    return (f32) (*HSD_RandSeedPtr >> 0x10) / (1 << 16);
}

s32 HSD_Randi(s32 max_val)
{
#ifdef PORT
    PORT_RNG_VIA();
#endif
    return max_val * HSD_Rand() / (1 << 16);
}

void _HSD_RandForgetMemory(void* low, void* high)
{
    if (low <= (void*) HSD_RandSeedPtr && (void*) HSD_RandSeedPtr < high) {
        HSD_RandSeedPtr = &seed;
    }
}
