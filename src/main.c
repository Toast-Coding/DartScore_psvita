#include <stdio.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

#include "debugScreen.h"
#define MENU_ITEMS 3
typedef struct {
    const char* name;
    int value;
    int min;
    int max;
} MenuItem;

MenuItem menu[MENU_ITEMS] = {
    {"Volume", 5, 0, 10},
    {"Brightness", 3, 0, 5},
    {"Difficulty", 1, 0, 2}
};

int ScoreCounter(int shot_score, int total_score) {
    return shot_score + total_score;
}
int shot() {

}
void main_menu() {
    psvDebugScreenInit();
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITAL);
    int selected = 0;
    int editing = 0;

    SceCtrlData pad, oldpad;
    oldpad.buttons = 0;

    while (1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        int pressed = pad.buttons & ~oldpad.buttons;

        // ------------------------
        // INPUT
        // ------------------------

        if (!editing) {
            if (pressed & SCE_CTRL_DOWN) {
                selected = (selected + 1) % MENU_ITEMS;
            }
            if (pressed & SCE_CTRL_UP) {
                selected--;
                if (selected < 0) selected = MENU_ITEMS - 1;
            }
            if (pressed & SCE_CTRL_CROSS) {
                editing = 1;
            }
        } else {
            if (pressed & SCE_CTRL_RIGHT) {
                if (menu[selected].value < menu[selected].max)
                    menu[selected].value++;
            }
            if (pressed & SCE_CTRL_LEFT) {
                if (menu[selected].value > menu[selected].min)
                    menu[selected].value--;
            }
            if (pressed & SCE_CTRL_CROSS) {
                editing = 0;
            }
        }

        oldpad = pad;

        // ------------------------
        // RENDER
        // ------------------------

        psvDebugScreenClear(0x00000000); // clear screen with black

        psvDebugScreenPrintf("=== Simple XMB-style Menu ===\n\n");

        for (int i = 0; i < MENU_ITEMS; i++) {
            if (i == selected) {
                if (editing)
                    psvDebugScreenSetFgColor(0xff00ffff); // yellow when editing
                else
                    psvDebugScreenSetFgColor(0xff00ff00); // green when selected
            } else {
                psvDebugScreenSetFgColor(0xffffffff); // white
            }

            psvDebugScreenPrintf("%s : %d\n", menu[i].name, menu[i].value);
        }

        psvDebugScreenSetFgColor(0xffffffff);

        if (editing)
            psvDebugScreenPrintf("\nEditing mode: LEFT/RIGHT to change, X to confirm");
        else
            psvDebugScreenPrintf("\nUse UP/DOWN to navigate, X to edit");

        sceKernelDelayThread(16000); // ~60 FPS
    }
}
int main(void) {
    psvDebugScreenInit();
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITAL);
    main_menu();
    return 0;
}
