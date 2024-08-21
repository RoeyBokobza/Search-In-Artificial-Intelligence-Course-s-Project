#pragma once

#include <boost/heap/fibonacci_heap.hpp>
#include "Graph.h"
#include "Agent.h"
#include "Types.h"
#include "Utils.h"
#include "Plan.h"
#include "ISingleAgentPathPlanner.h"
#include "Vertex.h"

class Astar: public ISingleAgentPathPlanner
{
public:
    Astar(): ISingleAgentPathPlanner() {}
    virtual ~Astar() override {Clear();}

    struct Plan Plan(const Graph& g, const Agent& a, float runtime = 30.000, const HeuristicFunction& h = Heuristic::ManhattanDistance) override;
    std::string GetName(void) const override {return "A*";}
    OutputColor GetColor(void) const override {return Green;}

protected:
    using MinFibHeap = boost::heap::fibonacci_heap<Vertex*, boost::heap::compare<Vertex::Comparator>>;

    LookupTable table;
    struct Plan plan{};

    Vertex* Init(const Agent& a, const HeuristicFunction& h);
    Vertex* Generate(Vertex* parent, const Coordinate& successor_coordinate, const Agent& a, const HeuristicFunction& h);
    Successors Expand(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h);
    void Clear(void);
    bool IsGenerated(const Coordinate& c) const;
};