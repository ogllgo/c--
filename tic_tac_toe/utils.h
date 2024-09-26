#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <stdlib.h>

enum class SquareName {
    empty,
    cross,
    circle
};

struct Int2 {
    int x, y;
    Int2(int inX = 0, int inY = 0) :
        x(inX),
        y(inY)
    {}
};

struct ColourRGB {
    int r, g, b;
    // r: red
    // g: green
    // b: blue
    ColourRGB(int inR = 0, int inG = 0, int inB = 0) :
        r(inR),
        g(inG),
        b(inB)
    {}
};

struct Square {
    Int2 index;
    SquareName type;
    Square(Int2 inIndex, SquareName inType) : 
        index(inIndex),
        type(inType)
    {}
};

void printGrid(SDL_Renderer* renderer, Int2 position, Int2 size, std::pair<ColourRGB, ColourRGB> colours) {
    for (int i = position.x; i < position.x + size.x; i++) {
        for (int j = position.y; j < position.y + size.y; j++) {
            ColourRGB colour;
            if ((i + j) % 2 == 0) {
                colours.first;
            } else {
                colours.second;
            }
            SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
};

struct Board {
    std::vector<Square> squares;
    SquareName currentTurn;
    Board() {
        this->squares = {
            Square(Int2(0, 0), SquareName::empty), Square(Int2(0, 1), SquareName::empty), Square(Int2(0, 2), SquareName::empty),
            Square(Int2(1, 0), SquareName::empty), Square(Int2(1, 1), SquareName::empty), Square(Int2(1, 2), SquareName::empty),
            Square(Int2(2, 0), SquareName::empty), Square(Int2(2, 1), SquareName::empty), Square(Int2(2, 2), SquareName::empty)
        };
        this->currentTurn = SquareName::cross;
    }

    void print(Int2 selected = Int2()) {
        system("clear");
        std::cout << "┏━━━━━┳━━━━━┳━━━━━┓\n";
        for (int row = 0; row < 3; row++) {

            for (int col = 0; col < 3; col++) {
                if (col < 2) std::cout << "┃";
                Square current = this->squares[row * 3 + col];
                switch (current.type) {
                    case (SquareName::cross):
                        std::cout << " ╲ ╱ ";
                        break;
                    case (SquareName::circle):
                        std::cout << " ┌─┐ ";
                        break;
                    case (SquareName::empty):
                    default:
                        std::cout << " ░░░ ";
                        break;
                }
                if (col > 0) std::cout << "┃";
            }
            std::cout << "\n";
            for (int col = 0; col < 3; col++) {
                if (col < 2) std::cout << "┃";
                Square current = this->squares[row * 3 + col];
                switch (current.type) {
                    case (SquareName::cross):
                        std::cout << "  ╳  ";
                        break;
                    case (SquareName::circle):
                        std::cout << " │ │ ";
                        break;
                    case (SquareName::empty):
                    default:
                        std::cout << " ░░░ ";
                        break;
                }
                if (col > 0) std::cout << "┃";
            }
            std::cout << "\n";
            for (int col = 0; col < 3; col++) {
                if (col < 2) std::cout << "┃";
                Square current = this->squares[row * 3 + col];
                switch (current.type) {
                    case (SquareName::cross):
                        std::cout << " ╱ ╲ ";
                        break;
                    case (SquareName::circle):
                        std::cout << " └─┘ ";
                        break;
                    case (SquareName::empty):
                    default:
                        std::cout << " ░░░ ";
                        break;
                }
                if (col > 0) std::cout << "┃";
            }
            std::cout << "\n";


            if (row < 2) std::cout << "┣━━━━━╋━━━━━╋━━━━━┫\n";
        }

        std::cout << "┗━━━━━┻━━━━━┻━━━━━┛\n";
    }

    void draw(SDL_Renderer* renderer, Int2 selected, float scaling = 1.0f) {

        // draw the outer box
        SDL_Rect gridRect;
        gridRect.x = 50;
        gridRect.y = 50;
        gridRect.w = 160 * scaling;
        gridRect.h = 160 * scaling;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &gridRect);
        gridRect.x = 50 + 2 * scaling;
        gridRect.y = 50 + 2 * scaling;
        gridRect.w = 156 * scaling;
        gridRect.h = 156 * scaling;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &gridRect);

        // draw the vertical lines
        for (int i = 0; i < 2; i++) {
            SDL_Rect lineRect;
            lineRect.x = 50 + 50 * scaling + 2 + i * 52 * scaling;
            lineRect.y = 52;
            lineRect.w = 2 * scaling;
            lineRect.h = 157 * scaling;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &lineRect);
        }

        // draw the horizontal lines
        for (int i = 0; i < 2; i++) {
            SDL_Rect lineRect;
            lineRect.x = 52;
            lineRect.y = 50 + 50 * scaling + 2 + i * 52 * scaling;
            lineRect.h = 2 * scaling;
            lineRect.w = 157 * scaling;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &lineRect);
        }

        for (Square& square : this->squares) {
            int x = 53 + 10 * scaling + square.index.x * 50 * scaling + square.index.x * 2 * scaling;
            int y = 53 + 10 * scaling + square.index.y * 50 * scaling + square.index.y * 2 * scaling;
            int w = 30 * scaling;
            int h = 30 * scaling;
            if (square.index.x == selected.x && square.index.y == selected.y) {
                SDL_SetRenderDrawColor(renderer, 25, 50, 50, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_Rect rect;
            rect.x = y + 1;
            rect.y = x + 1;
            rect.w = w - 2;
            rect.h = h - 2;
            SDL_RenderFillRect(renderer, &rect);
            if (square.index.x == selected.x && square.index.y == selected.y) {
                SDL_SetRenderDrawColor(renderer, 150, 200, 200, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            if (square.type == SquareName::circle) {
                SDL_Rect rect;
                rect.x = y;
                rect.y = x;
                rect.w = w;
                rect.h = h;
                SDL_RenderDrawRect(renderer, &rect);
            } else if (square.type == SquareName::cross) {
                SDL_RenderDrawLine(renderer, y, x, y + w, x + h);
                SDL_RenderDrawLine(renderer, y + h, x, y, x + w);
            }
        }
    }

    void move(Int2 pos) {
        if (this->squares[pos.y * 3 + pos.x].type != SquareName::empty) return;

        this->squares[pos.y * 3 + pos.x].type = this->currentTurn;
        this->currentTurn = this->currentTurn == SquareName::circle ? SquareName::cross : SquareName::circle;
    }
};
#endif