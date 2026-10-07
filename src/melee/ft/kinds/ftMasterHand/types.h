#ifndef MELEE_FT_CHARA_FTMASTERHAND_TYPES_H
#define MELEE_FT_CHARA_FTMASTERHAND_TYPES_H

#include <Runtime/platform.h>

#include <melee/ft/kinds/ftMasterHand/forward.h> // IWYU pragma: export
#include <melee/it/forward.h>
#include <sysdolphin/baselib/forward.h>

#include <dolphin/mtx.h>
#ifdef PORT
#include <stddef.h> // offsetof, for the item arms below
#endif

struct ftMasterhand_FighterVars {
    /* 0x222C */ HSD_GObj* x222C;
    /* 0x2230 */ u32 x2230;
    /* 0x2234 */ u32 x2234;
    /* 0x2238 */ float x2238;
    /* 0x223C */ float x223C;
    /* 0x2240 */ Vec3 x2240_pos;
    /* 0x224C */ u32 x224C;
    /* 0x2250 */ s32 x2250;
    /* 0x2254 */ s32 x2254;
    /* 0x2258 */ s32 x2258;
};

struct ftMasterHand_SpecialAttrs {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    s32 x14;
    s32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    float x28;
    float x2C;
    Vec2 x30_pos2;
    float x38;
    float x3C;
    Vec3 x40_pos;
    float x4C;
    Vec2 x50;
    float x58;
    float x5C;
    float x60;
    float x64;
    float x68;
    s32 x6C;
    s32 x70;
    s32 x74;
    float x78;
    s32 x7C;
    float x80;
    s32 x84;
    Vec2 x88_pos;
    s32 x90;
    s32 x94;
    float x98;
    float x9C;
    s32 xA0;
    float xA4;
    Vec2 xA8_pos;
    s32 xB0;
    s32 xB4;
    float xB8;
    Vec2 xBC_pos;
    Vec2 xC4_pos;
    Vec2 xCC_pos;
    float xD4;
    float xD8;
    float xDC;
    float xE0;
    float xE4;
    float xE8;
    s32 xEC;
    s32 xF0;
    float xF4;
    float xF8;
    float xFC;
    float x100;
    float x104;
    float x108;
    float x10C;
    Vec2 x110_pos;
    Vec2 x118_pos;
    float x120;
    Vec2 x124_pos;
    Vec2 x12C_pos;
    Vec2 x134_pos;
    Vec2 x13C_pos;
    s32 x144;
    s32 x148;
    float x14C;
    float x150;
    float x154;
    float x158;
    float x15C;
    s32 x160;
    s32 x164;
    s32 x168;
    s32 x16C;
    s32 x170;
    s32 x174;
    float x178;
};

union ftMasterHand_MotionVars {
    struct ftMasterHand_Unk0Vars {
        float x0;
        HSD_GObjEvent x4;
        int x8;
        Vec3 xC;
        float x18;
        float x1C;
        int x20;
        float x24;
        int x28;
        int x2C;
        int x30;
        int x34;
        int x38;
        int x3C;
        int x40;
        int x44;
        int x48;
        int x4C;
        float x50;
        int x54;
        Vec3 x58;
        Vec3 x64;
        int x70;
        int x74;
        int x78;
    } unk0;

    struct ftMasterHand_Unk4Vars {
        ftMasterHand_UnkEnum0 x0;
        int x4;
        int x8;
    } unk4;

    struct ftMasterHand_Unk13Vars {
        float x0;
        float x4;
    } unk13;

    struct ftMasterHand_FingerBeamVars {
#ifdef PORT
        // PORT: unk0.x4 is a function pointer, eight bytes here, so unk0's
        // fields sit later than on the console, and unk0.x30 (the sound
        // handle ftMh_MS_362_80152F80() sets to -1) would land on this x34's
        // low half. The four lasers go past the end of unk0, where no field
        // of it is; the hands' OnLoad clears them. See
        // docs/design/verification.md, "Union arms".
        char pad_0[sizeof(struct ftMasterHand_Unk0Vars)];
#else
        /*  +0 fp+2340 */ char pad_0[0x34];
#endif
        /* +34 fp+2374 */ Item_GObj* x34;
        /* +38 fp+2378 */ Item_GObj* x38;
        /* +3C fp+237C */ Item_GObj* x3C;
        /* +40 fp+2380 */ Item_GObj* x40;
    } fingerbeam;

    struct ftMasterHand_GrabVars {
#ifdef PORT
        // PORT: Crazy Hand's four lasers (ftcrazyhandpoke.c) are 32 bytes
        // here, so from +0x28 they would cover unk0.x20 and the sound
        // handles at unk0.x38..x40. They go past the end of unk0, as
        // fingerbeam's do.
        char pad_0[sizeof(struct ftMasterHand_Unk0Vars)];
#else
        char pad_0[0x28];
#endif
        Item_GObj* x28;
        Item_GObj* x2C;
        Item_GObj* x30;
        Item_GObj* x34;
    } grab;

    struct ftMasterHand_Damage_0 {
#ifdef PORT
        // PORT: x28..x30 are unk0's sound handles and x34..x40 fingerbeam's
        // lasers (ftmasterhanddamage0.c stops and frees both), each placed
        // where that arm has it here.
        char pad_0[offsetof(struct ftMasterHand_Unk0Vars, x28)];
        int x28;
        int x2C;
        int x30;
        char pad_34[sizeof(struct ftMasterHand_Unk0Vars) -
                    offsetof(struct ftMasterHand_Unk0Vars, x34)];
#else
        /*  +0 fp+2340 */ char pad_0[0x28];
        /* +28 fp+2368 */ int x28;
        /* +2C fp+236C */ int x2C;
        /* +30 fp+2370 */ int x30;
#endif
        /* +34 fp+2374 */ Item_GObj* x34;
        /* +38 fp+2378 */ Item_GObj* x38;
        /* +3C fp+237C */ Item_GObj* x3C;
        /* +40 fp+2380 */ Item_GObj* x40;
    } dmg0;

    struct ftCrazyHand_DamageVars {
        /*  +0 fp+2340 */ char pad_0[0x60];
        /* +60 fp+23A0 */ int x60;
        /* +64 fp+23A4 */ int x64;
    } ch_dmg;

    struct ftCrazyHand_BackCrushVars {
        /* +0 fp+2340 */ char pad_0[0x44];
        /* +44 fp+2384 */ Vec3 x44;
        /* +50 fp+2390 */ Vec3 x50;
        /* +5C fp+239C */ int x5C;
    } ch_backcrush;
};

#ifdef PORT
// PORT: the arms that share storage on the console share it here, and the
// item arms overlap no field of unk0.
_Static_assert(offsetof(union ftMasterHand_MotionVars, dmg0.x28) ==
                   offsetof(union ftMasterHand_MotionVars, unk0.x28),
               "damage0 stops the sounds fingerbeam started");
_Static_assert(offsetof(union ftMasterHand_MotionVars, dmg0.x30) ==
                   offsetof(union ftMasterHand_MotionVars, unk0.x30),
               "damage0 stops the sounds fingerbeam started");
_Static_assert(offsetof(union ftMasterHand_MotionVars, dmg0.x34) ==
                   offsetof(union ftMasterHand_MotionVars, fingerbeam.x34),
               "damage0 frees the lasers fingerbeam made");
_Static_assert(offsetof(union ftMasterHand_MotionVars, dmg0.x40) ==
                   offsetof(union ftMasterHand_MotionVars, fingerbeam.x40),
               "damage0 frees the lasers fingerbeam made");
_Static_assert(offsetof(union ftMasterHand_MotionVars, fingerbeam.x34) >=
                   sizeof(struct ftMasterHand_Unk0Vars),
               "Master Hand's lasers overlap no field of unk0");
_Static_assert(offsetof(union ftMasterHand_MotionVars, grab.x28) >=
                   sizeof(struct ftMasterHand_Unk0Vars),
               "Crazy Hand's lasers overlap no field of unk0");
#endif
#endif
