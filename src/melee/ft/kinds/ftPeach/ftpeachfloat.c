#include "ftpeachfloat.h"

#include <Runtime/platform.h>

#include <melee/ft/forward.h>

#include "ftpeachfloatattack.h"
#include "ftpeachfloatfall.h"
#include "types.h"
#include <dolphin/mtx.h>
#include <melee/ef/efasync.h>
#include <melee/ft/fighter.h>
#include <melee/ft/ft_081B.h>
#include <melee/ft/ftcommon.h>
#include <melee/ft/ftparts.h>
#include <melee/ft/kinds/ftCommon/ftCo_FallSpecial.h>
#include <melee/ft/kinds/ftCommon/ftCo_SpecialAir.h>
#include <melee/ft/types.h>
#ifdef PORT
#include <port/hooks.h> // tap jump off; port/game/tap_jump.c
#endif

bool ftPe_Float_CheckContinueInput(Fighter* fp)
{
#ifdef PORT
    // PORT: up held is a held jump, which starts the float at the top of a
    // jump and keeps it going; with tap jump off (port/game/tap_jump.c) only
    // X or Y held does.
    return (fp->input.lstick[0].y >= p_ftCommonData->tap_jump_threshold &&
            !port_hook_fighter_tap_jump_suppressed(fp)) ||
           fp->input.held_buttons[0] & HSD_PAD_XY;
#else
    return fp->input.lstick[0].y >= p_ftCommonData->tap_jump_threshold ||
           fp->input.held_buttons[0] & HSD_PAD_XY;
#endif
}

static bool checkStartFloatInput(HSD_GObj* gobj)
{
    Fighter* temp_r6 = GET_FIGHTER(gobj);
    return temp_r6->input.lstick[0].y <= -p_ftCommonData->x88 &&
           temp_r6->input.held_buttons[0] & HSD_PAD_XY;
}

bool ftPe_8011BA54(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    bool float_input = checkStartFloatInput(gobj);
    if (fp->kind == Ft_Kind_Peach && fp->u.pe.has_float && float_input) {
        ftPe_8011BB6C(gobj, true);
        return true;
    }
    return false;
}

bool ftPe_8011BAD8(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    if (fp->kind == Ft_Kind_Peach) {
        if (fp->self_vel.y <= 0 && fp->u.pe.has_float) {
            if (ftPe_Float_CheckContinueInput(fp)) {
                ftPe_8011BB6C(gobj, true);
                return true;
            }
        }
    }
    return false;
}

static void spawnParticle(HSD_GObj* gobj, HSD_JObj* joint)
{
    Fighter* fp = GET_FIGHTER(gobj);
    efAsync_Spawn(gobj, &fp->x60C, 0, 1236, joint);
}

void ftPe_8011BB6C(HSD_GObj* gobj, bool arg1)
{
    u8 _[8];
    Fighter* fp = GET_FIGHTER(gobj);
    ftPe_DatAttrs* da = fp->dat_attrs;
    HSD_JObj* joint;

    Fighter_ChangeMotionState(gobj, ftPe_MS_Float, Ft_MF_None, 0, 1, 0, NULL);
    fp->u.pe.has_float = false;
    if (arg1) {
        fp->u.pe.x4 = da->xC;
    }
    fp->self_vel.y = 0;
    fp->x2219_b0 = true;
    joint = fp->parts[ftParts_GetBoneIndex(fp, FtPart_TransN)].joint;
    spawnParticle(gobj, joint);
}

void ftPe_Float_Anim(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    if (fp->u.pe.x4 > 0) {
        fp->u.pe.x4 -= 1;
    }
    if (fp->u.pe.x4 <= 0) {
        ftPe_UpdateFloatDir(gobj);
    }
}

void ftPe_Float_IASA(HSD_GObj* gobj)
{
    Fighter* fp = gobj->user_data;
    if (!ftCo_SpecialAir_CheckInput(gobj) && !ftPe_8011BE80(gobj)) {
        /// @todo Call #checkContinueFloatInput
#ifdef PORT
        // PORT: the same test as ftPe_Float_CheckContinueInput().
        bool float_input =
            (fp->input.lstick[0].y >= p_ftCommonData->tap_jump_threshold &&
             !port_hook_fighter_tap_jump_suppressed(fp)) ||
            fp->input.held_buttons[0] & HSD_PAD_XY;
#else
        bool float_input =
            fp->input.lstick[0].y >= p_ftCommonData->tap_jump_threshold ||
            fp->input.held_buttons[0] & HSD_PAD_XY;
#endif
        if (!float_input) {
            ftPe_UpdateFloatDir(gobj);
        }
    }
}

void ftPe_Float_Phys(HSD_GObj* gobj)
{
    ftCommon_CalcSelfAccel_Drift(GET_FIGHTER(gobj));
}

void ftPe_Float_Coll(HSD_GObj* gobj)
{
    ft_800831CC(gobj, ftCo_80096CC8, ft_80082B1C);
}
