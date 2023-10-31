#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

// Định nghĩa kích thước của nút
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 50;

// Cấu trúc để lưu trạng thái của nút
struct Button {
    SDL_Rect rect;
    bool isClicked;
};

// Hàm kiểm tra xem một điểm có nằm trong hình chữ nhật không
bool IsPointInRect(int x, int y, const SDL_Rect& rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Khởi tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Phat Dung Nhac", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Khởi tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Khởi tạo SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL Mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Load và phát nhạc
    Mix_Music* music = Mix_LoadMUS("your_music_file.mp3");
    if (!music) {
        std::cerr << "Failed to load music! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Tạo nút "Phát"
    Button playButton;
    playButton.rect = { 100, 100, BUTTON_WIDTH, BUTTON_HEIGHT };
    playButton.isClicked = false;

    // Tạo nút "Dừng"
    Button stopButton;
    stopButton.rect = { 300, 100, BUTTON_WIDTH, BUTTON_HEIGHT };
    stopButton.isClicked = false;

    bool quit = false;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (IsPointInRect(x, y, playButton.rect)) {
                    // Người dùng nhấn nút "Phát"
                    Mix_PlayMusic(music, -1);  // Phát lặp vô hạn
                    playButton.isClicked = true;
                }
                else if (IsPointInRect(x, y, stopButton.rect)) {
                    // Người dùng nhấn nút "Dừng"
                    Mix_HaltMusic();  // Dừng nhạc
                    playButton.isClicked = false;
                }
            }
        }

        // Vẽ nút "Phát"
        if (playButton.isClicked) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Màu xanh khi nút được nhấn
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ khi nút không được nhấn
        }
        SDL_RenderFillRect(renderer, &playButton.rect);

        // Vẽ nút "Dừng"
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ
        SDL_RenderFillRect(renderer, &stopButton.rect);

        SDL_RenderPresent(renderer);
    }

    Mix_FreeMusic(music);
    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
