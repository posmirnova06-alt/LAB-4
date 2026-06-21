#include <iostream>
#include "LazySequence.h"
#include "ReadOnlyStream.h"
#include "WriteOnlyStream.h"
#include "StatCollector.h"

int fibRule(const MutableArraySequence<int>& cache)
{
    int n = cache.GetLength();
    return cache.Get(n - 1) + cache.Get(n - 2);
}

int doubleIt(int x) { return x * 2; }
bool isEven(int x) { return x % 2 == 0; }

void printMenu()
{
    std::cout << "\n Лабораторная работа 4 \n";
    std::cout << "1. Ручной ввод чисел (StatCollector)\n";
    std::cout << "2. Операции над бесконечной последовательностью\n";
    std::cout << "3. Concat двух бесконечных последовательностей\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите режим: ";
}

void runManualMode()
{
    std::cout << "\nВведите числа по одному (stop для завершения):\n";

    StatCollector<int> stats;

    while (true)
    {
        std::cout << "> ";
        std::string input;
        std::cin >> input;

        if (input == "stop")
        {
            break;
        }

        try
        {
            int value = std::stoi(input);
            stats.AddValue(value);

            std::cout << "count=" << stats.GetCount();
            std::cout << " min=" << stats.GetMin();
            std::cout << " max=" << stats.GetMax();
            std::cout << " mean=" << stats.GetMean();
            std::cout << " median=" << stats.GetMedian() << "\n";
        }
        catch (const std::exception&)
        {
            std::cout << "Ошибка: введите целое число или stop\n";
        }
    }

    if (stats.GetCount() == 0)
    {
        std::cout << "Данных не было введено.\n";
    }
}

void runLazyOpsMode()
{
    std::cout << "\nСколько элементов показать? ";
    int n;
    std::cin >> n;

    if (n <= 0)
    {
        std::cout << "Количество должно быть положительным.\n";
        return;
    }

    int seed[] = {1, 1};
    LazySequence<int> fib(fibRule, seed, 2);

    std::cout << "\nФибоначчи:\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << fib.Get(i) << " ";
    }
    std::cout << "\n";

    LazySequence<int>* doubled = fib.Map(doubleIt);
    std::cout << "\nMap (x2):\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << doubled->Get(i) << " ";
    }
    std::cout << "\n";

    MutableArraySequence<int> dest;
    WriteOnlyStream<int> writeStream(&dest);
    writeStream.Open();
    for (int i = 0; i < n; i++)
    {
        writeStream.Write(doubled->Get(i));
    }
    writeStream.Close();
    std::cout << "(записано через WriteOnlyStream: " << writeStream.GetPosition() << " элементов)\n";
    delete doubled;

    LazySequence<int>* evens = fib.Where(isEven);
    std::cout << "\nWhere (чётные):\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << evens->Get(i) << " ";
    }
    std::cout << "\n";
    delete evens;

    int seed2[] = {1, 2};
    LazySequence<int> fib2(fibRule, seed2, 2);

    LazySequence<Pair<int, int>>* zipped = fib.Zip(&fib2);
    std::cout << "\nZip:\n";
    for (int i = 0; i < n; i++)
    {
        Pair<int, int> p = zipped->Get(i);
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << "\n";
    delete zipped;

    std::cout << "\nМатериализовано элементов fib: " << fib.GetMaterializedCount() << "\n";
}

void runConcatInfiniteMode()
{
    std::cout << "\nConcat двух бесконечных последовательностей\n";
    std::cout << "Первая: Фибоначчи [1,1,...], вторая: Фибоначчи [2,3,...]\n\n";

    int seed1[] = {1, 1};
    LazySequence<int> fib1(fibRule, seed1, 2);

    int seed2[] = {2, 3};
    LazySequence<int> fib2(fibRule, seed2, 2);

    LazySequence<int>* combined = fib1.Concat(&fib2);

    std::cout << "Сколько элементов из первой? ";
    int n;
    std::cin >> n;

    if (n <= 0)
    {
        std::cout << "Количество должно быть положительным.\n";
        delete combined;
        return;
    }

    std::cout << "\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << "Get(" << i << ") = " << combined->Get(i) << "\n";
    }

    std::cout << "\nСколько элементов из второй? ";
    int m;
    std::cin >> m;

    if (m <= 0)
    {
        std::cout << "Количество должно быть положительным.\n";
        delete combined;
        return;
    }

    std::cout << "\n";
    for (int i = 0; i < m; i++)
    {
        std::cout << "Get(OMEGA+" << i << ") = ";
        std::cout << combined->Get(OMEGA + i) << "\n";
    }

    delete combined;
}

int main()
{
    while (true)
    {
        printMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            runManualMode();
        }
        else if (choice == 2)
        {
            runLazyOpsMode();
        }
        else if (choice == 3)
        {
            runConcatInfiniteMode();
        }
        else
        {
            std::cout << "Неверный выбор.\n";
        }
    }

    std::cout << "Выход.\n";
    return 0;
}