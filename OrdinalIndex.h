#pragma once

struct OrdinalIndex
{
    int omega;
    int finite;

    OrdinalIndex(int f) : omega(0), finite(f) {}
    OrdinalIndex(int w, int f) : omega(w), finite(f) {}

    bool operator<(const OrdinalIndex& other) const
    {
        if (omega != other.omega)
        {
            return omega < other.omega;
        }

        return finite < other.finite;
    }

    bool operator<=(const OrdinalIndex& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const OrdinalIndex& other) const
    {
        return !(*this < other);
    }

    bool operator==(const OrdinalIndex& other) const
    {
        return omega == other.omega && finite == other.finite;
    }

    OrdinalIndex operator+(int n) const
    {
        return OrdinalIndex(omega, finite + n);
    }

    OrdinalIndex operator-(const OrdinalIndex& other) const
    {
        return OrdinalIndex(omega - other.omega, finite - other.finite);
    }

    bool isFinite() const
    {
        return omega == 0;
    }
};

const OrdinalIndex OMEGA(1, 0);