#include "IBDA*.h"
#include "Timer.h"

struct Plan IBDAstar::Plan(const Graph& g, const Agent& a, const float runtime, const HeuristicFunction& h)
{
    Timer timer;
    Vertex* current;
    
    plan.Clear();

    timer.Start(runtime);
    
    auto root = Init(a, h);
    const auto max_simple_path_length = g.GetEdges().size();
    int min_bound = root->h;
    int max_bound = max_simple_path_length;

    while (min_bound < max_bound && !timer.ExceedsRuntime())
    {
        int bound = (min_bound + max_bound) >> 1;
        root->g = 0;
        const auto& [is_path_found, path_cost] = Search(root, g, a, h, bound, timer);

        if(is_path_found)
        {
            max_bound = path_cost;
        }
        else
        {
            min_bound = bound + 1;
        }

        gCostRestart();
    }

    plan.runtime = timer.Stop();
    
    plan.path = (min_bound == max_bound && min_bound < max_simple_path_length) ? ReconstructPath(table.at(a.goal)) : Path{};

    Clear();

    return plan;
}