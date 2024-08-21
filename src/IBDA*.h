#pragma once

#include "IDA*.h"
#include "Printer.h"
#include "Types.h"

class IBDAstar: public IDAstar
{
public:
    IBDAstar(): IDAstar() {}
    virtual ~IBDAstar() override {Clear();};

    struct Plan Plan(const Graph& g, const Agent& a, float runtime, const HeuristicFunction& h = Heuristic::ManhattanDistance) override;
    std::string GetName(void) const override {return "IBDA*";}
    OutputColor GetColor(void) const override {return Blue;}
};