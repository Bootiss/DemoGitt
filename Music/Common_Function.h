#ifndef COMMOM_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <vector>



const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;
const int SCREEN_BPP = 24;


// Định nghĩa kích thước của nút
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 100;

int playButtonX = 570;
int playButtonY = 450;
int playButtonWidth = 100;
int playButtonHeight = 100;

int backButtonX = 700;  // Vị trí nút "Back"
int backButtonY = 450;
int backButtonWidth = 50;
int backButtonHeight = 50;




// Cấu trúc để lưu trạng thái của nút
struct Button {
    SDL_Rect rect;
    bool isClicked;
};
bool IsPointInRect(int x, int y, const SDL_Rect& rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}


SDL_Surface* g_screen = NULL;
SDL_Surface* g_bkground = NULL;
SDL_Event g_even;

#endif

