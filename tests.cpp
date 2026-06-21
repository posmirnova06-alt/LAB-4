#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "LazySequence.h"
#include "ReadOnlyStream.h"
#include "WriteOnlyStream.h"
#include "MinHeap.h"
#include "StatCollector.h"
#include "Generator.h"

#define ASSERT_THROWS(expr) \
{ \
    bool threw = false; \
    try { expr; } \
    catch (...) { threw = true; } \
    assert(threw); \
}

int doubleFunc(const int& x) { return x * 2; }
int tripleFunc(const int& x) { return x * 3; }
bool isEvenRef(const int& x) { return x % 2 == 0; }
bool isBigRef(const int& x) { return x > 3; }
int addFunc(const int& a, const int& b) { return a + b; }
int multiplyFunc(const int& a, const int& b) { return a * b; }

int fibRule(const MutableArraySequence<int>& cache)
{
    int n = cache.GetLength();
    return cache.Get(n - 1) + cache.Get(n - 2);
}

int lcgRule(const MutableArraySequence<int>& cache)
{
    int prev = cache.GetLast();
    return (prev * 1103515245 + 12345) % 1000;
}

bool isEven(int x) { return x % 2 == 0; }
int doubleIt(int x) { return x * 2; }
int sumReduce(int acc, int x) { return acc + x; }

void TestDynamicArray()
{
    DynamicArray<int> a(3);
    a.Set(0, 10); a.Set(1, 20); a.Set(2, 30);
    assert(a.Get(0) == 10);
    assert(a.Get(2) == 30);
    assert(a.GetSize() == 3);

    a.Resize(5);
    assert(a.GetSize() == 5);
    assert(a.Get(0) == 10);

    DynamicArray<int> b(0);
    b.Append(1); b.Append(2); b.Append(3);
    assert(b.GetSize() == 3);
    assert(b.Get(2) == 3);

    b.Prepend(0);
    assert(b.Get(0) == 0);

    DynamicArray<int> c(b);
    c.Set(0, 999);
    assert(b.Get(0) != 999);

    ASSERT_THROWS(DynamicArray<int>(-1));
    ASSERT_THROWS(a.Get(-1));
    ASSERT_THROWS(a.Get(100));
    ASSERT_THROWS(a.Resize(-1));
}

void TestLinkedList()
{
    LinkedList<int> list;
    list.Append(1); list.Append(2); list.Append(3);
    assert(list.GetFirst() == 1);
    assert(list.GetLast() == 3);
    assert(list.GetLength() == 3);

    list.Prepend(0);
    assert(list.GetFirst() == 0);

    LinkedList<int> copy(list);
    copy.Set(0, 999);
    assert(list.Get(0) != 999);

    LinkedList<int> empty;
    ASSERT_THROWS(empty.GetFirst());
    ASSERT_THROWS(empty.GetLast());
    ASSERT_THROWS(list.Get(-1));
    ASSERT_THROWS(list.Get(100));
}

void TestMutableArraySequence()
{
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    assert(seq.GetFirst() == 1);
    assert(seq.GetLast() == 3);
    assert(seq.GetLength() == 3);

    MutableSequence<int>* doubled = seq.Map(doubleFunc);
    assert(doubled->Get(0) == 2);
    assert(seq.Get(0) == 1);
    delete doubled;

    MutableSequence<int>* evens = seq.Where(isEvenRef);
    for (int i = 0; i < evens->GetLength(); i++)
        assert(evens->Get(i) % 2 == 0);
    delete evens;

    int sum = seq.Reduce(addFunc, 0);
    assert(sum == 6);

    MutableArraySequence<int> empty;
    ASSERT_THROWS(empty.GetFirst());
    ASSERT_THROWS(empty.GetLast());
}

void TestMutableListSequence()
{
    MutableListSequence<int> seq;
    seq.Append(2); seq.Append(4); seq.Append(6);

    MutableSequence<int>* tripled = seq.Map(tripleFunc);
    assert(tripled->Get(0) == 6);
    assert(seq.Get(0) == 2);
    delete tripled;

    int product = seq.Reduce(multiplyFunc, 1);
    assert(product == 48);

    MutableListSequence<int> empty;
    ASSERT_THROWS(empty.GetFirst());
    ASSERT_THROWS(empty.GetLast());
}

void TestOrdinalIndex()
{
    OrdinalIndex five(5);
    OrdinalIndex ten(10);
    OrdinalIndex omega = OMEGA;
    OrdinalIndex omegaPlusThree = OMEGA + 3;

    assert(five < ten);
    assert(five < omega);
    assert(ten < omega);
    assert(omega < omegaPlusThree);

    assert(omegaPlusThree.omega == 1);
    assert(omegaPlusThree.finite == 3);

    OrdinalIndex diff = omegaPlusThree - omega;
    assert(diff.omega == 0);
    assert(diff.finite == 3);

    assert(five.isFinite());
    assert(!omega.isFinite());
    assert(!omegaPlusThree.isFinite());
}

void TestLazySequence()
{
    int seed[] = {1, 1};
    LazySequence<int> fib(fibRule, seed, 2);
    assert(fib.Get(0) == 1);
    assert(fib.Get(1) == 1);
    assert(fib.Get(5) == 8);
    assert(fib.Get(9) == 55);
    assert(fib.IsInfinite());

    fib.Get(4);
    int countAfter = fib.GetMaterializedCount();
    fib.Get(4);
    assert(fib.GetMaterializedCount() == countAfter);

    int arr[] = {10, 20, 30};
    LazySequence<int> finite(arr, 3);
    assert(finite.GetFirst() == 10);
    assert(finite.GetLast() == 30);
    assert(!finite.IsInfinite());

    finite.Append(40);
    assert(finite.Get(3) == 40);

    finite.Prepend(0);
    assert(finite.Get(0) == 0);

    LazySequence<int> copy(finite);
    assert(copy.GetFirst() == finite.GetFirst());

    LazySequence<int>* sub = finite.GetSubsequence(1, 3);
    assert(sub->GetMaterializedCount() == 3);
    assert(sub->Get(0) == 10);
    delete sub;

    ASSERT_THROWS(finite.GetSubsequence(-1, 2));
    ASSERT_THROWS(finite.GetSubsequence(2, 0));
    ASSERT_THROWS(fib.Get(-1));
    ASSERT_THROWS(fib.GetLast());
    ASSERT_THROWS(fib.Append(1));
    ASSERT_THROWS(fib.Prepend(1));
}

void TestLazySequenceLazy()
{
    int seed[] = {1, 1};
    LazySequence<int> fib(fibRule, seed, 2);

    LazySequence<int>* doubled = fib.Map(doubleIt);
    assert(doubled->GetMaterializedCount() == 0);
    assert(doubled->Get(4) == 10);
    assert(doubled->GetMaterializedCount() == 5);
    delete doubled;

    LazySequence<int>* evens = fib.Where(isEven);
    assert(evens->GetMaterializedCount() == 0);
    assert(evens->Get(0) == 2);
    assert(evens->Get(1) == 8);
    delete evens;

    int arr[] = {10, 20, 30};
    LazySequence<int> finite(arr, 3);
    LazySequence<int>* cat = finite.Concat(&fib);
    assert(cat->GetMaterializedCount() == 0);
    assert(cat->Get(0) == 10);
    assert(cat->Get(2) == 30);
    assert(cat->Get(3) == 1);
    assert(cat->Get(4) == 1);
    delete cat;

    LazySequence<Pair<int,int>>* zipped = finite.Zip(&finite);
    assert(zipped->GetMaterializedCount() == 0);
    Pair<int,int> p = zipped->Get(0);
    assert(p.first == 10 && p.second == 10);
    delete zipped;
}

void TestLazySequenceConcatTwoInfinite()
{
    int seed1[] = {1, 1};
    LazySequence<int> fib(fibRule, seed1, 2);

    int seed2[] = {42};
    LazySequence<int> lcg(lcgRule, seed2, 1);

    LazySequence<int>* combined = fib.Concat(&lcg);
    assert(combined->IsInfinite());

    assert(combined->Get(0) == 1);
    assert(combined->Get(1) == 1);
    assert(combined->Get(5) == 8);

    int fromSecond0 = combined->Get(OMEGA + 0);
    assert(fromSecond0 == 42);

    int fromSecond1 = combined->Get(OMEGA + 1);
    int fromSecond2 = combined->Get(OMEGA + 2);
    assert(fromSecond1 != fromSecond0);
    assert(fromSecond2 != fromSecond1);

    delete combined;
}

void TestGenerator()
{
    FunctionGenerator<int> gen(fibRule);
    assert(gen.HasNext() == true);

    int seed[] = {1, 1};
    MutableArraySequence<int> cache(seed, 2);

    int outValue = 0;
    bool success = gen.TryGetNext(cache, outValue);
    assert(success == true);
    assert(outValue == 2);
}

void TestReadOnlyStream()
{
    int seed[] = {1, 1};
    LazySequence<int> fib(fibRule, seed, 2);
    ReadOnlyStream<int> stream(&fib);
    stream.Open();

    assert(stream.Read() == 1);
    assert(stream.Read() == 1);
    assert(stream.Read() == 2);
    assert(stream.GetPosition() == 3);
    assert(!stream.IsEndOfStream());
    assert(stream.IsCanSeek());

    stream.Seek(0);
    assert(stream.Read() == 1);

    int arr[] = {10, 20};
    LazySequence<int> finite(arr, 2);
    ReadOnlyStream<int> finiteStream(&finite);
    finiteStream.Open();
    finiteStream.Read();
    finiteStream.Read();
    assert(finiteStream.IsEndOfStream());

    ReadOnlyStream<int> closedStream(&finite);
    ASSERT_THROWS(closedStream.Read());
    ASSERT_THROWS(finiteStream.Read());

    finiteStream.Close();
    ASSERT_THROWS(finiteStream.Read());
}

void TestWriteOnlyStream()
{
    MutableArraySequence<int> dest;
    WriteOnlyStream<int> stream(&dest);
    stream.Open();

    size_t pos1 = stream.Write(10);
    size_t pos2 = stream.Write(20);
    stream.Write(30);

    assert(pos1 == 1);
    assert(pos2 == 2);
    assert(dest.GetLength() == 3);
    assert(dest.Get(0) == 10 && dest.Get(2) == 30);
    assert(stream.GetPosition() == 3);

    stream.Close();
    ASSERT_THROWS(stream.Write(40));

    WriteOnlyStream<int> closedStream(&dest);
    ASSERT_THROWS(closedStream.Write(1));
}

void TestMinHeap()
{
    MinHeap<int> heap;
    assert(heap.IsEmpty());

    int values[] = {5, 1, 9, 3, 7};
    for (int v : values)
        heap.Insert(v);

    assert(heap.GetSize() == 5);
    assert(heap.Top() == 1);
    assert(heap.ExtractMin() == 1);
    assert(heap.ExtractMin() == 3);
    assert(heap.ExtractMin() == 5);

    MinHeap<int> single;
    single.Insert(42);
    assert(single.Top() == 42);
    assert(single.ExtractMin() == 42);
    assert(single.IsEmpty());

    MinHeap<int> empty;
    ASSERT_THROWS(empty.Top());
    ASSERT_THROWS(empty.ExtractMin());
}

void TestStatCollector()
{
    StatCollector<int> stats;
    stats.AddValue(5);
    stats.AddValue(2);
    stats.AddValue(8);
    stats.AddValue(1);

    assert(stats.GetCount() == 4);
    assert(stats.GetMin() == 1);
    assert(stats.GetMax() == 8);
    assert(stats.GetMean() == 4.0);
    assert(stats.GetMedian() == 3.5);

    StatCollector<int> oddCount;
    oddCount.AddValue(1);
    oddCount.AddValue(2);
    oddCount.AddValue(5);
    assert(oddCount.GetMedian() == 2.0);

    StatCollector<int> single;
    single.AddValue(42);
    assert(single.GetMin() == 42);
    assert(single.GetMax() == 42);
    assert(single.GetMean() == 42.0);
    assert(single.GetMedian() == 42.0);

    StatCollector<int> empty;
    ASSERT_THROWS(empty.GetMean());
    ASSERT_THROWS(empty.GetMin());
    ASSERT_THROWS(empty.GetMedian());
}

double NaiveMedian(int* values, int count)
{
    int* sorted = new int[count];
    for (int i = 0; i < count; i++)
        sorted[i] = values[i];

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (sorted[j] > sorted[j + 1])
            {
                int temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    double result;
    if (count % 2 == 0)
    {
        result = (sorted[count / 2 - 1] + sorted[count / 2]) / 2.0;
    }
    else
    {
        result = sorted[count / 2];
    }

    delete[] sorted;
    return result;
}

void TestStatCollectorAgainstNaive()
{
    srand(12345);

    for (int trial = 0; trial < 20; trial++)
    {
        int count = 1 + rand() % 50;
        int* values = new int[count];

        StatCollector<int> stats;
        for (int i = 0; i < count; i++)
        {
            values[i] = rand() % 1000;
            stats.AddValue(values[i]);
        }

        double naive = NaiveMedian(values, count);
        assert(std::abs(naive - stats.GetMedian()) < 1e-9);

        delete[] values;
    }
}

void TestPerformance()
{
    int seed[] = {42};
    LazySequence<int> seq(lcgRule, seed, 1);

    int last = seq.Get(999999);
    assert(seq.GetMaterializedCount() == 1000000);
    assert(seq.Get(999999) == last);

    ReadOnlyStream<int> stream(&seq);
    stream.Open();

    StatCollector<int> stats;
    for (int i = 0; i < 1000000; i++)
        stats.AddValue(stream.Read());

    assert(stats.GetCount() == 1000000);
    stats.GetMean();
    stats.GetMedian();
}

int main()
{
    TestDynamicArray();
    std::cout << "DynamicArray: OK\n";
    TestLinkedList();
    std::cout << "LinkedList: OK\n";
    TestMutableArraySequence();
    std::cout << "MutableArraySequence: OK\n";
    TestMutableListSequence();
    std::cout << "MutableListSequence: OK\n";
    TestOrdinalIndex();
    std::cout << "OrdinalIndex: OK\n";
    TestLazySequence();
    std::cout << "LazySequence: OK\n";
    TestLazySequenceLazy();
    std::cout << "LazySequence (laziness): OK\n";
    TestLazySequenceConcatTwoInfinite();
    std::cout << "LazySequence (concat two infinite + OMEGA): OK\n";
    TestGenerator();
    std::cout << "Generator: OK\n";
    TestReadOnlyStream();
    std::cout << "ReadOnlyStream: OK\n";
    TestWriteOnlyStream();
    std::cout << "WriteOnlyStream: OK\n";
    TestMinHeap();
    std::cout << "MinHeap: OK\n";
    TestStatCollector();
    std::cout << "StatCollector: OK\n";
    TestStatCollectorAgainstNaive();
    std::cout << "StatCollector vs naive median (randomized): OK\n";
    TestPerformance();
    std::cout << "Performance (1M elements): OK\n";
    std::cout << "\nAll tests passed!\n";
    return 0;
}