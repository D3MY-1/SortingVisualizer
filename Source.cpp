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

    template<typename _Iter, typename _Pred>
    void InsertionSortA(_Iter begin, _Iter end, _Pred pred)
    {
        for (_Iter i = begin + 1; i != end; i++)
        {
            _Iter j = i;
            while (j > begin && pred(*j, *(j - 1)))
            {
                std::iter_swap(j - 1, j);
                j--;
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

    void QSort_HuareC(std::vector<Visualizer::IntTracked>& vec,Visualizer::tPred pred)
    {
        auto qsort = [&](int start, int end, auto&& sort)->void {
            if (start < end)
            {
                int privotIndex = start;
                int left = start - 1;
                int right = end + 1;
                while (true)
                {
                    do
                    {
                        left++;

                    } while (pred(vec[left], vec[privotIndex]));

                    do
                    {
                        right--;

                    } while (pred(vec[privotIndex], vec[right]));

                    if (left >= right)
                    {
                        break;
                    }
                    std::swap(vec[left], vec[right]);
                }
                sort(start, right, sort);
                sort(right + 1, end, sort);
                //std::swap(vec[privotIndex], vec[right]);
                

            }
        };

        qsort(0, vec.size() - 1, qsort);

    }
    void QSortC(std::vector<Visualizer::IntTracked>& vec, Visualizer::tPred pred)
    {
        constexpr int threshold = 16;
        auto qsort = [&](int start, int end, auto&& sort)->void {
            if (end - start > threshold)
            {
                int mid = (start + end) / 2;
                if (pred(vec[mid], vec[start]))
                    std::swap(vec[start], vec[mid]);
                if (pred(vec[end], vec[start]))
                    std::swap(vec[start], vec[end]);
                if (pred(vec[end], vec[mid]))
                    std::swap(vec[end], vec[mid]);
                auto privotIdx = mid;
                int left = mid;
                int right = mid;

                while (true)
                {
                    do
                    {
                        left--;
                    } while (left > start && pred(vec[left], vec[privotIdx]));

                    do
                    {
                        right++;
                    } while (right < end && pred(vec[privotIdx], vec[right]));

                    if (right >= end || left <= start)
                    {
                        if (left > start)
                        {
                            std::swap(vec[left], vec[privotIdx - 1]);
                            std::swap(vec[privotIdx - 1], vec[privotIdx]);
                            privotIdx--;
                        }
                        else if (right < end)
                        {
                            std::swap(vec[right], vec[privotIdx + 1]);
                            std::swap(vec[privotIdx + 1], vec[privotIdx]);
                            privotIdx++;
                        }
                        else
                            break;
                    }
                    else
                        std::swap(vec[left], vec[right]);

                }
                sort(start, privotIdx, sort);
                sort(privotIdx, end, sort);

            }
            else if (end > start)
                InsertionSortA(vec.begin() + start, vec.begin() + end, pred);
        };

        qsort(0, vec.size() - 1, qsort);


    }
    void QSort_LomutoC(std::vector<Visualizer::IntTracked>& vec,Visualizer::tPred pred)
    {
        auto sort = [&](int low, int high, auto&& sort)->void
        {
            if (low < high)
            {
                int pivotIdx = high;
                int i = low - 1;
                for (int j = low; j <= high - 1; j++)
                {
                    if (!pred(vec[pivotIdx], vec[j]))
                    {
                        i++;
                        std::swap(vec[i], vec[j]);
                    }
                }
                std::swap(vec[i + 1], vec[high]);
                sort(low, i, sort);
                sort(i + 2, high, sort);
            }

        };
        sort(0, vec.size() - 1, sort);
    }

    void QSort_SedgewickC(std::vector<Visualizer::IntTracked>& vec, Visualizer::tPred pred)
    {
        constexpr int threshold = 16;
        auto qsort = [&](int start, int end, auto&& sort)->void {

            int left = start - 1;
            int right = end + 1;
            if (end - start > threshold)
            {
                int mid = (start + end) / 2;
                if (pred(vec[mid], vec[start]))
                    std::swap(vec[start], vec[mid]);
                if (pred(vec[end], vec[start]))
                    std::swap(vec[start], vec[end]);
                if (pred(vec[end], vec[mid]))
                    std::swap(vec[end], vec[mid]);
                auto privot = vec[mid];
                
                while (true)
                {
                    do
                    {
                        left++;

                    } while (pred(vec[left], privot));

                    do
                    {
                        right--;

                    } while (pred(privot, vec[right]));

                    if (left >= right)
                    {
                        break;
                    }
                    std::swap(vec[left], vec[right]);
                }
                sort(start, right, sort);
                sort(right + 1, end, sort);
            }
            else if (end > start)
                Test::InsertionSortA(vec.begin() + start, vec.begin() + end + 1, pred);
        };

        qsort(0, vec.size() - 1, qsort);
        
    }

    void HeapSortC(std::vector<Visualizer::IntTracked>& vec,Visualizer::tPred pred)
    {
        auto sort = [&](int i,int N,auto&& sort)->void {
            int largest = i;

            int l = 2 * i + 1;

            int r = 2 * i + 2;

            if (l < N && pred(vec[largest], vec[l]))
                largest = l;
            if (r < N && pred(vec[largest], vec[r]))
                largest = r;
            if (largest != i)
            {
                std::swap(vec[i], vec[largest]);
                sort(largest,N, sort);
            }
        };

        for (int i = vec.size() / 2 - 1; i >= 0; i--)
            sort(i,vec.size(), sort);

        for (int i = vec.size() - 1; i > 0; i--)
        {
            std::swap(vec[0], vec[i]);


            sort(0,i, sort);
        }

    }

    void MergeSortC(std::vector<Visualizer::IntTracked>& vec,Visualizer::tPred pred)
    {
        auto merge = [&](int left, int right, auto&& merge)->void {

            if (right - left <= 1)
                return;
            int mid = left + (right - left) / 2;



            merge(left, mid, merge);
            merge(mid, right, merge);

            std::vector<int> L(vec.begin() + left, vec.begin() + mid);
            std::vector<int> R(vec.begin() + mid, vec.begin() + right);

            int i = 0, j = 0, k = left;

            while (i < L.size() && j < R.size())
            {
                if (!pred(R[j],L[i]))
                {
                    vec[k] = L[i];
                    i++;
                }
                else
                {
                    vec[k] = R[j];
                    j++;
                }
                k++;
            }

            while (i < L.size())
            {
                vec[k] = L[i];
                i++;
                k++;
            }
            while (j < R.size())
            {
                vec[k] = R[j];
                j++;
                k++;
            }

        };
        merge(0, vec.size(), merge);
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
        std::cout << "Hello to visualizer 0.5v !!!\n";
        std::cout << "Choose 1 of 11 sorting algorithms!\n1. Bubble sort\n 2. Shaker sort \n 3. Gnome sort\n 4. Selection sort\n 5. Insertion sort\n 6. Radix sort\n 7. std::sort\n 8. std::stable_sort\n 9. Quicksort Hoare variation\n 10. Quicksort Lamuto variation\n 11. Quicksort Sedgewick variation\n 12. Quicksort my variation\n 13. Heap sort\n 14. Binary Merge sort\n";
        bool wrong = true;

        int result;

        do
        {
            result = GetInput<int>();
        } while (result >= 15 || result <= 0);

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
        } while (Amount < 11 || Amount > 800);

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
            break;
        case 9:
            Visualizer::Start(Test::QSort_HuareC);
            break;
        case 10:
            Visualizer::Start(Test::QSort_LomutoC);
            break;
        case 11:
            Visualizer::Start(Test::QSort_SedgewickC);
            break;
        case 12:
            Visualizer::Start(Test::QSortC);
            break;
        case 13:
            Visualizer::Start(Test::HeapSortC);
            break;
        case 14:
            Visualizer::Start(Test::MergeSortC);
        }

        auto stats = Visualizer::GetStats();
        std::cout << "\n Array was sorted!!!\n Stats\n Number of comparisons is " << stats.first << "\n Number of Array changes is " << stats.second << "\n Press Enter To Continue...";
        getchar();

        system("cls");
    }

    return 0;
}