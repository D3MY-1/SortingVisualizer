#include "Visualizer.h"

bool Visualizer::SetupWindow(int width, int height)
{
	window = SDL_CreateWindow(
		"Cringe visuals",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	windowWith = width;
	windowHeight = height;

	if (!window) {
		return false;
	}

	// SDL_GetWindowSize(window, &vWidth, &vHeight);

	// renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		return false;
	}
	return true;
}

void Visualizer::Setup(int UpdatePerSecond, int ArraySize)
{

	IntElem::SetComp(Visualizer::Comparison);
	IntElem::SetSwap(Visualizer::isInPlace);

	if (ArraySize * 4 < windowWith)
		paddingPx = 2;
	else if (ArraySize * 2 < windowWith)
		paddingPx = 1;
	else
		paddingPx = 0;

	delay = UpdatePerSecond > 0 ?  1000.f / UpdatePerSecond : 0;
	array = std::vector<int>(ArraySize);
	indexes = std::vector<IntElem>(ArraySize);

	IntElem::SetArray(&array);
	//IntElem::SetIndexes(&indexes);

	for (int i = 0; i < indexes.size(); i++)
	{
		indexes[i] = i;
	}

	for (int i = 0; i < array.size(); i++)
	{
		array[i] = i + 1;
	}

	std::random_device rd;
	rng = std::mt19937(rd());
}




void Visualizer::Start(tSort func)
{
	IntElem::EnableTracking();
	auto lamda = [](IntElem a, IntElem b)
	{	
		Visualizer::Draw();
		return a < b;
	};
	std::shuffle(array.begin(), array.end(), rng);
	running = true;
	func(indexes.begin(), indexes.end(),lamda);
	for (int i = 0; i < indexes.size(); i++)
	{
		indexes[i] = i;
	}
}

void Visualizer::Start(tElemArray f)
{
	IntElem::DisableTracking();
	std::shuffle(array.begin(), array.end(), rng);
	running = true;
	f(array);
	comp = std::vector<int>();
	swap = std::vector<int>();
}

void Visualizer::Comparison(int Elem)
{
	comp.emplace_back(Elem);
}

void Visualizer::isInPlace(int Elem)
{
	swap.clear();
	swap.emplace_back(Elem);
}

void Visualizer::Draw()
{
	if (!running)
	{
		Stop();
		return;
	}

	Events();

	auto start = SDL_GetPerformanceCounter();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	float pad = array.size() * paddingPx;

	float widthPer1 = (windowWith - pad) / array.size();

	SDL_FRect rect;

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

	for (int i = 0; i < array.size(); i++)
	{
		rect = SDL_FRect{ (float)(widthPer1 + paddingPx) * i,windowHeight - floor(((float)windowHeight / (float)array.size()) * array[indexes[i]]) ,(float)widthPer1,floor(((float)windowHeight / (float)array.size()) * array[indexes[i]]) };
		SDL_RenderFillRectF(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	for (auto& a : comp)
	{
		rect = SDL_FRect{ (float)(widthPer1 + paddingPx) * a,windowHeight - floor(((float)windowHeight / (float)array.size()) * array[indexes[a]]) ,(float)widthPer1,floor(((float)windowHeight / (float)array.size()) * array[indexes[a]]) };
		SDL_RenderFillRectF(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	for (auto& a : swap)
	{
		rect = SDL_FRect{ (float)(widthPer1 + paddingPx) * a,windowHeight - floor(((float)windowHeight / (float)array.size()) * array[indexes[a]]) ,(float)widthPer1,floor(((float)windowHeight / (float)array.size()) * array[indexes[a]]) };
		SDL_RenderFillRectF(renderer, &rect);
	}

	comp.clear();
	//		swap.clear();

	SDL_RenderPresent(renderer);

	auto end = SDL_GetPerformanceCounter();

	float elapsed = (end - start) / SDL_GetPerformanceFrequency() * 1000.f;

	if (floor(delay - elapsed) > 0)
		SDL_Delay(floor(delay - elapsed));
}

void Visualizer::Stop()
{
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
}

void Visualizer::Events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
		default:
			break;
		}
	}
	
}

void Visualizer::Swap(int iDx1, int iDx2)
{
	int temp = array[iDx1];
	array[iDx1] = array[iDx2];
	array[iDx2] = temp;
}