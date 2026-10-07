#include "lbcommand.h"

#include "inlines.h"
#include "lb_0219.h"
#include "types.h"
#ifdef PORT
#include <port/port.h> // port_mem1_from_u32()

// PORT: a command word holding an address, widened back. The transcoder
// writes the target's address truncated to 32 bits; see struct Command_05
// in lb/types.h.
static union CmdUnion* port_cmd_target(u32 addr)
{
    return (union CmdUnion*) port_mem1_from_u32(addr, sizeof(union CmdUnion));
}
#endif

void (*lbCommand_803B9840[16])(CommandInfo*) = {
    Command_00, Command_01, Command_02, Command_03, Command_04, Command_05,
    Command_06, Command_07, Command_08, Command_09, NULL,       NULL,
    NULL,       NULL,       NULL,       NULL
};

/// Reset
void Command_00(CommandInfo* info)
{
    info->x8.u = NULL;
}

/// SynchronousTimer
void Command_01(CommandInfo* info)
{
    info->timer += info->x8.u->Command_00.value;
    NEXT_CMD(info);
}

/// AsynchronousTimer
void Command_02(CommandInfo* info)
{
    info->timer = info->x8.u->Command_02.value - info->frame_count;
    NEXT_CMD(info);
}

/// SetLoop
void Command_03(CommandInfo* info)
{
    info->event_return[info->loop_count++] = info->x8.u + 1;
    info->event_return[info->loop_count++] =
        (union CmdUnion*) info->x8.u->Command_03.value;
    NEXT_CMD(info);
}

/// Execute Loop
void Command_04(CommandInfo* info)
{
#ifdef PORT
    // PORT: the console indexes the struct as an array of 32-bit words.
    // ptr[loop_count + 3] is event_return[loop_count - 1], the counter
    // Command_03() pushed, and x8.ptr[loop_count] is
    // event_return[loop_count - 2], the address it pushed before that. Here
    // those members are eight bytes and the same expressions land on halves
    // of pointers, so the members are named. The counter stays in a pointer
    // slot: the stack holds both kinds in one array.
    union CmdUnion** counter = &info->event_return[info->loop_count - 1];

    *counter = (union CmdUnion*) ((uintptr_t) *counter - 1);
    if ((s32) (uintptr_t) *counter) {
        info->x8.u = info->event_return[info->loop_count - 2];
        return;
    }
#else
    u32* ptr = (u32*) info;
    ptr[info->loop_count + 3] -= 1;

    if ((s32) info->event_return[info->loop_count - 1]) {
        info->x8.ptr[0] = &info->x8.ptr[info->loop_count][0];
        return;
    }
#endif
    NEXT_CMD(info);
    info->loop_count -= 2;
}

/// Subroutine
void Command_05(CommandInfo* info)
{
    NEXT_CMD(info);
    info->event_return[info->loop_count++] = info->x8.u + 1;
#ifdef PORT
    // PORT: the target is a command word, so it stays four bytes and is
    // widened here; see struct Command_05 in lb/types.h.
    info->x8.u = port_cmd_target(info->x8.u->Command_05.ptr);
#else
    info->x8.u = info->x8.u->Command_05.ptr;
#endif
}

/// Return
void Command_06(CommandInfo* info)
{
    info->x8.u = info->event_return[info->loop_count -= 1];
}

/// Goto
void Command_07(CommandInfo* info)
{
    NEXT_CMD(info);
#ifdef PORT
    info->x8.u = port_cmd_target(info->x8.u->Command_07.ptr);
#else
    info->x8.u = info->x8.u->Command_07.ptr;
#endif
}

/// SetTimerAnimation
void Command_08(CommandInfo* info)
{
    NEXT_CMD(info);
    info->timer = F32_MAX;
}

void Command_09(CommandInfo* info)
{
    lbBgFlash_80021C48(info->x8.u->Command_09.param_1,
                       info->x8.u->Command_09.param_2);
    NEXT_CMD(info);
}

bool Command_Execute(CommandInfo* info, u32 command)
{
    if (command < 10) {
        lbCommand_803B9840[command](info);
        return true;
    }
    return false;
}
