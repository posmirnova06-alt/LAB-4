#pragma once
#include <stdexcept>

template <class T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;

    virtual int GetLength() const = 0;
};