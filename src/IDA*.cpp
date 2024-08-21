#include "Constants.h"
#include "Coordinate.h"
#include "Graph.h"
#include "IDA*.h"
#include "ISingleAgentPathPlanner.h"
#include "Timer.h"
#include "Types.h"
#include "Vertex.h"

struct Plan IDAstar::Plan(const Graph& g, const Agent& a, const float runtime, const HeuristicFunction& h)
{
    Timer timer;
    Vertex* current;
    
    plan.Clear();

    timer.Start(runtime);

    bool is_path_found = false;
    float path_cost = INF;
    auto root = Init(a, h);
    auto bound = root->h;

    while (bound < INF && !is_path_found && !timer.ExceedsRuntime())
    {
        root->g = 0;
        std::tie(is_path_found, path_cost) = Search(root, g, a, h, bound, timer);
        bound = path_cost;
        gCostRestart();
    }

    plan.runtime = timer.Stop();
    
    plan.path = is_path_found ? ReconstructPath(table.at(a.goal)) : Path{};

    Clear();

    return plan;
}

std::tuple<bool, float> IDAstar::Search(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h, const float bound, const Timer& timer)
{
    if(current->fVal() > bound)
    {
        return {false, current->fVal()};
    }

    if(current->c == a.goal)
    {
        return {true, current->fVal()};
    }

    if(timer.ExceedsRuntime())
    {
        return {false, INF};
    }

    auto min = INF;

    for(auto* successor: Expand(current, g, a, h))
    {        
        if(gCost(current, successor, g) < successor->g)
        {
            successor->g = gCost(current, successor, g);
            successor->parent = current;

            const auto& [is_path_found, path_cost] = Search(successor, g, a, h, bound, timer);

            if(is_path_found)
            {
                return {is_path_found, path_cost};
            }

            min = std::min(min, path_cost);
        }
    }

    return {false, min};
}

ISingleAgentPathPlanner::Successors IDAstar::Expand(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h)
{
    Successors successors;

    for(const auto& successor_coordinate: g.SuccessorsOf(current->c))
    {
        successors.push_back(Generate(successor_coordinate, g, a, h));
    }

    std::sort(successors.begin(), successors.end(), [](const Vertex* v1, const Vertex* v2) {return *v1 < *v2;});

    if(current->is_expanded)
    {
        plan.number_of_reopened_nodes += 1;
    }
    else
    {
        current->is_expanded = true;
    }

    plan.number_of_expanded_nodes += 1;
    
    return successors;
}

bool IDAstar::IsGenerated(const Coordinate& successor_coordinate) const
{
    return table.find(successor_coordinate) != table.end();
}

Vertex* IDAstar::Generate(const Coordinate& successor_coordinate, const Graph& g, const Agent& a, const HeuristicFunction& h)
{
    Vertex* successor;
    
    if(IsGenerated(successor_coordinate))
    {
        successor = table.at(successor_coordinate);
    }
    else
    {
        successor = new Vertex();
        successor->c = successor_coordinate;
        successor->h = h(successor_coordinate, a.goal);
        
        table[successor_coordinate] = successor;

        plan.number_of_generated_nodes += 1;
    }

    return successor;
}

Vertex* IDAstar::Init(const Agent& a, const HeuristicFunction& h)
{
    Vertex* start = new Vertex();

    start->c = a.start;
    start->g = 0;
    start->h = h(a.start, a.goal);

    plan.number_of_generated_nodes += 1;

    return start;
}

void IDAstar::Clear(void)
{
    std::for_each(table.begin(), table.end(), [](const auto& p) { delete p.second; });
    table.clear();
}

void IDAstar::gCostRestart(void)
{
    for(auto& [coordinate, vertex]: table)
    {
        vertex->g = INF;
    }
}