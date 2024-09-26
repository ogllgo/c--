#include "utils.h"

int main(int argc, char* argv[]) {
    int scale = 1;
    if (argc > 1) scale = atoi(argv[1]);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("An overengineered children's game", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100+160*scale, 100+160*scale, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    bool running = true;
    SDL_Event event;
    Board board;
    int x, y;
    Int2 pos;
    while (running) {
        board.print();
        std::cout << "event loop" << std::endl;
        SDL_GetMouseState(&x, &y);
        std::cout << "x: " << x << " y: " << y << "\n";
        if (x > 50 && y > 50 && x < 50 + 3*50*scale+5*2*scale && y < 50 + 3*50*scale+5*2*scale) {
            pos = Int2((x - 50 - 2 * scale) / (50 + 2 * scale), (y - 50 - 2 * scale) / (50 + 2 * scale));
        }
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << "Mouse button down event detected at x: " << pos.x << " y: " << pos.y << "\n";
                if (event.button.button == SDL_BUTTON_LEFT && board.squares[pos.y * 3 + pos.x].type == SquareName::empty) {
                    board.squares[pos.y * 3 + pos.x].type = board.currentTurn;
                    board.currentTurn = board.currentTurn == SquareName::circle ? SquareName::cross : SquareName::circle;
                } else if (board.squares[pos.y * 3 + pos.x].type != SquareName::empty) {
                    std::cout << "occupied square" << "\n";
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        board.draw(renderer, Int2(pos.y, pos.x), scale);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}