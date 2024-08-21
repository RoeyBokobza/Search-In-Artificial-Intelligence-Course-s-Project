#include "A*.h"
#include "Timer.h"

struct Plan Astar::Plan(const Graph& g, const Agent& a, const float runtime, const HeuristicFunction& h)
{
    Timer timer;
    MinFibHeap open;
    Vertex* current;
    
    plan.Clear();

    timer.Start(runtime);

    auto root = Init(a, h);
    root->handler = open.push(root);
    bool is_goal_found = false;

    while(!open.empty() && !is_goal_found && !timer.ExceedsRuntime())
    {
        current = open.top();
        open.pop();
        plan.number_of_expanded_nodes += 1;
        current->in_open = false;

        is_goal_found = (current->c == a.goal);

        if(!is_goal_found)
        {
            for(Vertex* successor: Expand(current, g, a, h))
            {
                if(gCost(current, successor, g) < successor->g)
                {
                    successor->g = gCost(current, successor, g);
                    successor->parent = current;

                    if(successor->in_open)
                    {
                        open.decrease(successor->handler, successor);
                    } 
                    else
                    {
                        successor->handler = open.push(successor);
                        successor->in_open = true;
                    }
                }
            }
        }
    }

    plan.runtime = timer.Stop();

    plan.path = is_goal_found ? ReconstructPath(current) : Path{};

    Clear();

    return plan;
}

Astar::Successors Astar::Expand(Vertex* current, const Graph& g, const Agent& a, const HeuristicFunction& h)
{
    Successors successors;

    for(const auto& successor_coordinate: g.SuccessorsOf(current->c))
    {
        successors.push_back(Generate(current, successor_coordinate, a, h));
    }
    
    return successors;
}

bool Astar::IsGenerated(const Coordinate& c) const
{
    return table.find(c) != table.end();
}

Vertex* Astar::Generate(Vertex* parent, const Coordinate& successor_coordinate, const Agent& a, const HeuristicFunction& h)
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

Vertex* Astar::Init(const Agent& a, const HeuristicFunction& h)
{
    Vertex* start = new Vertex();
    start->parent = nullptr;
    start->c = a.start;
    start->g = 0;
    start->h = h(a.start, a.goal);
    table[start->c] = start;
    plan.number_of_generated_nodes += 1;
    return start;
}

void Astar::Clear(void)
{
    for(auto& [coordinate, vertex]: table)
    {
        delete(vertex);
    }
    table.clear();
}