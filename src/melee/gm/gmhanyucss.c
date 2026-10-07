#include "gm_1601.h"
#include "gm_unsplit.h"
#include "gmvsmelee.h"
#include "types.h"
#include <melee/mn/types.h>

/* 1BED3C */ static void gm_801BED3C(GameModeState*);
/* 1BEDA8 */ static void gm_801BEDA8(GameModeState*);
/* 49BEE8 */ static CSSData gm_8049BEE8;

GameModeState gm_Mode_HanyuCss_States[] = {
    {
        0,
        lbDvdPreload_2,
        0,
        gm_801BED3C,
        gm_801BEDA8,
        {
            GS_CSS,
            &gm_8049BEE8,
            NULL,
        },
    },
    { -1 },
};

void gm_801BED3C(GameModeState* arg0)
{
    CSSData* temp_r31 = gm_GetGameModeStateEnterData(arg0);
    temp_r31->vs = *gmVsMelee_GetVsData();
#ifdef PORT
    // PORT: every other route into GS_CSS sets all three fields of a CSSData
    // (gmVsMelee_EnterCss() sets match_type, ko_counts and vs); this one sets
    // only vs and leaves ko_counts at the zero of the static gm_8049BEE8.
    // mnCharSel_8025D1C4() reads `css->ko_counts[port]` for every match_type
    // that shows KO stars, match_type 0 among them, and dereferences null.
    // This points it at the array the VS entry uses. The mode is a
    // development one that retail menus cannot reach.
    temp_r31->ko_counts = gmVsMelee_GetKOCounts();
#endif

    gm_80164F18();
    if (temp_r31->match_type & 1) {
        gm_80164A0C(7);
    }
}

void gm_801BEDA8(GameModeState* arg0)
{
    CSSData* css = gm_GetGameModeStateEnterData(arg0);
    VsModeData* vs = gmVsMelee_GetVsData();

    if (css->pending_scene_change == 2) {
        if (css->match_type != 0) {
            css->match_type--;
        } else {
            css->match_type = 0x17;
        }
    } else {
        css->match_type = (css->match_type + 1) % 24;
    }

    *vs = css->vs;
}
