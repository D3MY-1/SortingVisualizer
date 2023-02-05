#include <SDL.h>
#include <iostream>
#include "Visualizer.h"
#include <algorithm>
#include <numeric>



#undef main


void HighlightRed(int idx)
{
    Visualizer::Highlight(idx, { 255,0,0 });
}

void isInPlace(int idx)
{
    Visualizer::Highlight(idx, { 0,255,0 });
}

namespace Test
{
    template <class _Iter,typename Pred>
    void BubblesortA(_Iter first, _Iter Last,Pred pr)
    {
        bool sorted = false;
        int end = 0;

        while (!sorted)
        {
            sorted = true;

            int a;

            for (auto i = first; ; i++)
            {
                a = i - first;
                if (std::next(i) == Last - end)
                {
                    end++;
                    break;
                }
                //Visualizer::Comparison(a + 1);
                if (pr(*std::next(i),*i))
                {
                    
                    std::iter_swap(i, std::next(i));
                    sorted = false;
                    //b = a;
                }
                //Visualizer::Draw();
            }
        }
    }

    void BubblesortB(std::vector<Visualizer::IntTracked>& vec)
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
                    isInPlace(end - 1);
                    break;
                }

                HighlightRed(i + 1);
                if (vec[i + 1] < vec[i])
                {
                    Visualizer::Swap(i, i + 1);
                    sorted = false;
                    temp = vec.size();
                    temp = i;
                }
                Visualizer::Update();
            }
            end = temp + 1;
        }
    }

    void ShakerSortB(std::vector<Visualizer::IntTracked>& vec)
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
                    isInPlace(forward ? end - 1 : start);
                    break;
                }

                HighlightRed(forward ? i + 1 : i);
                if (vec[i + 1] < vec[i])
                {
                    Visualizer::Swap(i, i + 1);
                    sorted = false;
                    temp = forward ? vec.size() : 0;
                    temp = i;
                }
                Visualizer::Update();
            }
            forward ? end = temp + 1 : start = temp + 1;
            forward = !forward;
        }
    }

    void GnomeSortB(std::vector<Visualizer::IntTracked>& vec)
    {
        int pos = 0;
        int temp = 0;

        bool yes = false;

        while (pos < vec.size())
        {
            if (pos == 0 || vec[pos] >= vec[pos - 1])
            {
                if (yes)
                    isInPlace(pos);
                pos++;
                yes = false;
            }
            else
            {
                Visualizer::Swap(pos, pos - 1);
                pos--;
                yes = true;
            }
            HighlightRed(pos < vec.size() ? pos : pos - 1);
            Visualizer::Update();
        }
    }

    void SelectionSortB(std::vector <Visualizer::IntTracked> & vec)
    {

        int it = 0;

        int start = 0;

        bool sorted = false;



        Visualizer::IntTracked temp = 0;

        while (!sorted)
        {
            it = start;
            sorted = true;
            for (int i = start; i < vec.size(); i++)
            {
                HighlightRed(i);
                HighlightRed(it);
                if (vec[it] > vec[i])
                {
                    it = i;
                    sorted = false;
                }
                Visualizer::Update();
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
            isInPlace(start);
            temp = vec[it];
            vec[it] = vec[start];
            vec[start] = temp;
            start++;
        }
    }

    void InsertionSortB(std::vector<Visualizer::IntTracked>& vec)
    {
        int i, j,key;
        for (i = 1; i < vec.size(); i++)
        {
            key = int(vec[i]);
            j = i - 1;

            

            while (j >= 0 && vec[j] > key)
            {
                HighlightRed(j);
                Visualizer::Update();
                vec[j + 1] = vec[j];
                j = j - 1;
                
            }
            vec[j + 1] = key;
            isInPlace(j + 1);
            
        }
        Visualizer::Update();
    }

    void RadixSortB(std::vector<Visualizer::IntTracked>& vec)
    {
        auto countingSort = [](std::vector<Visualizer::IntTracked>& vec, int exp) {
            std::vector<int> outp(vec.size());
            int i, count[10] = { 0 };

            for (i = 0; i < vec.size(); i++)
                count[(int(vec[i]) / exp) % 10]++;

            for (i = 1; i < 10; i++)
                count[i] += count[i - 1];

            for (i = vec.size() - 1; i >= 0; i--)
            {
                outp[count[(int(vec[i]) / exp) % 10] - 1] = int(vec[i]);
                count[(int(vec[i]) / exp) % 10]--;
            }

            for (i = 0; i < vec.size(); i++)
            {
                vec[i] = outp[i];
                HighlightRed(i);
                Visualizer::Update();
            }
                
        };

        int max = int( * std::max_element(vec.begin(), vec.end()));

        for (int exp = 1; max / exp > 0; exp *= 10)
            countingSort(vec, exp);

        Visualizer::Update();
        
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
        std::cout << "Hello to visualizer 0.2v !!!\n";
        std::cout << "Choose 1 of 8 sorting algorithms!\n 1. Bubble sort\n 2. Shaker sort \n 3. Gnome sort\n 4. Selection sort\n 5. Insertion sort\n 6. Radix sort\n 7. std::sort\n 8. std::stable_sort\n";
        bool wrong = true;

        int result;

        do
        {
            result = GetInput<int>();
        } while (result >= 9 || result <= 0);

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

        std::cout << " Nice!!!\n Visualizing...";
        Visualizer::SetupWindow(800, 800);

        Visualizer::Setup(Speed, Amount);
        
        
        switch (result)
        {
        case 1:
            Visualizer::Start(Test::BubblesortA);
            break;
        case 2:
            Visualizer::Start(Test::ShakerSortB);
            break;
        case 3:
            Visualizer::Start(Test::GnomeSortB);
            break;
        case 4:
            Visualizer::Start(Test::SelectionSortB);
            break;
        case 5:
            Visualizer::Start(Test::InsertionSortB);
            break;
        case 6:
            Visualizer::Start(Test::RadixSortB);
            break;
        case 7:
            Visualizer::Start(std::sort);
            break;
        case 8:
            Visualizer::Start(std::stable_sort);
        }

        auto stats = Visualizer::GetStats();
        std::cout << "\n Array was sorted!!!\n Stats\n Number of comparisons is " << stats.first << "\n Number of Array changes is " << stats.second << "\n Press Enter To Continue...";
        getchar();

        system("cls");
    }

    return 0;
}