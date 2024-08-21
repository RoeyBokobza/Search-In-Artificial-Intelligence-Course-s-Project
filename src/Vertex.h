#pragma once

#include "Coordinate.h"
#include "Constants.h"
#include <boost/heap/fibonacci_heap.hpp>

struct Vertex
{
    struct Comparator{ bool operator() (const Vertex* v1, const Vertex* v2) const noexcept;};
    struct Hasher{ size_t operator()(const Vertex* v) const noexcept; };
    struct Equal{ bool operator()(const Vertex* v1, const Vertex* v2) const noexcept; };

    using MinFibHeap = boost::heap::fibonacci_heap<Vertex*, boost::heap::compare<Comparator>>;

    const Vertex* parent = nullptr;
    Coordinate c{};
    float g = INF;
    float h = 0;
    bool in_open = false;
    bool is_expanded = false;
    MinFibHeap::handle_type handler;
    
    bool operator < (const Vertex& v) const noexcept;
    bool operator == (const Vertex& v) const noexcept;
    inline float fVal(void) const noexcept {return g + h;}
};