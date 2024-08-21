#include "IBDA*2.h"
#include "Timer.h"
#include "Vertex.h"

struct Plan IBDAstar2::Plan(const Graph& g, const Agent& a, const float runtime, const HeuristicFunction& h)
{
    Timer timer;
    Vertex* current;
    std::deque<Vertex*> dq;
    
    plan.Clear();

    timer.Start(runtime);
    
    auto root = Init(a, h);
    const auto max_simple_path_length = g.GetEdges().size();
    int min_bound = root->h;
    int max_bound = max_simple_path_length;
    dq.push_back(root);

    while (min_bound < max_bound && !timer.ExceedsRuntime())
    {
        int bound = (min_bound + max_bound) >> 1;
        root->g = 0;
        const auto& [is_path_found, path_cost] = BoundedDFS(dq, g, a, h, min_bound, max_bound, timer);

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

std::tuple<bool, float> IBDAstar2::BoundedDFS(std::deque<Vertex*>& dq, const Graph& g, const Agent& a, const HeuristicFunction& h, float min_bound, float max_bound, const Timer& timer)
{
    int number_of_vertices_exceeds_max_bound = 0;
    auto min_rejected_f_val = INF;

    while(dq.size() > number_of_vertices_exceeds_max_bound && !timer.ExceedsRuntime())
    {
        auto* current = dq.front();
        dq.pop_front();

        if(current->fVal() >= min_bound && current->fVal() <= max_bound)
        {
            if(current->c == a.goal)
            {
                return {true, current->fVal()};
            }

            for(auto* successor: Expand(current, g, a, h))
            {        
                if(gCost(current, successor, g) < successor->g)
                {
                    successor->g = gCost(current, successor, g);
                    successor->parent = current;

                    dq.push_back(successor);
                }
            }   
        }
        else if(current->fVal() > max_bound) // current might be valid in next iterations if max_bound increased.
        {
            dq.push_front(current);
            number_of_vertices_exceeds_max_bound += 1;

            min_rejected_f_val = std::min(min_rejected_f_val, current->fVal());
        }
        // else current f value is lower then min_bound, in previous iterations we found that no such path is exists with such a cost.
    }

    return {false, min_rejected_f_val};
}