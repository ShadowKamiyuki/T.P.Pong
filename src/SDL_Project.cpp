#include <iostream>
#include <string>
#include <random>

// SDL libraries
#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_image.h"

using namespace std;

SDL_Event event;
TTF_Font* arial;
SDL_Window* window;
SDL_Renderer* renderer;

float movementSpeed = 10.0;
int velX = 3;
int velY = 3;

int player1Score = 0;
int player2Score = 0;
SDL_Rect textDisplay;
int timer;

// window size
#define WIDTH 1280
#define HEIGHT 720

void drawText(string text, int x, int y)
{
    const char* t = text.c_str();
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(arial, t, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    textDisplay.x = x - surface->w / 2;
    textDisplay.y = y - surface->h / 2;
    textDisplay.w = surface->w;
    textDisplay.h = surface->h;

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &textDisplay);
    SDL_DestroyTexture(texture);
}

void render(SDL_Texture* paddleJ1Texture, SDL_Texture* paddleJ2Texture, SDL_Rect paddleJ1Rect, SDL_Rect paddleJ2Rect, SDL_Texture* ballTexture, SDL_Rect ballRect)
{
    // Limpiar el renderer
    SDL_RenderClear(renderer);

    // Renderizar la nave en la posición especificada
    SDL_RenderCopy(renderer, paddleJ1Texture, NULL, &paddleJ1Rect);
    SDL_RenderCopy(renderer, paddleJ2Texture, NULL, &paddleJ2Rect);
    SDL_RenderCopy(renderer, ballTexture, NULL, &ballRect);

    // Actualiza los textos
    drawText(to_string(player1Score) + " | " + to_string(player2Score), WIDTH / 2, 50);

    // Actualizar pantalla
    SDL_RenderPresent(renderer);

    // Delay para controlar la velocidad de actualización
    SDL_Delay(16); // Aproximadamente 60 FPS
}

void mainMenu()
{
    string menuText = "PONG";
    string button1 = "Press space to start";
    drawText(menuText, WIDTH / 2, HEIGHT / 4);
    drawText(button1, WIDTH / 2, HEIGHT / 2);
    SDL_RenderPresent(renderer);
}

void resetMenu()
{
    SDL_RenderClear(renderer);
    string menuText = "Play again?";
    string button1 = "Press space to reset";
    drawText(menuText, WIDTH / 2, HEIGHT / 4);
    drawText(button1, WIDTH / 2, HEIGHT / 2);
    SDL_RenderPresent(renderer);
}

void showWinnerMessage(int player)
{
    string winnerText = "Jugador " + to_string(player) + " gana!";
    drawText(winnerText, WIDTH / 2, HEIGHT / 2);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000); // Mostrar el mensaje durante 3 segundos
}

int main(int argc, char* argv[])
{
    // SDL initialization check
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error intializing SDL. " << SDL_GetError() << endl;
    }

    // SDL window and render creation
    window = SDL_CreateWindow("PONG", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // window and renderer creation check
    if (window == NULL || renderer == NULL)
    {
        cout << "Error when creating window or renderer. " << SDL_GetError() << endl;
    }

    // TTF initialization
    if (TTF_Init() != 0)
    {
        cout << "Error when initializing TTF. " << TTF_GetError() << endl;
    }

    arial = TTF_OpenFont("assets/fonts/arial.ttf", 100);
    if (!arial)
    {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }

    // set background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 40, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    bool start = true;
    bool running = false;
    bool ending = false;

    // menu principal
    while (start)
    {
        mainMenu();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                start = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    start = false;
                    running = true;
                    break;
                case SDLK_ESCAPE:
                    start = false;
                    running = false;
                    break;
                }
            }
        }
    }

    // load paddle textures
    string paddleJ1Path = "assets/img/PaletaJ1.png";
    SDL_Texture* paddleJ1Texture = IMG_LoadTexture(renderer, paddleJ1Path.c_str());

    string paddleJ2Path = "assets/img/PaletaJ2.png";
    SDL_Texture* paddleJ2Texture = IMG_LoadTexture(renderer, paddleJ2Path.c_str());

    // define paddle J1 size and position (struct)
    SDL_Rect paddleJ1Rect;
    paddleJ1Rect.x = 40;
    paddleJ1Rect.y = 100;
    paddleJ1Rect.w = 25;
    paddleJ1Rect.h = 100;

    // define paddle J2/CPU size and position (struct)
    SDL_Rect paddleJ2Rect;
    paddleJ2Rect.x = 1210;
    paddleJ2Rect.y = 100;
    paddleJ2Rect.w = 25;
    paddleJ2Rect.h = 100;

    // create ball
    string ballPath = "assets/img/pongPelota.png";
    SDL_Texture* ballTexture = IMG_LoadTexture(renderer, ballPath.c_str());

    SDL_Rect ballRect;
    ballRect.x = 200;
    ballRect.y = HEIGHT / 2;
    ballRect.w = 20;
    ballRect.h = 20;

    // game loop
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    paddleJ1Rect.y -= movementSpeed; // Mover hacia arriba
                    break;
                case SDLK_s:
                    paddleJ1Rect.y += movementSpeed; // Mover hacia abajo
                    break;
                case SDLK_UP:
                    paddleJ2Rect.y -= movementSpeed;
                    break;
                case SDLK_DOWN:
                    paddleJ2Rect.y += movementSpeed;
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
            }
        }

        // actualizar posicion de la pelota
        ballRect.x += velX;
        ballRect.y += velY;

        //comprobar colisiones de la pelota con los bordes y sumar puntos
        if (ballRect.x < 0)
        {
            SDL_Delay(100);
            ballRect.y = HEIGHT / 2;
            ballRect.x = 200;
            velX = -velX;
            player2Score++;
        }

        if (ballRect.x + ballRect.w > WIDTH)
        {
            SDL_Delay(100);
            ballRect.y = HEIGHT / 2;
            ballRect.x = 1180;
            velX = -velX;
            player1Score++;
        }

        if (ballRect.y < 0 || ballRect.y + ballRect.h > HEIGHT)
        {
            velY = -velY;
        }

        // collisions Y axis
        if (paddleJ1Rect.y > HEIGHT - paddleJ1Rect.h)
        {
            paddleJ1Rect.y = HEIGHT - paddleJ1Rect.h;
        }
        else if (paddleJ1Rect.y < 0)
        {
            paddleJ1Rect.y = 0;
        }

        if (paddleJ2Rect.y > HEIGHT - paddleJ2Rect.h)
        {
            paddleJ2Rect.y = HEIGHT - paddleJ2Rect.h;
        }
        else if (paddleJ2Rect.y < 0)
        {
            paddleJ2Rect.y = 0;
        }

        // colision con las paletas
        SDL_Rect result;

        if (SDL_IntersectRect(&paddleJ1Rect, &ballRect, &result))
        {
            velX = -velX;
            velY = -3 + rand() % 2;
        }

        if (SDL_IntersectRect(&paddleJ2Rect, &ballRect, &result))
        {
            velX = -velX;
            velY = -3 + rand() % 2;
        }

        // funcion render
        render(paddleJ1Texture, paddleJ2Texture, paddleJ1Rect, paddleJ2Rect, ballTexture, ballRect);

        // tiempo de partida
        timer++;

        if (timer >= 2000)
        {
            if (player1Score == player2Score)
            {
                string winnerText = "Empate";
                drawText(winnerText, WIDTH / 2, HEIGHT / 2);
                SDL_RenderPresent(renderer);
                SDL_Delay(3000); // Mostrar el mensaje durante 3 segundos
            }
            else
            {
                // Verificar si alguno de los jugadores ha ganado
                if (player1Score > player2Score)
                {
                    showWinnerMessage(1);
                    running = false;
                }
                else if (player1Score < player2Score)
                {
                    showWinnerMessage(2);
                    running = false;
                }
            }

            ending = true;
        }

        // Verificar si alguno de los jugadores ha llegado a 3 puntos
        if (player1Score == 3)
        {
            showWinnerMessage(1);
            running = false;
            ending = true;
        }
        else if (player2Score == 3)
        {
            showWinnerMessage(2);
            running = false;
            ending = true;
        }

        //reinicio del juego
        while (ending)
        {
            resetMenu();
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    ending = false;
                }

                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        ending = false;
                        running = true;
                        player1Score = 0;
                        player2Score = 0;
                        timer = 0;
                        break;
                    case SDLK_ESCAPE:
                        ending = false;
                        running = false;
                        break;
                    }
                }
            }
        }
    }

    // clear resources
    TTF_CloseFont(arial);
    SDL_DestroyTexture(paddleJ1Texture);
    SDL_DestroyTexture(paddleJ2Texture);
    SDL_DestroyTexture(ballTexture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}