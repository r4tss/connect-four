#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void displayNumber(SDL_Renderer *renderer, char ch, SDL_Rect dest, SDL_Texture *Nums[10])
  {
    switch((int)(ch)+0)
      {
        case 48:
          SDL_RenderCopy(renderer, Nums[0], NULL, &dest);
          break;
        case 49:
          SDL_RenderCopy(renderer, Nums[1], NULL, &dest);
          break;
        case 50:
          SDL_RenderCopy(renderer, Nums[2], NULL, &dest);
          break;
        case 51:
          SDL_RenderCopy(renderer, Nums[3], NULL, &dest);
          break;
        case 52:
          SDL_RenderCopy(renderer, Nums[4], NULL, &dest);
          break;
        case 53:
          SDL_RenderCopy(renderer, Nums[5], NULL, &dest);
          break;
        case 54:
          SDL_RenderCopy(renderer, Nums[6], NULL, &dest);
          break;
        case 55:
          SDL_RenderCopy(renderer, Nums[7], NULL, &dest);
          break;
        case 56:
          SDL_RenderCopy(renderer, Nums[8], NULL, &dest);
          break;
        case 57:
          SDL_RenderCopy(renderer, Nums[9], NULL, &dest);
          break;
        default:
          SDL_RenderCopy(renderer, Nums[0], NULL, &dest);
          break;
      }
  }

bool gameLoop(int win_w, int win_h, SDL_Renderer *renderer, SDL_Event event, SDL_Rect dest[win_w][win_h], SDL_Texture *Texture[7], SDL_Texture *Nums[10], int slots[win_w][win_h], int *redScore, int *yellowScore)
  {
    int startFrame = 0, ticks = 0, framesPerSecond = 30, ticksPerFrame = 1000/framesPerSecond, turn = 1, won = 0;
    char redBuff[8], yellowBuff[8];
    SDL_Rect HL[win_w], redScoreDest[3], yellowScoreDest[3];

    for(int x = 0;x < 3;x++)
      {
        redScoreDest[x].x = (win_w * 64 / 2) - 224 + (64 * x);
        redScoreDest[x].y = 0;
        redScoreDest[x].w = 64;
        redScoreDest[x].h = 64;

        yellowScoreDest[x].x = (win_w * 64 / 2) + 32 + (64 * x);
        yellowScoreDest[x].y = 0;
        yellowScoreDest[x].w = 64;
        yellowScoreDest[x].h = 64;
      }

    for(int x = 0;x < win_w;x++)
      {
        HL[x].x = x * 64;
        HL[x].y = 32;
        HL[x].w = 64;
        HL[x].h = 64;
      }
    while(won == 0)
      {
        SDL_RenderClear(renderer);
        startFrame = SDL_GetTicks();
        while(SDL_PollEvent(&event))
          {
            switch(event.type)
              {
                case SDL_MOUSEBUTTONDOWN:
                  int x, y;
                  SDL_GetMouseState(&x, &y);
                  switch(event.button.button)
                    {
                      case SDL_BUTTON_LEFT:
                        for(int j = 0;j < win_w;j++)
                          {
                            if(dest[j][0].x < x && x < dest[j][0].x + 64)
                              {
                                for(int i = win_h - 1;i >= 0;i--)
                                  {
                                    if(slots[j][i] == 0)
                                      {
                                        if(turn % 2 == 0)
                                          slots[j][i] = 2;
                                        else
                                          slots[j][i] = 1;
                                        i = -1;
                                        turn++;
                                      }
                                  }
                              }
                          }
                    }
                  break;
                case SDL_KEYDOWN:
                  switch(event.key.keysym.sym)
                    {
                    case SDLK_r:
                      *redScore += 1;
                      return false;
                      break;
                    case SDLK_y:
                      *yellowScore += 1;
                      return false;
                      break;
                    }
                  break;
                case SDL_QUIT:
                  return true;
                  break;
              }
          }
        for(int i = 0;i < win_h;i++)
          {
            for(int j = 0;j < win_w;j++)
              {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if(dest[j][0].x < x && x < dest[j][0].x + 64)
                  {
                    if(turn % 2 == 0)
                      SDL_RenderCopy(renderer, Texture[1], NULL, &HL[j]);
                    else
                      SDL_RenderCopy(renderer, Texture[0], NULL, &HL[j]);
                  }
                if(slots[j][i] == 0)
                  SDL_RenderCopy(renderer, Texture[2], NULL, &dest[j][i]);
                else if(slots[j][i] == 1)
                  SDL_RenderCopy(renderer, Texture[3], NULL, &dest[j][i]);
                else
                  SDL_RenderCopy(renderer, Texture[4], NULL, &dest[j][i]);
              }
          }

        sprintf(redBuff, "%i", *redScore);
        sprintf(yellowBuff, "%i", *yellowScore);
        for(int i = 0;i < 3;i++)
          {
            displayNumber(renderer, redBuff[i+1], redScoreDest[i], Nums);
            displayNumber(renderer, yellowBuff[i+1], yellowScoreDest[i], Nums);
          }

        SDL_RenderPresent(renderer);
        ticks = startFrame - SDL_GetTicks();
        if(ticks < ticksPerFrame)
          SDL_Delay(ticksPerFrame - ticks);
        if(won != 0)
          {
            SDL_Delay(ticksPerFrame * framesPerSecond * 3);
            return false;
          }
      }
    return false;
  }

int main()
  {
    int win_w = 7, win_h = 6, slots[win_w][win_h], redScore = 1000, yellowScore = 1000;
    bool quit = false;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("OpenSans-Bold.ttf", 24);
    SDL_Color whiteFont = {255, 255, 255, 0};

    window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * win_w, 64 * (win_h + 1), SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *Texture[7];
    SDL_Surface *Surface = SDL_LoadBMP("./red.bmp");
    Texture[0] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = SDL_LoadBMP("./yellow.bmp");
    Texture[1] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = SDL_LoadBMP("./slot.bmp");
    Texture[2] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = SDL_LoadBMP("./slot_red.bmp");
    Texture[3] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = SDL_LoadBMP("./slot_yellow.bmp");
    Texture[4] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "Red Wins!!!", whiteFont);
    Texture[5] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "Yellow Wins!!!", whiteFont);
    Texture[6] = SDL_CreateTextureFromSurface(renderer, Surface);

    SDL_Texture *Nums[10];
    Surface = TTF_RenderText_Solid(font, "0", whiteFont);
    Nums[0] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "1", whiteFont);
    Nums[1] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "2", whiteFont);
    Nums[2] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "3", whiteFont);
    Nums[3] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "4", whiteFont);
    Nums[4] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "5", whiteFont);
    Nums[5] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "6", whiteFont);
    Nums[6] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "7", whiteFont);
    Nums[7] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "8", whiteFont);
    Nums[8] = SDL_CreateTextureFromSurface(renderer, Surface);
    Surface = TTF_RenderText_Solid(font, "9", whiteFont);
    Nums[9] = SDL_CreateTextureFromSurface(renderer, Surface);

    SDL_FreeSurface(Surface);

    SDL_Rect dest[win_w][win_h];
    for(int i = 0;i < win_h;i++)
      {
        for(int j = 0;j < win_w;j++)
          {
            dest[j][i].x = j * 64;
            dest[j][i].y = (i + 1) * 64;
            dest[j][i].w = 64;
            dest[j][i].h = 64;
          }
      }

    while(!quit)
      {
        for(int i = 0;i < win_h;i++)
          {
            for(int j = 0;j < win_w;j++)
              {
                slots[j][i] = 0;
              }
          }
        quit = gameLoop(win_w, win_h, renderer, event, dest, Texture, Nums, slots, &redScore, &yellowScore);
      }
    return 0;
  }
