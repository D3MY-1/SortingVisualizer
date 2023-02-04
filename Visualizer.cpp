#include "Visualizer.h"

void Visualizer::SetupWindow(int width, int height)
{
	windowWith = width;
	windowHeight = height;
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

	
	delay = UpdatePerSecond > 0 ?  std::chrono::milliseconds(1000) / UpdatePerSecond : std::chrono::microseconds(0);
	draw_delay = std::chrono::milliseconds(1000) / FPS;
	array = std::vector<int>(ArraySize);
	indexes = std::vector<IntElem>(ArraySize);

	IntElem::SetArray(&array);

	for (int i = 0; i < ArraySize; i++)
	{
		indexes[i] = i;
		indexes[i].setIndex(i);
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
		bool c = a < b;
		
		return c;
	};
	std::shuffle(array.begin(), array.end(), rng);
	running = true;

	drawThread = std::thread(start);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	func(indexes.begin(), indexes.end(),lamda);
	running = false;
	drawThread.join();

	for (int i = 0; i < indexes.size(); i++)
	{
		indexes[i] = i;
	}

	

	
	comp = std::set<int>();
	compDrawing = std::set<int>();
	swap = std::vector<int>();
	history = std::set<int>();
}

void Visualizer::Start(tElemArray f)
{
	IntElem::DisableTracking();
	std::shuffle(array.begin(), array.end(), rng);
	
	running = true;
	drawThread = std::thread(start);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	f(array);
	running = false;
	drawThread.join();
	
	comp = std::set<int>();
	compDrawing = std::set<int>();
	swap = std::vector<int>();
	history = std::set<int>();
}

void Visualizer::HighlightRed(int ElemIdx)
{
	comp.insert(ElemIdx);
}

void Visualizer::isInPlace(int ElemIdx)
{
	swap.clear();
	swap.emplace_back(ElemIdx);
}

void Visualizer::Update()
{
	if (!running)
		return;
	auto start = std::chrono::high_resolution_clock::now();
	{
		std::lock_guard<std::mutex> guard(comp_mutex);

		for (auto a : comp)
		{
			compDrawing.insert(a);
		}
		comp.clear();
	}
	
	
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	if (delay > elapsed)
		std::this_thread::sleep_for(delay - elapsed);
		
}


/* TODO: Rewrite function so that it draws only 60 times or 120
         Make another function that will calculate how much time and so on
*/
void Visualizer::draw(SDL_Renderer* renderer)
{
	
	
	//std::cout << comp.size() << '\n';

		//Events();
	auto start = std::chrono::high_resolution_clock::now();
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
	

	std::set<int> draw;
	{
		std::lock_guard<std::mutex> guard(comp_mutex);

		if (compDrawing.size() > 0)
		{
			draw = compDrawing;
			history = compDrawing;
		}
			
		else
			draw = history;

		
		
		
		compDrawing.clear();
	}

	for (auto& a : draw)
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

	SDL_RenderPresent(renderer);

	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	if (floor(Visualizer::draw_delay.count() - elapsed.count()) > 0)
		std::this_thread::sleep_for(draw_delay - elapsed);

	
		
	
}

void Visualizer::start()
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	

	window = SDL_CreateWindow(
		"Cringe visuals",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWith, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (!window) {
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		return;
	}
	while (running)
	{
		Events();
		draw(renderer);
	}
	
	draw(renderer);

	SDL_Delay(2000);

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