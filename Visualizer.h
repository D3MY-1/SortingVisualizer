#pragma once
#include <SDL.h>
#include <random>
#include <vector>
#include "ElemTracker.h"
#include <set>
#include <thread>
#include <mutex>
#include <unordered_map>

class Visualizer
{
public:

	static void SetupWindow(int width, int height);

	static void Setup(int UpdatePerSecond, int ArraySize);

	typedef ElemTracker<int> IntTracked;
	typedef bool (*tPred)(const IntTracked&, const IntTracked&);
	typedef void(*tSort)(std::vector<IntTracked>::iterator, std::vector<IntTracked>::iterator, tPred f);
	static void Start(tSort func, bool AutoVisualize = true);
	//typedef void(*tIndexArray)(std::vector<IntTracked>&);
	typedef void(*tElemArray)(std::vector<IntTracked>&);
	static void Start(tElemArray vec);

	typedef void(*tSort2)(std::vector<IntTracked>&, tPred f);
	static void Start(tSort2 vec, bool AutoVisualize = true);
	/// <summary>
	/// Highlights element with red color
	/// </summary>
	/// <param name="ElemIdx index of element that will be highlited with col"></param>
	/// <param anme = "col" color in what element will be highlited></param>
	static void Highlight(int ElemIdx,SDL_Color col);

	static std::pair<std::int64_t, std::int64_t> GetStats();
	
	static void Swap(int iDx1, int iDx2);

	/// <summary>
	/// Updates highlights array and controls sorting speed.
	/// Call after comparison was performed
	/// </summary>
	static void Update();




private:
	class ColorHash
	{
	public:
		size_t operator()(const SDL_Color& col) const
		{
			return ((col.r & 0xff) << 24) + ((col.g & 0xff) << 16) + ((col.b & 0xff) << 8) + (col.a & 0xff);
		}

		bool operator()(const SDL_Color& l, const SDL_Color& r) const
		{
			return l.r == r.r && l.g == r.g && r.b == l.b && r.a == l.a;
		}
	};

private:
	static void draw(SDL_Renderer* renderer);

	static void Events();

	static void start();

	static inline std::int64_t compCount;
	static inline std::int64_t arrayChangesCount;

	static inline std::chrono::microseconds delay;
	static inline std::chrono::microseconds draw_delay;
	static inline std::vector<IntTracked> array;
	static inline std::mt19937 rng;

	/* TODO: Make one array that will handle all colors 
			 Maybe make an array that will contain all pillar collors so it will eliminate unnenasary drawing 
	*/
	// For Multithreading
	typedef std::unordered_map<SDL_Color, std::set<int>, ColorHash, ColorHash> tDrawable;

	static inline tDrawable highlightDrawing;
	static inline tDrawable drawing;



	static inline tDrawable highlight;

	static inline std::mutex comp_mutex;

	static inline int windowWith = 800;
	static inline int windowHeight = 800;

	static inline int paddingPx;

	static inline std::thread drawThread;

	static inline bool running;

	static constexpr int FPS = 60;

	

};