// zClone.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>

int main(int argc, char** args) {
  std::cout << "Hello World!\n";

  SDL_Surface* winSurface = nullptr;
  SDL_Window* window = nullptr;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Error Init SDL" << std::endl;
    system("pause");
    return 1;
  }

  window =
      SDL_CreateWindow("Ex.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       1280, 760, SDL_WINDOW_SHOWN);
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

  SDL_FillRect(winSurface, nullptr,
               SDL_MapRGB(winSurface->format, 255, 255, 255));
  SDL_UpdateWindowSurface(window);
  system("pause");
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
