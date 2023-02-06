#include "Visualizer.h"

void Visualizer::SetupWindow(int width, int height)
{
	windowWith = width;
	windowHeight = height;
}



void Visualizer::Setup(int UpdatePerSecond, int ArraySize)
{
	IntTracked::EnableTracking();
	
	

	if (ArraySize * 4 < windowWith)
		paddingPx = 2;
	else if (ArraySize * 2 < windowWith)
		paddingPx = 1;
	else
		paddingPx = 0;

	
	delay = UpdatePerSecond > 0 ?  std::chrono::milliseconds(1000) / UpdatePerSecond : std::chrono::microseconds(0);
	draw_delay = std::chrono::milliseconds(1000) / FPS;
	array = std::vector<IntTracked>();
	array.reserve(ArraySize);

	//IntElem::SetArray(&array);

	for (int i = 0; i < ArraySize; i++)
	{
		array.emplace_back(i + 1, i);
	}

	std::random_device rd;
	rng = std::mt19937(rd());
}


void Visualizer::preStart(bool AutoVisualize)
{
	IntTracked::EnableTracking();
	bool resetDelay = false;
	if(delay == std::chrono::milliseconds(0))
	{
		resetDelay = true;
		long a = 1600000 / array.size();
		delay = std::chrono::microseconds(a);
	}

	static bool isShuffled;
	isShuffled = false;
	IntTracked::SetComp([](int a,int b) {
		if (a != -1)
			Highlight(a, { 255,0,0 });
		if (b != -1)
			Highlight(b, { 255,0,0 });
		//compCount++; });
		});
	IntTracked::SetChange([](int a) {
		Highlight(a, { 255,0,0 });
		arrayChangesCount++; 
		if (!isShuffled)
			Update();
		});

	//std::shuffle()
	std::shuffle(array.begin(), array.end(),rng);

	isShuffled = true;
	if (resetDelay == true)
		delay = std::chrono::microseconds(0);

	if (!AutoVisualize)
	{
		IntTracked::SetComp([](int a,int b) {
			compCount++; 
			});
		IntTracked::SetChange([](int a) {
			arrayChangesCount++;
			if (!isShuffled)
				Update();
			});
	}


	compCount = 0;
	arrayChangesCount = 0;
	IntTracked::DisableTracking();
}

void Visualizer::Start(tSort2 func, bool AutoVisualize)
{
	

	

	
	
	running = true;
	drawThread = std::thread(start);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	preStart(AutoVisualize);

	IntTracked::EnableTracking();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto lamda = [](const IntTracked& a, const IntTracked& b)
	{
		bool c = a < b;
		compCount++;
		Visualizer::Update();
		return c;
	};

	func(array, lamda);
	running = false;
	drawThread.join();



	
	highlight = {};
}


void Visualizer::Start(tSort func, bool AutoVisualize)
{
	

	

	running = true;
	drawThread = std::thread(start);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	preStart(AutoVisualize);

	IntTracked::EnableTracking();

	auto lamda = [](const IntTracked& a, const IntTracked& b)
	{	
		bool c = a < b;
		compCount++;
		Visualizer::Update();
		return c;
	};

	

	std::this_thread::sleep_for(std::chrono::seconds(1));

	func(array.begin(), array.end(),lamda);
	running = false;
	drawThread.join();

	

	highlight = {};
}

void Visualizer::Start(tElemArray f)
{
	
	running = true;
	drawThread = std::thread(start);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	preStart(false);

	IntTracked::EnableTracking();
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	f(array);
	running = false;
	drawThread.join();
	
	highlight = {};
}

void Visualizer::Highlight(int ElemIdx,SDL_Color col)
{
	highlight[col].insert(ElemIdx);
}

void Visualizer::Update()
{
	if (!running)
		return;
	auto start = std::chrono::high_resolution_clock::now();
	{
		std::lock_guard<std::mutex> guard(comp_mutex);

		for (auto& a : highlight)
			highlightDrawing[a.first].merge(std::move(a.second));
		highlight = {};
	}
	
	
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	if (delay > elapsed)
		std::this_thread::sleep_for(delay - elapsed);
		
}

std::pair<std::int64_t,std::int64_t> Visualizer::GetStats()
{
	return { compCount,arrayChangesCount };
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
		rect = SDL_FRect{ (float)(widthPer1 + paddingPx) * i,windowHeight - floor(((float)windowHeight / (float)array.size()) * int(array[i])) ,(float)widthPer1,floor(((float)windowHeight / (float)array.size()) * int(array[i])) };
			SDL_RenderFillRectF(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	
	{
		std::lock_guard<std::mutex> guard(comp_mutex);


		for (auto& a : highlightDrawing)
		{
			if (a.second.size() > 0)
				drawing[a.first] = std::move(a.second);

		}
		highlightDrawing = {};
		//highlightDrawing.clear();
	}

	for (auto& elem : drawing)
	{
		auto col = elem.first;
		SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b,col.a);
		for (auto& a : elem.second)
		{
			rect = SDL_FRect{ (float)(widthPer1 + paddingPx) * a,windowHeight - floor(((float)windowHeight / (float)array.size()) * int(array[a])) ,(float)widthPer1,floor(((float)windowHeight / (float)array.size()) * int(array[a])) };
			SDL_RenderFillRectF(renderer, &rect);
		}
		
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

	drawing = {};

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
	highlightDrawing = {};

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
	IntTracked temp = array[iDx1];
	array[iDx1] = array[iDx2];
	array[iDx2] = temp;
}