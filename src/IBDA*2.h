#pragma once

#include "IDA*.h"
#include "Printer.h"
#include "Types.h"
#include <deque>

class IBDAstar2: public IDAstar
{
public:
    IBDAstar2(): IDAstar() {}
    virtual ~IBDAstar2() override {Clear();};

    struct Plan Plan(const Graph& g, const Agent& a, float runtime, const HeuristicFunction& h = Heuristic::ManhattanDistance) override;
    std::string GetName(void) const override {return "IBDA*2";}
    OutputColor GetColor(void) const override {return Cyan;}
protected:
    std::tuple<bool, float> BoundedDFS(std::deque<Vertex*>& dq, const Graph& g, const Agent& a, const HeuristicFunction& h, float min_bound, float max_bound, const Timer& timer);
};