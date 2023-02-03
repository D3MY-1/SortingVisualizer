#pragma once
#include <SDL.h>
#include <random>
#include <vector>
#include "IntElem.h"
#include <set>
#include <thread>
#include <mutex>

class Visualizer
{
public:

	static void SetupWindow(int width, int height);

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

private:
	static void draw(SDL_Renderer* renderer);

	static void Events();

	static void start();

	static inline std::chrono::microseconds delay;
	static inline std::chrono::microseconds draw_delay;
	static inline std::vector<int> array;
	static inline std::vector<IntElem> indexes;
	static inline std::mt19937 rng;

	/* TODO: Make one array that will handle all colors 
			 Maybe make an array that will contain all pillar collors so it will eliminate unnenasary drawing 
	*/
	static inline std::set<int> compDrawing;
	static inline std::set<int> comp;

	static inline std::mutex comp_mutex;


	static inline std::vector<int> swap;

	static inline int windowWith = 800;
	static inline int windowHeight = 800;

	static inline int paddingPx;

	static inline std::thread drawThread;

	static inline bool running;

	static constexpr int FPS = 60;

	

};