#include "Common_Function.h"

// Hàm kiểm tra xem một điểm có nằm trong hình chữ nhật không
bool PointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH) {
    return x >= rectX && x <= rectX + rectW && y >= rectY && y <= rectY + rectH;
}

int main(int argc, char* args[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Music Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Danh sách các bài hát
    std::vector<std::string> songs = {
        "C:\\visual studio\\Music\\Music\\List\\Nhung-Ke-Mong-Mo-Piano-Cover-An-Coong.mp3",
        "C:\\visual studio\\Music\\Music\\List\\vevoianhdi.mp3",
        "C:\\visual studio\\Music\\Music\\List\\ConMuaTinhYeu.mp3",
        "C:\\visual studio\\Music\\Music\\List\\Id072019.mp3",
        "C:\\visual studio\\Music\\Music\\List\\3107.mp3",
        "C:\\visual studio\\Music\\Music\\List\\31073.mp3",
        // Thêm các bài hát khác vào đây
    };

    // Load hình ảnh nút "play"
    SDL_Surface* playButtonSurface = IMG_Load("D:\\Download\\play2.png");
    if (playButtonSurface == nullptr) {
        std::cerr << "Failed to load play button image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* playButtonTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
    SDL_FreeSurface(playButtonSurface);

    // Load hình ảnh nút "back"
    SDL_Surface* backButtonSurface = IMG_Load("C:\\visual studio\\Music\\Music\\Picture\\next2.png");
    if (backButtonSurface == nullptr) {
        std::cerr << "Failed to load back button image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* backButtonTexture = SDL_CreateTextureFromSurface(renderer, backButtonSurface);
    SDL_FreeSurface(backButtonSurface);

    //Load hinh anh nut "next"
    SDL_Surface* nextButtonSurface = IMG_Load("C:\\visual studio\\Music\\Music\\Picture\\next2.png");
    if (nextButtonSurface == nullptr) {
        std::cerr << "Failed to load play button image! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* nextButtonTexture = SDL_CreateTextureFromSurface(renderer, nextButtonSurface);
    SDL_FreeSurface(nextButtonSurface);


    // Tải hình nền
    SDL_Surface* backgroundSurface = IMG_Load("C:\\visual studio\\Music\\Music\\Picture\\BG.jpg");
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

    // Các thông số vị trí và kích thước của nút "play"
    int playButtonX = 470;
    int playButtonY = 450;
    int playButtonWidth = 100;
    int playButtonHeight = 100;

    int nextButtonX = 600;  // Vị trí nút "Next"
    int nextButtonY = 450;
    int nextButtonWidth = 100;
    int nextButtonHeight = 100;

    int backButtonX = 340;  // Vị trí nút "Back"
    int backButtonY = 450;
    int backButtonWidth = 100;
    int backButtonHeight = 100;

    SDL_Rect playButtonRect;
    playButtonRect.x = playButtonX;
    playButtonRect.y = playButtonY;
    playButtonRect.w = playButtonWidth;
    playButtonRect.h = playButtonHeight;

    SDL_Rect nextButtonRect;
    nextButtonRect.x = nextButtonX;
    nextButtonRect.y = nextButtonY;
    nextButtonRect.w = nextButtonWidth;
    nextButtonRect.h = nextButtonHeight;

    SDL_Rect backButtonRect;
    backButtonRect.x = backButtonX;
    backButtonRect.y = backButtonY;
    backButtonRect.w = backButtonWidth;
    backButtonRect.h = backButtonHeight;

    // Cập nhật renderer
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButtonRect);
    SDL_RenderCopy(renderer, nextButtonTexture, nullptr, &nextButtonRect);
    SDL_RenderCopy(renderer, backButtonTexture, nullptr, &backButtonRect);
    SDL_RenderPresent(renderer);

    // Biến để theo dõi trạng thái của bài hát
    bool isPlaying = false;

    // Biến chỉ mục của bài hát hiện tại
    int currentSongIndex = 0;

    // Biến chỉ mục của bài hát cuối cùng được phát
    int lastPlayedSongIndex = -1;

    // Biến để xác định trạng thái của nút "play"
    bool playButtonClicked = false;

    // Khởi tạo SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL Mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Music* music = nullptr;

    // Xử lý sự kiện
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (PointInRect(mouseX, mouseY, playButtonX, playButtonY, playButtonWidth, playButtonHeight)) {
                    if (!playButtonClicked) {
                        // Nếu nút "play" chưa được click trước đó
                        if (currentSongIndex < songs.size()) {
                            if (music) {
                                Mix_FreeMusic(music);
                            }
                            music = Mix_LoadMUS(songs[currentSongIndex].c_str());
                            if (Mix_PlayMusic(music, 0) == 0) {
                                lastPlayedSongIndex = currentSongIndex;
                                currentSongIndex++;
                                if (currentSongIndex >= songs.size()) {
                                    currentSongIndex = 0;
                                }
                            }
                        }
                    }
                    else {
                        // Nếu nút "play" đã được click trước đó
                        if (isPlaying) {
                            Mix_PauseMusic();
                        }
                        else {
                            Mix_ResumeMusic();
                        }
                    }
                    isPlaying = !isPlaying;  // Đảo ngược trạng thái phát/tạm dừng
                    playButtonClicked = true;
                }
                else if (PointInRect(mouseX, mouseY, nextButtonX, nextButtonY, nextButtonWidth, nextButtonHeight)) {
                    // Xử lý sự kiện khi nút "Next" được click
                    if (currentSongIndex < songs.size()) {
                        if (music) {
                            Mix_FreeMusic(music);
                        }
                        music = Mix_LoadMUS(songs[currentSongIndex].c_str());
                        if (Mix_PlayMusic(music, 0) == 0) {
                            lastPlayedSongIndex = currentSongIndex;
                            currentSongIndex++;
                            if (currentSongIndex >= songs.size()) {
                                currentSongIndex = 0;
                            }
                        }
                    }
                }
                else if (PointInRect(mouseX, mouseY, backButtonX, backButtonY, backButtonWidth, backButtonHeight)) {
                    // Xử lý sự kiện khi nút "Back" được click
                    if (currentSongIndex > 0) {  // Kiểm tra currentSongIndex > 0
                        if (music) {
                            Mix_FreeMusic(music);
                        }
                        currentSongIndex--;  // Giảm chỉ mục của bài hát
                        if (currentSongIndex < 0) {
                            currentSongIndex = songs.size() - 1;  // Quay lại bài cuối cùng nếu cần
                        }
                        music = Mix_LoadMUS(songs[currentSongIndex].c_str());
                        if (Mix_PlayMusic(music, 0) == 0) {
                            lastPlayedSongIndex = currentSongIndex;
                        }
                    }
                }

            }
            else if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        // Kiểm tra xem bài hát đã kết thúc
        if (music != nullptr && Mix_PlayingMusic() == 0) {
            if (currentSongIndex < songs.size()) {
                if (music) {
                    Mix_FreeMusic(music);
                }
                music = Mix_LoadMUS(songs[currentSongIndex].c_str());
                if (Mix_PlayMusic(music, 0) == 0) {
                    lastPlayedSongIndex = currentSongIndex;
                    currentSongIndex++;
                    if (currentSongIndex >= songs.size()) {
                        currentSongIndex = 0;
                    }
                }
            }
        }
    }

    // Giải phóng tài nguyên

    if (playButtonTexture) {
        SDL_DestroyTexture(playButtonTexture);
    }

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    if (nextButtonTexture) {
        SDL_DestroyTexture(nextButtonTexture);
    }

    if (backButtonTexture) {
        SDL_DestroyTexture(backButtonTexture);
    }

    // Giải phóng renderer và cửa sổ
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    // Đóng SDL Mixer
    Mix_CloseAudio();

    // Đóng SDL
    SDL_Quit();

    return 0;
}
