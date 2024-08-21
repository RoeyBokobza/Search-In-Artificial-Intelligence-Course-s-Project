#pragma once

#include "Coordinate.h"
#include "Graph.h"
#include "Agent.h"
#include "Utils.h"
#include "Printer.h"
#include "Plan.h"
#include "Vertex.h"
#include <boost/unordered/unordered_set.hpp>
#include <boost/unordered_map.hpp>

struct Vertex;

class ISingleAgentPathPlanner
{
public:
    ISingleAgentPathPlanner() = default;
    virtual ~ISingleAgentPathPlanner() = default;

    virtual struct Plan Plan(const Graph& g, const Agent& a, float runtime = 30.000, const HeuristicFunction& h = Heuristic::ManhattanDistance) = 0;
    virtual std::string GetName(void) const = 0;
    virtual OutputColor GetColor(void) const = 0;
    
protected:
    using Successors = std::vector<Vertex*>;
    using LookupTable = boost::unordered::unordered_map<Coordinate, Vertex*, Coordinate::Hasher, Coordinate::Equal>;

    Path ReconstructPath(const Vertex* goal) const;
    float gCost(const Vertex* parent, const Vertex* successor, const Graph& g) const;
};