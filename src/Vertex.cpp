#include "Vertex.h"
#include "Coordinate.h"

bool Vertex::Comparator::operator() (const Vertex* v1, const Vertex* v2) const noexcept
{
    assert(v1 && v2);
    return !(*v1 < *v2);
}

size_t Vertex::Hasher::operator()(const Vertex* v) const noexcept
{
    Coordinate::Hasher h;
    return h(v->c);
}

bool Vertex::Equal::operator()(const Vertex* v1, const Vertex* v2) const noexcept
{
    return v1->c == v2->c;
}

bool Vertex::operator < (const Vertex& other) const noexcept
{
    if(fVal() == other.fVal())
    {
        if(g == other.g)
        {
            return  h < other.h; // prefer node with LOWER h cost: closer to destination
        }
        else
        {
            return g > other.g;  // break tie in a depth-first manner: prefer node further from root
        }
    }
    return fVal() < other.fVal();
}

bool Vertex::Vertex::operator == (const Vertex& other) const noexcept
{
    return c == other.c;
}