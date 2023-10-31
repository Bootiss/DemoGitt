#include<Windows.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <SDL_image.h>
#include <string>




bool Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    SDL_Window* g_screen = SDL_CreateWindow("My SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_screen == NULL)
        return false;
    return true;
}

int main(int argc, char* args[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("K", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 600, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Tải hình nền
    SDL_Surface* backgroundSurface = IMG_Load("D:\\Download\\BG.jpg"); // Đặt đường dẫn đến hình ảnh nền của bạn
    if (backgroundSurface == nullptr) {
        std::cerr << "Failed to load background image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    if (backgroundTexture == nullptr) {
        std::cerr << "Failed to create texture from background image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Vẽ hình nền lên renderer
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    // Cập nhật renderer
    SDL_RenderPresent(renderer);
    if (renderer != nullptr) {
        // Khởi tạo SDL Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL Mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
            return 1;
        }

        // Load file âm thanh
        Mix_Music* music = Mix_LoadMUS("D:\\Download\\Nhung-Ke-Mong-Mo-Piano-Cover-An-Coong.mp3");
        if (!music) {
            std::cerr << "Failed to load music! Mix_Error: " << Mix_GetError() << std::endl;
            return 1;
        }

        // Phát nhạc
        Mix_PlayMusic(music, -1);  // -1 để phát lặp vô hạn, 0 để phát một lần

        // Chờ người dùng bấm phím bất kỳ
        bool quit = false;
        while (!quit) {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }

        // Giải phóng tài nguyên và thoát
        Mix_FreeMusic(music);
        Mix_CloseAudio();

        // Giải phóng texture
        if (backgroundTexture) {
            SDL_DestroyTexture(backgroundTexture);
        }
    }
    else {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Giải phóng renderer và cửa sổ
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    // Đóng SDL
    SDL_Quit();

    return 0;
}
