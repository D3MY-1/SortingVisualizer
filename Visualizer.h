#pragma once
#include <SDL.h>
#include <random>
#include <vector>
#include "IntElem.h"
#include <set>

class Visualizer
{
public:

	static bool SetupWindow(int width, int height);

	static void Setup(int UpdatePerSecond, int ArraySize);

	typedef bool (*tFunc)(IntElem, IntElem);
	typedef void(*tSort)(std::vector<IntElem>::iterator, std::vector<IntElem>::iterator, tFunc f);
	static void Start(tSort func);
	typedef void(*tIndexArray)(std::vector<IntElem>&);
	typedef void(*tElemArray)(std::vector<int>&);
	static void Start(tElemArray vec);
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
	static void isInPlace(int Elem);
	/// <summary>
	/// Call at end of 1 cycle
	/// </summary>
	
	static void Swap(int iDx1, int iDx2);

	static void Draw();

	static void Stop();

private:
	

	static void Events();

	static inline float delay;
	static inline std::vector<int> array;
	static inline std::vector<IntElem> indexes;
	static inline std::mt19937 rng;

	/* TODO: Make one array that will handle all colors 
			 Maybe make an array that will contain all pillar collors so it will eliminate unnenasary drawing 
	*/
	static inline std::set<int> comp;


	static inline std::vector<int> swap;

	static inline SDL_Window* window;
	static inline SDL_Renderer* renderer;

	static inline int windowWith;
	static inline int windowHeight;

	static inline int paddingPx;

	static inline bool running;

};