#include <stdio.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
//#include <psp2/>

#include "debugScreen.h"

typedef struct {
    const char* name;
    const char** options;
    int options_count;
    int selected;
} MenuItem;

const char* points[] = {"101", "201", "301", "401", "501"};
const char* sets[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
const char* legs[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
const char* check_in[] = {"Straight in", "Double in", "Master in"};
const char* check_out[] = {"Straight out", "Double out", "Master out"};

MenuItem menu[MENU_ITEMS] = {
    {"Points", points, 5, 0},
    {"Sets", sets, 10, 0},
    {"Legs", legs, 10, 0},
    {"Check in", check_in, 3, 0}
    {"Check out", check_out, 3, 0}
};

#define MENU_ITEMS (sizeof(menu)/sizeof(MenuItem))

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

    SceCtrlData pad, oldpad = {0};

    while (1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        int pressed = pad.buttons & ~oldpad.buttons;
        MenuItem* item = &menu[selected];

        //Input for menu
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
                item->selected = (item->selected +1) % item->options_count;
            }
            if (pressed & SCE_CTRL_LEFT) {
                item->selected--;
                if (item->selected < 0) item->selected = item->options_count - 1;
            }
            if (pressed & SCE_CTRL_CROSS) {
                editing = 0;
            }
        }

        oldpad = pad;

        //Rednering menu

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

            psvDebugScreenPrintf("%s : %s\n", menu[i].name, menu[i].options[menu[i].selected]);
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
