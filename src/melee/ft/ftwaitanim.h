#ifndef GALE01_08A698
#define GALE01_08A698

#include <melee/ft/forward.h>

typedef struct WaitStruct {
    union WaitStruct_u {
        struct WaitStruct_u_p {
#if defined(PORT) || defined(LINT)
            // PORT: both words are integers, and the DAT transcoder converts
            // a pointer field the file does not relocate to null. The only
            // reader, getAnimID() (ft/ftwaitanim.c), stops at
            // `u.i.x == -1`, sums `u.i.y` as percentage weights and returns
            // `u.p.x` cast to `enum_t`: one word, read through either arm.
            // PlKb.dat holds 31 and 80 here, idle animation 31 (the one
            // ftCo_8008A7A8() tests for by number) at an 80% chance. Nulled,
            // the terminator is 0 and every weight zero, so the loop runs
            // past the table. `u.i` already says int.
            int x;
            int y;
#else
            int* x;
            int* y;
#endif
        } p;
        struct WaitStruct_u_i {
            int x;
            int y;
        } i;
    } u;
} WaitStruct;

/* 08A698 */ bool ftCo_8008A698(Fighter* fp);
/* 08A6D8 */ void ftCo_8008A6D8(Fighter_GObj* gobj, s32 anim_id);
/* 08A7A8 */ void ftCo_8008A7A8(Fighter_GObj* gobj, WaitStruct* arg1);
/* 3C54A8 */ extern char ftWaitAnim_803C54A8[];
/* 3C54C4 */ extern char ftWaitAnim_803C54C4[];

#endif
