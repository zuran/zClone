// zClone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <vector>

int main(int argc, char** args) {
  std::cout << "Hello World!\n";

  SDL_Surface* winSurface = nullptr;
  SDL_Window* window = nullptr;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Error Init SDL" << std::endl;
    system("pause");
    return 1;
  }

  int kWidth = 256, kHeight = 240, kMag = 3;



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

  // Load image
  static std::string path = SDL_GetBasePath();
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface* image = SDL_LoadBMP((path + "Overworld.bmp").c_str());
  
  int iWidth = image->w;
  int iHeight = image->h;
  int totalSize = iWidth * iHeight;
  std::vector<int> pixels;
  pixels.reserve(totalSize);

  SDL_LockSurface(image);
  uint32_t* iPixels = static_cast<uint32_t*>(image->pixels);
  for (int i = 0; i < totalSize; ++i) {
    pixels.push_back(iPixels[i]);
  }
  SDL_UnlockSurface(image);

  SDL_LockSurface(backBuffer);
  uint32_t* bPixels = static_cast<uint32_t*>(backBuffer->pixels);
  for (int i = 0; i < iHeight; ++i) {
    for (int j = 0; j < iWidth; j++) {
      if (i >= kWidth || j >= kHeight) continue;
      const int imagePos = j + (i * iWidth);
      bPixels[i] = iPixels[imagePos];
    }
  }
  SDL_UnlockSurface(backBuffer);

  // Swap to front
  SDL_BlitScaled(backBuffer, nullptr, winSurface, nullptr);
  SDL_UpdateWindowSurface(window);


  //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
  //SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  //SDL_RenderPresent(renderer);


  system("pause");
  //SDL_DestroyTexture(texture);
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
