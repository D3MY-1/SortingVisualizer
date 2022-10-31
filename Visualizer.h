#pragma once
#include <SDL.h>
#include <random>
#include <vector>

class Visualizer
{
public:

	static bool SetupWindow(int width, int height);

	static void Setup(int UpdatePerSecond, int ArraySize);
	typedef void(*tSort)(std::vector<int>::iterator, std::vector<int>::iterator);
	static void Start(void* func);
	typedef void(*tSort2)(std::vector<int>&);
	static void Start(tSort2 vec);
	/// <summary>
	/// Call When Comparison is performed
	/// </summary>
	/// <param name="Elem1 pass 1 element that is compared"></param>
	/// <param name="Elem2 pass 2 element that is compared"></param>
	static void Comparison(int Elem);
	/// <summary>
	/// Call When Swapping is performed
	/// </summary>
	/// <param name="Elem1 pass 1 element that is Swapped"></param>
	/// <param name="Elem2 pass 2 element that is Swaapped"></param>
	static void Swap(int Elem);
	/// <summary>
	/// Call at end of 1 cycle
	/// </summary>
	static void Draw();

	static void Stop();

private:

	static void Events();

	static inline float delay;
	static inline std::vector<int> array;
	static inline std::mt19937 rng;

	static inline std::vector<int> comp;
	static inline std::vector<int> swap;

	static inline SDL_Window* window;
	static inline SDL_Renderer* renderer;

	static inline int windowWith;
	static inline int windowHeight;

	static inline int paddingPx;

	static inline bool running;

};