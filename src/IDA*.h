#pragma once

#include "Coordinate.h"
#include "ISingleAgentPathPlanner.h"
#include "Printer.h"
struct Timer;

class IDAstar: public ISingleAgentPathPlanner
{
public:
    IDAstar(): ISingleAgentPathPlanner() {}
    virtual ~IDAstar() override {Clear();};

    struct Plan Plan(const Graph& g, const Agent& a, float runtime, const HeuristicFunction& h = Heuristic::ManhattanDistance) override;
    virtual std::string GetName(void) const override {return "IDA*";}
    virtual OutputColor GetColor(void) const override {return Yellow;}

protected:
    struct Plan plan{};
    LookupTable table;

    Vertex* Init(const Agent& a, const HeuristicFunction& h);
    Vertex* Generate(const Coordinate& successor_coordinate, const Graph& g, const Agent& a, const HeuristicFunction& h);
    Successors Expand(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h);
    std::tuple<bool, float> Search(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h, float bound, const Timer& timer);
    void Clear(void);
    bool IsGenerated(const Coordinate& successor_coordinate) const;
    void gCostRestart(void);
};