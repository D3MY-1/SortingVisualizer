#include <SDL.h>
#include <iostream>
#include "Visualizer.h"
#include <algorithm>
#include <numeric>

#undef main


namespace Test
{
    template <class _Iter>
    void BubblesortA(_Iter first, _Iter Last)
    {
        bool sorted = false;
        int end = 0;

        while (!sorted)
        {
            sorted = true;

            int a;
            int b = 0;

            for (auto i = first; ; i++)
            {
                a = i - first;
                if (std::next(i) == Last - end)
                {
                    Visualizer::Swap(b + 1);
                    end++;
                    break;
                }
                
                Visualizer::Comparison(a + 1);
                if (*std::next(i) < *i)
                {
                    
                    std::iter_swap(i, std::next(i));
                    sorted = false;
                    b = a;
                }
                Visualizer::Draw();
            }
        }
    }

    void BubblesortB(std::vector<int>& vec)
    {
        bool sorted = false;

        int end = vec.size();

        int temp = 0;

        while (!sorted)
        {
            sorted = true;

            for (int i = 0; ; i++)
            {
                if (i + 1 >= end)
                {
                    Visualizer::Swap(end - 1);
                    break;
                }

                Visualizer::Comparison(i + 1);
                if (vec[i + 1] < vec[i])
                {
                    temp = vec[i + 1];
                    vec[i + 1] = vec[i];
                    vec[i] = temp;
                    sorted = false;
                    temp = vec.size();
                    temp = i;
                }
                Visualizer::Draw();
            }
            end = temp + 1;
        }
    }

    void ShakerSortB(std::vector<int>& vec)
    {
        bool sorted = false;

        bool forward = true;

        int end = vec.size();

        int start = 0;

        int temp = 0;

        int i = 0;

        while (!sorted)
        {
            sorted = true;

            
            for (i = forward ? start : end - 1; ; forward ? i++ : i--)
            {
                if (forward ? i + 1 >= end : i < start)
                {
                    Visualizer::Swap(forward ? end - 1 : start);
                    break;
                }

                Visualizer::Comparison(forward ? i + 1 : i);
                if (vec[i + 1] < vec[i])
                {
                    temp = vec[i + 1];
                    vec[i + 1] = vec[i];
                    vec[i] = temp;
                    sorted = false;
                    temp = forward ? vec.size() : 0;
                    temp = i;
                }
                Visualizer::Draw();
            }
            forward ? end = temp + 1 : start = temp + 1;
            forward = !forward;
        }
    }

    void GnomeSortB(std::vector<int>& vec)
    {
        int pos = 0;
        int temp = 0;

        bool yes = false;

        while (pos < vec.size())
        {
            if (pos == 0 || vec[pos] >= vec[pos - 1])
            {
                if (yes)
                    Visualizer::Swap(pos);
                pos++;
                yes = false;
            }
            else
            {
                temp = vec[pos - 1];
                vec[pos - 1] = vec[pos];
                vec[pos] = temp;
                pos--;
                yes = true;
            }
            Visualizer::Comparison(pos < vec.size() ? pos : pos - 1);
            Visualizer::Draw();
        }
    }

    void SelectionSortB(std::vector<int>& vec)
    {

        int it = 0;

        int start = 0;

        bool sorted = false;



        int temp = 0;

        while (!sorted)
        {
            it = start;
            sorted = true;
            for (int i = start; i < vec.size(); i++)
            {
                Visualizer::Comparison(i);
                if (vec[it] > vec[i])
                {
                    it = i;
                    sorted = false;
                }
                Visualizer::Draw();
            }
            if (sorted)
            {
                for (int i = 0; i + 1 < vec.size(); i++)

                    if (vec[i] > vec[i + 1])
                    {
                        sorted = false;
                        break;
                    }
            }

            Visualizer::Swap(start);
            temp = vec[it];
            vec[it] = vec[start];
            vec[start] = temp;
            start++;
        }
    }
}

template <typename T>
T GetInput()
{
    std::cout << ">>> ";
    T answer = 0;
    bool wrong = true;
    while (wrong)
    {
        wrong = false;
        std::cin >> answer;
        if (!std::cin)
        {
            wrong = true;
            std::cin.clear();
            std::cout << "Invalid Input!!!\n >>> ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return answer;
}

int main(int argc, char* argv[]) {

    if ((SDL_Init(SDL_INIT_EVERYTHING) == -1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    while (true)
    {
        std::cout << "Hello to visualizer 0.1v !!!\n";
        std::cout << "Choose 1 of 4 sorting algorithms!\n 1. Bubble sort\n 2. Shaker sort \n 3. Gnome sort\n 4. Selection sort\n";
        bool wrong = true;

        int result;

        do
        {
            result = GetInput<int>();
        } while (result >= 5 || result <= 0);

        std::cout << " Nice!\n Now you need to input Speed of algorythm and Amount of elements!!!\n";
        std::cout << " Enter Speed (0 for max speed)!!!\n";

        int Speed;
        do
        {
            Speed = GetInput<int>();
        } while (Speed < 0);
        std::cout << " Enter Amount of elements (10 min | 800 max)!!!\n";
        int Amount;
        do
        {
            Amount = GetInput<int>();
        } while (Amount < 10 || Amount > 800);

        std::cout << "Nice!!!\n Visualizing...";
        Visualizer::SetupWindow(800, 800);

        Visualizer::Setup(Speed, Amount);
        
        Visualizer::tSort2 sort = nullptr;

        switch (result)
        {
        case 1:
            sort = Test::BubblesortB;
            break;
        case 2:
            sort = Test::ShakerSortB;
            break;
        case 3:
            sort = Test::GnomeSortB;
            break;
        case 4:
            sort = Test::SelectionSortB;
        }

        Visualizer::Start(sort);
        system("cls");
        SDL_Delay(2000);
        Visualizer::Stop();
    }

    

 //   void* f = static_cast<void(*)(std::vector<int>::iterator, std::vector<int>::iterator)>(Test::BubblesortA);

    

 //   Visualizer vs;
 //   vs.Render(1,1,&std::sort);

 //0   template <typename Iterator>
//    void* f = static_cast<void(*)(std::vector<int>::iterator, std::vector<int>::iterator)>(std::sort);
//    std::function<void(std::vector<int>::iterator, std::vector<int>::iterator)> fs = static_cast<void(*)(std::vector<int>::iterator, std::vector<int>::iterator)>(std::sort);

//    fs(a.begin(), a.end());

 //   std::vector<int> a{34,123,234,312412,56,36,};

//    sortA(a.begin(), a.end());

//    for (auto b : a)
//    {
//        std::cout << b << ' ';
//    }

    return 0;
}