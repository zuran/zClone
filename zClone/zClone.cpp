// zClone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>

#include "Color.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** args) {
  std::cout << "Hello World!\n";
  json j;
  j["test"] = 3.5;
  std::cout << j << std::endl;


  SDL_Surface* winSurface = nullptr;
  SDL_Window* window = nullptr;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Error Init SDL" << std::endl;
    system("pause");
    return 1;
  }

  int kWidth = 256, kHeight = 240, kMag = 4;



  window =
      SDL_CreateWindow("Ex.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       kWidth * kMag, kHeight * kMag, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cout << "Error creating window" << std::endl;
    system("pause");
    return 1;
  }

  winSurface = SDL_GetWindowSurface(window);

  if (!winSurface) {
    std::cout << "Error getting surface" << std::endl;
    system("pause");
    return 1;
  }

  // Create back buffer
  SDL_Surface* backBuffer;
  backBuffer = SDL_CreateRGBSurfaceWithFormat(0, winSurface->w, winSurface->h,
                                              0, winSurface->format->format);
  SDL_FillRect(backBuffer, nullptr, 0);

  // Swap to front
  SDL_BlitScaled(backBuffer, nullptr, winSurface, nullptr);
  SDL_UpdateWindowSurface(window);

  std::cout << SDL_GetBasePath() << std::endl;

  // Load image
  static std::string path = SDL_GetBasePath();
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);

  std::string p = (path + "../Assets/turtle_walk.png");
  const char* tmpPath = p.c_str();

  SDL_Surface* image =  // SDL_LoadBMP((path + "character.bmp").c_str());
      IMG_Load(tmpPath);

  int iWidth = image->w;
  int iHeight = image->h;
  int totalSize = iWidth * iHeight;
  std::vector<int> pixels;
  pixels.reserve(totalSize);

  //SDL_LockSurface(image);
  //uint32_t* iPixels = static_cast<uint32_t*>(image->pixels);
  //for (int i = 0; i < totalSize; ++i) {
  //  pixels.push_back(iPixels[i]);
  //}
  //SDL_UnlockSurface(image);

  //SDL_LockSurface(backBuffer);
  //uint32_t* bPixels = static_cast<uint32_t*>(backBuffer->pixels);
  //for (int i = 0; i < iHeight; ++i) {
  //  for (int j = 0; j < iWidth; j++) {
  //    if (i >= kWidth || j >= kHeight) continue;
  //    const int imagePos = j + (i * iWidth);
  //    bPixels[i] = iPixels[imagePos];
  //  }
  //}
  //SDL_UnlockSurface(backBuffer);

  // Swap to front
  SDL_BlitScaled(backBuffer, nullptr, winSurface, nullptr);
  SDL_UpdateWindowSurface(window);

  //SDL_Rect rect = {0, 0, iWidth * kMag, iHeight * kMag};
  SDL_Rect src = {0, 0, 16, 16};
  SDL_Rect dest = {10, 10, src.w * kMag, src.h * kMag};
  
  SDL_Rect dest2 = {100, 100, src.w * kMag, src.h * kMag};
  SDL_Point center = {8 * kMag, 8 * kMag};
  
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  //SDL_RenderCopy(renderer, texture, &src, &dest2);
  //SDL_RenderCopyEx(renderer, texture, &src, &dest2, 45.0f, &center, SDL_FLIP_NONE);
  //SDL_RenderPresent(renderer);  

  SDL_Rect walkUp0 = {0, 0, 16, 16};
  SDL_Rect walkUp1 = {16, 0, 16, 16};

  SDL_Rect walkDown0 = {0, 16, 16, 16};
  SDL_Rect walkDown1 = {16, 16, 16, 16};

  SDL_Rect walkRight0 = {0, 32, 16, 16};
  SDL_Rect walkRight1 = {16, 32, 16, 16};

  SDL_Rect loc = {kWidth * kMag / 2, 64 * kMag + 64*kMag, 16 * kMag, 16 * kMag};

  SDL_Rect frame0 = walkDown0;
  SDL_Rect frame1 = walkDown1;

  int xDir = 1;
  int yDir = 0;

  float kFps = 30.0;
  float kFrameTime = 1000.0 / kFps;
  int currentTicks = SDL_GetTicks();
  float kSpeed = 200.0f;

  bool isRunning = true;
  bool isFlipped = false;

  float x = static_cast<float>(loc.x);
  float y = static_cast<float>(loc.y);
  float dt = 0.0;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_Texture* gridTex =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, kWidth , kHeight );
  SDL_SetRenderTarget(renderer, gridTex);

  // try drawing grid to texture first
  // Draw Major Grid
  SDL_SetRenderDrawColor(renderer, 240, 240, 240, SDL_ALPHA_OPAQUE);
  for (int row = 0; row < 11; ++row) {
    int y = 64  + (row * 16 );
    SDL_RenderDrawLine(renderer, 0, y, kWidth , y);
  }
  for (int col = 0; col < 16; ++col) {
    int x = col * 16 ;
    SDL_RenderDrawLine(renderer, x, 64 , x, kHeight );
  }

  // Draw Minor Grid
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
  for (int row = 0; row < 11; ++row) {
    int y = 72  + (row * 16 );
    SDL_RenderDrawLine(renderer, 0, y, kWidth , y);
  }
  for (int col = 0; col < 16; ++col) {
    int x = col * 16  + 8 ;
    SDL_RenderDrawLine(renderer, x, 64 , x, kHeight );
  }
  SDL_SetRenderTarget(renderer, nullptr);

  while (isRunning) {
    int frameStart = SDL_GetTicks();

    SDL_Event event;
    int eventCount = 0;
    
    while (SDL_PollEvent(&event)) {
      eventCount++;
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }
      
      const Uint8* key_state = SDL_GetKeyboardState(nullptr);

      if (key_state[SDL_SCANCODE_DOWN] && !key_state[SDL_SCANCODE_UP]) {
        frame0 = walkDown0;
        frame1 = walkDown1;
        isFlipped = false;
        xDir = 0;
        yDir = 1;
        // align to horizontal subgrid
        // subpixel loc is x,y
        float gridX = std::roundf(x / (kMag * 8));
        x = gridX * kMag * 8;
      } else if (!key_state[SDL_SCANCODE_DOWN] &&
                  key_state[SDL_SCANCODE_UP]) {
        frame0 = walkUp0;
        frame1 = walkUp1;
        isFlipped = false;
        xDir = 0;
        yDir = -1;
        // align to horizontal subgrid
        // subpixel loc is x,y
        float gridX = std::roundf(x / (kMag * 8));
        x = gridX * kMag * 8;
      } else if (key_state[SDL_SCANCODE_RIGHT] && !key_state[SDL_SCANCODE_LEFT]) {
        frame0 = walkRight0;
        frame1 = walkRight1;
        isFlipped = false;
        xDir = 1;
        yDir = 0;
        // align to vertical subgrid
        // subpixel loc is x,y
        float gridY = std::roundf(y / (kMag * 8));
        y = gridY * kMag * 8;
      } else if (!key_state[SDL_SCANCODE_RIGHT] &&
                  key_state[SDL_SCANCODE_LEFT]) {
        frame0 = walkRight0;
        frame1 = walkRight1;
        isFlipped = true;
        xDir = -1;
        yDir = 0;
        // align to vertical subgrid
        // subpixel loc is x,y
        float gridY = std::roundf(y / (kMag * 8));
        y = gridY * kMag * 8;
      } else {
        xDir = 0;
        yDir = 0;
      }
        //break;
      
        //isMoving = false;
        //break;
      
    }

    if (!isRunning) break;

    x += xDir * kSpeed * dt;
    y += yDir * kSpeed * dt;
  
    loc.x = static_cast<int>(x);
    loc.y = static_cast<int>(y);
    
    
    //SDL_RenderCopy(renderer, texture, &frame0, &loc);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, gridTex, nullptr, nullptr);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_RenderFillRect(renderer, &loc);
    

    SDL_RenderCopyEx(renderer, texture, &frame0, &loc, 0.0f, &center,
                     isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    //SDL_Delay(150);
    
    //loc.x += xDir * kMag * 2;
    //loc.y += yDir * kMag * 2;
    //SDL_RenderClear(renderer);
    //SDL_RenderCopyEx(renderer, texture, &frame1, &loc, 0.0f, &center,
    //                 isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    //SDL_RenderPresent(renderer);
    //SDL_Delay(150);
    

    

    SDL_RenderPresent(renderer);
    
    SDL_Delay(10);


    int frameEnd = SDL_GetTicks();
    int totalFrameTime = frameEnd - frameStart;
    
    dt = totalFrameTime / 1000.0f;

    if(totalFrameTime < kFrameTime) {
      //SDL_Delay(static_cast<int>(kFrameTime - totalFrameTime));
    }
  }
  


  SDL_DestroyTexture(gridTex);
  
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
