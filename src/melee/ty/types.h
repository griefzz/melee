#ifndef MELEE_TY_TYPES_H
#define MELEE_TY_TYPES_H

#include <Runtime/platform.h>

#include <melee/ty/forward.h> // IWYU pragma: export
#include <sysdolphin/baselib/forward.h>

#include <placeholder.h>

#include <dolphin/gx/GXStruct.h>
#include <dolphin/mtx.h>

struct TySortElem {
    s32 key;
    f32 val;
};

struct TyModeState {
    s8 x0;
    u8 x1;
    u8 x2;
    u8 x3;
    s8 x4;
    u8 x5;
    u16 x6;
    u16 x8;
    u16 xA;
};

struct ToyAnimState {
    /* 0x00 */ struct HSD_GObj* gobj;
    /* 0x04 */ struct HSD_JObj* jobj[2];
    /* 0x0C */ s16 xC;
    /* 0x0E */ s8 x0E;
    /* 0x0F */ s8 x0F;
    /* 0x10 */ s8 x10;
    /* 0x11 */ s8 x11;
    /* 0x12 */ u8 pad_12[2];
};
ASSERT_SIZE(ToyAnimState, 0x14);

/* Used by _Toy_803109A0 for table lookup */
struct ToyEntry {
    s32 id;
    union ToyEntry_x4 {
        s8 value_byte;
        s32 value;
    } x4;
};

/* Trophy metadata entry. Size: 0x24 bytes. */
struct TrophyData {
    s32 id;
    s32 x04;
    f32 x08;
    f32 x0C;
    f32 x10;
    f32 x14;
    f32 x18;
    f32 x1C;
    s8 x20;
    s8 x21;
    s8 x22;
    s8 x23;
};
#if defined(PORT) || defined(LINT)
/// PORT: one row of TyDataf.dat's `tyModelFileTbl`: which archive a trophy's
/// model lives in and what it is called inside it. The decomp has no type
/// for it (`_Toy_sbss_804D6EA8` is a `void*` walked by byte offset), and an
/// untyped root converts to nothing. The shape is Toy_80308250()'s: it hands
/// `ptr + 4` to lbArchive_LoadSymbols() as the file name and `ptr + 0x24` as
/// the symbol, and Toy_8030813C() strides the table by 0x54 comparing
/// `*(s32*) ptr` to a trophy id. The file agrees: no relocations, and the
/// second root begins at 0x6024, 293 (TY_TROPHY_COUNT) * 0x54. Guarded for
/// LINT too, so that the DAT schema's console pass sees it.
struct TyModelFileEntry {
    /* 0x00 */ s32 trophy_id;
    /* 0x04 */ char archive[0x20];
    /* 0x24 */ char symbol[0x30];
};
#endif

/* Trophy list entry. Size: 0x34 bytes. */
struct TyListArg {
    /* 0x00 */ struct TyListArg* links[3];
    /* 0x0C */ struct HSD_JObj* jobjs[3];
    /* 0x18 */ struct HSD_Text* texts[3];
    /* 0x24 */ s8 x24;
    /* 0x25 */ u8 x25;
    /* 0x26 */ s16 idx;
    /* 0x28 */ int x28;
    /* 0x2C */ float x2C;
    /* 0x30 */ float x30;
};

struct Toy {
    /*   +0 */ char pad_0[0x4];
    /*   +4 */ int x4;
    /*   +8 */ int x8;
    /*   +C */ char pad_C[0x40 - 0xC];
    /*  +40 */ Vec3 translate;
    /*  +4C */ Vec3 offset;
    /*  +58 */ char pad_58[0x194 - 0x58];
    /* +194 */ s32 x194;
    /* +198 */ char pad_198[0x19A - 0x198];
    /* +19A */ u16 x19A;
    /* +19C */ u16 x19C;
    /* +19E */ u16 trophyTable[TY_TROPHY_COUNT];
    /* +3E8 */ char pad_3E8[0x3EC - 0x3E8];
    /* +3EC */ s16 trophyCount;
};
/// @TODO: This struct should only be 0x58
// STATIC_ASSERT(sizeof(struct Toy) == 0x58);

struct TyDspEntry {
    /* 0x00 */ s32 x00;
    /* 0x04 */ u8 x04;
    /* 0x05 */ u8 x05;
    /* 0x06 */ u8 pad_06[2];
    /* 0x08 */ f32 x08;
    /* 0x0C */ f32 x0C;
};
ASSERT_SIZE(struct TyDspEntry, 0x10);

struct ToySubStructS_ {
#if defined(PORT) || defined(LINT)
    // PORT: a third view of ToyListEntry: Toy_80310324() reaches the display
    // list's selected entry through it and reads the trophy id (x10).
    // `u8 pad0[0x10]` is that entry's four leading pointers written as
    // console bytes, half their size here, which puts `x10` on
    // `symbol_name`. ty/toy.c asserts the views agree, for both targets.
    /* 0x00 */ void* pad0[4]; ///< prev, next, archive_name, symbol_name
#else
    u8 pad0[0x10];
#endif
    s16 x10;
};

struct ToyGlobalsS_ {
    HSD_GObj* x0;
    u8 x4;
    HSD_GObj* x8;
    HSD_GObj* xC;
    s32 x10;
    u8 pad14[0x1C];
    void* x30;
    u8 pad34[0x1C];
    void* x50;
    HSD_Archive* x54;
    s32 x58;
    u8 pad0[0x140 - 0x5C];
    ToySubStructS_* x140;
    void* x144;
    void* x148;
    void* x14C;
    void* x150;
    s16 x154;
};

struct TyFiguponED4 {
    /* 0x00 */ u32 x0;
    /* 0x04 */ u32 x4;
    /* 0x08 */ u8 pad_08[0x4];
    /* 0x0C */ u32 xC;
};

struct ToyModelFile {
    /* 0x00 */ s32 trophy_id;
    /* 0x04 */ char archive_name[0x20];
    /* 0x24 */ char symbol_name[0x30];
};

struct ToyListEntry {
    /* 0x00 */ struct ToyListEntry* prev;
    /* 0x04 */ struct ToyListEntry* next;
    /* 0x08 */ char* archive_name;
    /* 0x0C */ char* symbol_name;
    /* 0x10 */ s16 trophy_id;
    /* 0x12 */ u8 pad_12[2];
    /* 0x14 */ HSD_Archive* archive;
};

struct TyDisplayData {
    /* 0x000 */ ToyListEntry entries[13];
    /* 0x138 */ ToyListEntry* first_entry;
    /* 0x13C */ ToyListEntry* last_entry;
    /* 0x140 */ ToyListEntry* selected_entry;
#if defined(PORT) || defined(LINT)
    // PORT: the trophy display list. Several structs are cast over this one
    // object, and here they agree only with the same member sequence, so
    // this is the whole layout and the other views are gone. `pad_144` is
    // the four HSD_Text* that tyDispData names.
    /* 0x144 */ HSD_Text* texts[4];
#else
    /* 0x144 */ u8 pad_144[0x154 - 0x144];
#endif
    /* 0x154 */ s16 selectedIdx;
    /* 0x156 */ u8 pad_156;
    /* 0x157 */ s8 visible_count;
};

struct Toy26B8 {
    /* 0x000 */ Vec3 x0;
    /* 0x00C */ u8 pad_00C[0x195 - 0x00C];
    /* 0x195 */ s8 x195;
    /* 0x196 */ s8 x196;
    /* 0x197 */ u8 x197;
    /* 0x198 */ u8 x198;
    /* 0x199 */ u8 pad_199;
    /* 0x19A */ u16 x19A;
    /* 0x19C */ u16 x19C;
    /* 0x19E */ u16 trophy_flags[TY_TROPHY_COUNT];
    /* 0x3E8 */ s16 selectedIdx;
    /* 0x3EA */ s16 selectedTrophyId;
    /* 0x3EC */ s16 trophy_count;
    /* 0x3EE */ u8 pad_3EE[0x3F0 - 0x3EE];
    /* 0x3F0 */ union Toy26B8_x3F0 {
        ToyAnimState anim;
        HSD_GObj* x3F0;
    } x3F0_u;
};

struct _Toy_804A26B8_t {
    struct Toy26B8* x0;
    UNK_T x4;
    UNK_T x8;
};
ASSERT_SIZE(struct _Toy_804A26B8_t, 0xC);

struct TyViewData {
    HSD_GObj* gobj;
    s8 x4;
    char pad_5[0x3];
};
ASSERT_SIZE(struct TyViewData, 0x8);

struct TyFiguponData {
    /* 0x00 */ HSD_GObj* x0;
    /* 0x04 */ HSD_GObj* x4;
    /* 0x08 */ HSD_GObj* x8;
    /* 0x0C */ u8 pad_0C[0x4];
    /* 0x10 */ s32 x10;
    /* 0x14 */ HSD_Text* x14;
    /* 0x18 */ HSD_Text* x18;
    /* 0x1C */ u8 pad_1C[0x4];
    /* 0x20 */ s32 x20;
    /* 0x24 */ s32 x24;
    /* 0x28 */ u8 x28;
    /* 0x29 */ u8 x29;
};

struct TyDspPos {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
};

struct TyDspGrid {
    /* 0x000 */ s32 x00;
    /* 0x004 */ f32 x04_min_x;
    /* 0x008 */ f32 x08_min_z;
    /* 0x00C */ f32 x0C_max_x;
    /* 0x010 */ f32 x10_max_z;
    /* 0x014 */ TySortElem sort[301];
    /* 0x97C */ TyDspPos pos[301];
};

struct TyDspConfig {
    /* 0x00 */ HSD_GObj* x00;
    /* 0x04 */ u8 pad_04[4];
    /* 0x08 */ s32 x08;
    /* 0x0C */ f32 x0C;
    /* 0x10 */ f32 x10;
    /* 0x14 */ u8 pad_14[4];
    /* 0x18 */ f32 x18;
    /* 0x1C */ f32 x1C;
    /* 0x20 */ f32 x20;
    /* 0x24 */ f32 x24;
    /* 0x28 */ u8 pad_28[8];
    /* 0x30 */ f32 x30;
    /* 0x34 */ f32 x34;
    /* 0x38 */ u8 pad_38[8];
    /* 0x40 */ f32 x40;
    /* 0x44 */ f32 x44;
    /* 0x48 */ f32 x48;
    /* 0x4C */ f32 x4C;
    /* 0x50 */ f32 x50;
    /* 0x54 */ f32 x54;
    /* 0x58 */ f32 x58;
    /* 0x5C */ Vec3 x5C;
    /* 0x68 */ Vec3 x68;
    /* 0x74 */ s8 x74;
    /* 0x75 */ u8 x75;
    /* 0x76 */ u8 x76;
    /* 0x77 */ u8 pad_77[1];
    /* 0x78 */ HSD_GObj* x78;
    /* 0x7C */ s32 x7C;
};

struct TyDspArchNames {
    const char* entries[43];
};

struct TyDspNameTables {
    const char* jobj_names[43];
    const char* matanim_names[43];
    TyDspArchNames arch_names;
    s32 terminator;
};

struct TyDspBgData {
    /* 0x00 */ HSD_GObj* gobj0;
    /* 0x04 */ HSD_GObj* gobj4;
    /* 0x08 */ u8 pad_08[4];
    /* 0x0C */ HSD_JObj* jobj;
    /* 0x10 */ u8 pad_10[0x3C];
    /* 0x4C */ HSD_Archive* archive;
    /* 0x50 */ HSD_Archive* archives[43];
    /* 0xFC */ u8 pad_FC[8];
    /* 0x104 */ s16 x104;
    /* 0x106 */ u8 pad_106[2];
};

struct TyDspArchiveHolder {
#if defined(PORT) || defined(LINT)
    // PORT: a ToyListEntry by another name: Toy_80308250() fills one with a
    // trophy's archive name, symbol name and id, and Toy_803087F4() reads it
    // as a ToyEntryData. `pad_4[0x10]` is that entry's `next`,
    // `archive_name` and `symbol_name` (three pointers) plus its id, and
    // here would put `archive` at 24 where the other views put it at 40.
    /*  +0 */ struct ToyListEntry* x0; ///< ToyListEntry::prev
    /*  +4 */ struct ToyListEntry* next;
    /*  +8 */ char* archive_name;
    /*  +C */ char* symbol_name;
    /* +10 */ s16 trophy_id;
    /* +12 */ u8 pad_12[2];
#else
    /*  +0 */ UNK_T x0;
    /*  +4 */ u8 pad_4[0x10];
#endif
    /* +14 */ HSD_Archive* archive;
};

struct TyDspBaseData {
    /* 0x00 */ char x00[0x38];
};

struct TyListGobjEntry {
    /*  +0 */ HSD_GObj* x0;
    /*  +4 */ HSD_GObj* x4;
    /*  +8 */ u8 pad_8[0x0C - 0x08];
    /*  +C */ s8 x0C;
    /*  +D */ s8 x0D;
    /*  +E */ u8 pad_0E;
    /*  +F */ s8 x0F;
    /* +10 */ s8 x10;
    /* +11 */ s8 x11;
    /* +12 */ s8 x12;
    /* +13 */ s8 x13;
    /* +14 */ s8 x14;
    /* +15 */ u8 pad_15;
    /* +16 */ s8 x16;
};

struct TyListRow {
#if defined(PORT) || defined(LINT)
    // PORT: a second view of TyListArg: every caller of _tyList_80312904()
    // hands it an element of TyListState::entries. The two byte pads are
    // that struct's three links and other two joints, five pointers, so here
    // `jobj` would read links[2] and every field below it part of another
    // pointer. The same member sequence; ty/tylist.c asserts the two agree,
    // for both targets.
    /* 0x00 */ struct TyListArg* links[3];
    /* 0x0C */ HSD_JObj* jobj;         ///< TyListArg::jobjs[0]
    /* 0x10 */ HSD_JObj* jobjs_12[2];  ///< TyListArg::jobjs[1], [2]
#else
    /* 0x00 */ u8 pad_0[0xC];
    /* 0x0C */ HSD_JObj* jobj;
    /* 0x10 */ u8 pad_10[0x18 - 0x10];
#endif
    /* 0x18 */ HSD_Text* text0;
    /* 0x1C */ HSD_Text* text1;
    /* 0x20 */ HSD_Text* text2;
    /* 0x24 */ s8 x24;
    /* 0x25 */ u8 pad_25;
    /* 0x26 */ s16 idx;
    /* 0x28 */ s32 x28;
    /* 0x2C */ u8 pad_2C[0x30 - 0x2C];
    /* 0x30 */ f32 x30;
};

struct DigitInit {
    s32 x0, x4, x8, xC;
};

struct ToyNameData {
    s16 x0;
    s16 x2;
    s16 x4;
    s16 x6;
    s16 x8;
    s16 xA;
};

struct tyUnkStruct {
    /* 0x00 */ HSD_GObj* x0;
    /* 0x04 */ void* x4;
    /* 0x08 */ HSD_GObj* x8;
};

struct TyCleanupObj {
    /* 0x00 */ void* x0;
    /* 0x04 */ void* x4;
    /* 0x08 */ void* x8;
    /* 0x0C */ void* xC;
    /* 0x10 */ void* x10;
};

struct TyGObjX8_ {
    u8 pad[0x28];
    HSD_CObj* x28;
};

struct TyCameraData_ {
    void* x0;
    void* x4;
    TyGObjX8_* x8;
    u8 padC[0x18 - 0x0C];
    f32 x18;
    f32 x1C;
    f32 x20;
    f32 x24;
    f32 x28;
    f32 x2C;
    u8 pad30[0x58 - 0x30];
    s32 x58;
};

struct ToyDataJObj {
    /* 0x00 */ void* x0;
    /* 0x04 */ struct ToyDataJObj* x4;
    /* 0x08 */ u8 pad08[0x40 - 0x08];
    /* 0x40 */ s32 x40;
};

#if defined(PORT) || defined(LINT)
// PORT: the front of an HSD_GObj in console byte offsets, up to `hsd_obj`
// at 0x28. The structs below are that prefix with a differently typed
// `hsd_obj` after it: the trophy code holds a gobj and reaches its scene
// object by hand. The decomp writes `u8 pad[0x28]`; here that offset is 64,
// as what is above it holds six pointers and a u64. Spelled as one prefix so
// the views cannot drift; toy.c asserts it against gobj.h for both targets.
#define TOY_GOBJ_PREFIX                                                       \
    /* +00 */ u8 head[8];     /* classifier .. user_data_kind */              \
    /* +08 */ void* links[4]; /* next, prev, next_gx, prev_gx */              \
    /* +18 */ void* proc;                                                     \
    /* +1C */ void* render_cb;                                                \
    /* +20 */ u64 gxlink_prios
#endif
struct ToyDataX8 {
#if defined(PORT) || defined(LINT)
    TOY_GOBJ_PREFIX;
#else
    /* 0x00 */ u8 pad0[0x28];
#endif
    /* 0x28 */ ToyDataJObj* x28;
};

struct tyLightData {
    /* 0x00 */ char _pad0[0x0C];
    /* 0x0C */ HSD_GObj* x0C;
    /* 0x10 */ char _pad1[0x48];
    /* 0x58 */ void* x58;
};

struct tyDispData {
    u8 pad[0x144];
    HSD_Text* x144;
    HSD_Text* x148;
    HSD_Text* x14C;
    HSD_Text* x150;
};

struct un_804D6E68_t {
    /* 0x00 */ u8 pad[0x18];
    /* 0x18 */ f32 x18;
};
#if defined(PORT) || defined(LINT)
/// PORT: the same gobj prefix as ToyDataX8. toy.c takes `hsd_obj` off this
/// and walks the HSD_SObj list behind it, which HSD_SObjLib_803A477C() built
/// on that gobj (_Toy_803078E4()).
typedef struct Toy26B8_2 {
    TOY_GOBJ_PREFIX;
    /* +28 */ void* hsd_obj; ///< HSD_GObj::hsd_obj, which is what upstream's
                        ///< HSD_GObj* spelling of this slot reaches.
} Toy26B8_2;
#endif

#ifdef PORT
#include <sysdolphin/baselib/sobjlib.h>
// PORT: an HSD_SObj (gobj2's hsd_obj list, which _Toy_803078E4() builds)
// read at the console's offsets: x4 is `next` and x40 is `x40`, at +8 and
// +0x50 here. At the console's offsets the gallery's A toggle writes 8 or 9
// into x38_u, the sprite's colour. The pads come from HSD_SObj; the call
// sites keep their names.
struct ToyJObjNode {
    u8 x0[offsetof(HSD_SObj, next)];
    void* x4; // HSD_SObj.next
    u8 x8[offsetof(HSD_SObj, x40) - offsetof(HSD_SObj, next) - sizeof(void*)];
    s32 x40; // HSD_SObj.x40
};
#else
struct ToyJObjNode {
    u8 x0[0x4];
    void* x4;
    u8 x8[0x40 - 0x8];
    s32 x40;
};
#endif

struct ToyCameraControl {
    /*  +0 */ HSD_GObj* x00;
    /*  +4 */ HSD_GObj* x04;
    /*  +8 */ HSD_GObj* x08;
    /*  +C */ HSD_Archive* archive;
    /* +10 */ s32 x10;
    /* +14 */ f32 x14;
    /* +18 */ f32 x18;
    /* +1C */ Vec3 positions[8];
    /* +7C */ Vec3 interests[8];
    /* +DC */ s8 has_position_anim[8];
};
ASSERT_SIZE(ToyCameraControl, 0xE4);

struct ToyTransitionObj {
    u8 pad[0x20];
    s32 x20;
    s32 x24;
};

struct Toy6E68 {
    ToyTransitionObj* x0;
    ToyTransitionObj* x4;
    void* x8;
    ToyTransitionObj* xC;
#if defined(PORT) || defined(LINT)
    // PORT: two gobjs, not a pad: _Toy_8030FA50() stores the screen and
    // overlay gobjs as `void** state[4]/[5]` and Toy_80310660() frees them
    // through the same view. The two agree only while all six leading slots
    // are pointers.
    HSD_GObj* x10;
    HSD_GObj* x14;
#else
    u8 pad10[0x18 - 0x10];
#endif
    f32 x18;
    f32 x1C;
    f32 x20;
    f32 x24;
    f32 x28;
    f32 x2C;
    f32 x30;
    f32 x34;
    f32 x38;
    f32 x3C;
    f32 x40;
    f32 x44;
    f32 x48;
    f32 x4C;
    f32 x50;
    f32 x54;
    s32 x58;
    s32 x5C;
    s8 x60;
    s8 x61;
};

struct Ty25Entry {
#if defined(PORT) || defined(LINT)
    // PORT: a fourth view of ToyListEntry: Toy_80310660() walks the display
    // list's thirteen entries through it to release their archives (x14).
    // `u8 pad[0x14]` is four pointers and the id written as console bytes,
    // so here the stride would be 32 against 48 and `x14` would sit on
    // `symbol_name`. ty/toy.c asserts the offset and the size agree, for
    // both targets.
    /* 0x00 */ void* pad0[4]; ///< prev, next, archive_name, symbol_name
    /* 0x10 */ s16 x10;       ///< ToyListEntry::trophy_id
    /* 0x12 */ u8 pad_12[2];
#else
    u8 pad[0x14];
#endif
    void* x14;
};

/* Trophy list UI state. Size: 0x2D8 bytes. */
struct TyListState {
    /* 0x000 */ TyListArg entries[12]; /* 12 * 0x34 = 0x270 */
    /* 0x270 */ struct TyListArg* x270;
    /* 0x274 */ struct TyListArg* x274;
    /* 0x278 */ struct TyListArg* x278;
    /* 0x27C */ struct HSD_GObj* gobj;
    /* 0x280 */ u8 pad_280[0x8];
    /* 0x288 */ struct HSD_JObj* x288;
    /* 0x28C */ struct HSD_JObj* jobj;
    /* 0x290 */ struct HSD_Text* x290;
    /* 0x294 */ u8 pad_294[4];
    /* 0x298 */ s16 selectedIdx;
    /* 0x29A */ s8 entryCount;
    /* 0x29B */ s8 x29B;
    /* 0x29C */ s8 x29C;
    /* 0x29D */ s8 x29D;
    /* 0x29E */ s8 x29E;
    /* 0x29F */ s8 x29F;
    /* 0x2A0 */ s8 x2A0;
    /* 0x2A1 */ s8 x2A1;
    /* 0x2A4 */ float x2A4;
    /* 0x2A8 */ float x2A8;
};
ASSERT_SIZE(struct TyListState, 0x2AC);

struct TyListData {
#if defined(PORT) || defined(LINT)
    // PORT: the list camera's gobj, read by its render callback
    // (_tyList_80314504()): the same gobj prefix as ToyDataX8, and `cobj` is
    // HSD_GObj::hsd_obj. `u8 pad[0x28]` is 40 bytes on both targets and
    // hsd_obj is at 64 here, so `cobj` would read `proc`.
    TOY_GOBJ_PREFIX;
#else
    u8 pad[0x28];
#endif
    HSD_CObj* cobj;
};

struct TyListWaitData {
    u8 pad[0x20];
    u32 x20;
    s32 x24;
};

/// @todo = ToyGlobalsS_
/// @todo = TyArchiveData
/// @todo = tyLightData
struct ToyED8Data {
    /*  +0 */ HSD_JObj** x0;
    /*  +4 */ HSD_GObj* gobj;
    /*  +8 */ ToyDataX8* x8;
#if defined(PORT) || defined(LINT)
    /*  +C */ Toy26B8_2* gobj2;
    /// PORT: 0x10 to 0x34 is one array of nine joints, not three regions:
    /// Toy_80307470() fills nine from 0x10 with
    /// `lb_8001204C(loaded_jobj, &tg->x10, _Toy_803FE3F8, 9)`, `jobjs` is
    /// elements 2..4 (tyfigupon.c) and `x30` element 8. The two pads are the
    /// elements nothing names; on the console the spelling makes no
    /// difference, but here it decides where everything after 0x34 lands, so
    /// the nine are spelled out with the existing names kept.
    /* +10 */ HSD_JObj* x10[2];
#else
    /*  +C */ HSD_GObj* gobj2;
    /* +10 */ u8 pad_10[0x18 - 0x10];
#endif
    /* +18 */ HSD_JObj* jobjs[3];
#if defined(PORT) || defined(LINT)
    /* +24 */ HSD_JObj* x24[3];
#else
    /* +24 */ u8 pad_24[0x30 - 0x24];
#endif
    /* +30 */ HSD_JObj* x30;
    u8 pad_34[0x50 - 0x34];
    /* 0x50 */ HSD_Archive* archive;
#if defined(PORT) || defined(LINT)
    /// PORT: an archive too; Toy_80310324() assigns lbArchive_LoadSymbols()
    /// to it. `u32` in the #else, as another view of this object called it.
    /* 0x54 */ HSD_Archive* x54;
    /* 0x58 */ void* x58;
#else
    /* 0x54 */ u32 x54;
    UNK_T x58;
#endif
};
STATIC_ASSERT(offsetof(struct ToyED8Data, x0) == 0x0);
STATIC_ASSERT(offsetof(struct ToyED8Data, gobj) == 0x4);
STATIC_ASSERT(offsetof(struct ToyED8Data, gobj2) == 0xC);
#if defined(PORT) || defined(LINT)
STATIC_ASSERT(offsetof(struct ToyED8Data, x10) == 0x10);
#endif
STATIC_ASSERT(offsetof(struct ToyED8Data, jobjs) == 0x18);
#if defined(PORT) || defined(LINT)
STATIC_ASSERT(offsetof(struct ToyED8Data, x24) == 0x24);
#endif
STATIC_ASSERT(offsetof(struct ToyED8Data, x30) == 0x30);
STATIC_ASSERT(offsetof(struct ToyED8Data, archive) == 0x50);
STATIC_ASSERT(offsetof(struct ToyED8Data, x54) == 0x54);
#if defined(PORT) || defined(LINT)
STATIC_ASSERT(offsetof(struct ToyED8Data, x58) == 0x58);
#endif
ASSERT_SIZE(struct ToyED8Data, 0x5C);
struct TyArchiveData {
    HSD_GObj* gobj;
    u8 pad[0x4C];
    void* data;
};

struct TyFiguponInner {
    u8 pad[0x4D];
    u8 x4D;
};

struct TyFiguponUD {
    /* 0x00 */ u32 x0;
    /* 0x04 */ u32 x4;
    /* 0x08 */ s32 x8;
    /* 0x0C */ u8 pad_0C[0x4];
    /* 0x10 */ s32 x10;
    /* 0x14 */ s32 x14;
    /* 0x18 */ s32 x18;
    /* 0x1C */ u8 pad_1C[0xC];
    /* 0x28 */ s32 x28;
    /* 0x2C */ s32 x2C;
    /* 0x30 */ s32 x30;
    /* 0x34 */ u8 pad_34[0x10];
    /* 0x44 */ f32 x44;
    /* 0x48 */ u8 pad_48[0x10];
};

struct ToyParamEditor {
    /* 0x00 */ HSD_GObj* gobj;
    /* 0x04 */ u8 selected_slot;
    /* 0x05 */ u8 repeat_delay;
    /* 0x06 */ s16 values[9];
};

struct ToyTable {
    ToyEntry entries[9];
};

typedef struct ToyEntryData {
#if defined(PORT) || defined(LINT)
    // PORT: the second view of ToyListEntry: Toy_803087F4() is handed one of
    // the display list's entries and loads the trophy model named in it.
    // `u8 x0[0x8]` is that entry's `prev` and `next`, two pointers, so here
    // everything from `x8` on would sit eight bytes early and `x14`, the
    // archive, on the trophy id. ty/toy.c asserts the views agree, for both
    // targets.
    /* 0x00 */ void* prev;
    /* 0x04 */ void* next;
#else
    u8 x0[0x8];
#endif
    char* x8;
    char* xC;
    s16 x10;
    u8 x12[2];
    HSD_Archive* x14;
} ToyEntryData;

struct PosArray {
    s32 xy[2];
};

struct PosArrayFull {
    PosArray a[7];
};

struct lbl_803FDDE4_t {
    struct lbl_803FDDE4_t_symbols {
        char* name;
        UNK_T empty;
    } symbols[6];
    struct lbl_803FDDE4_t_values {
        int index;
        GXColor color;
        bool flag;
    } values[6];
};
ASSERT_SIZE(struct lbl_803FDDE4_t, 0x78);

#endif
