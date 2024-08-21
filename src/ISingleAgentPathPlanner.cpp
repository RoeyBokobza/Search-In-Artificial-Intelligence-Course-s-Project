#include "ISingleAgentPathPlanner.h"

Path ISingleAgentPathPlanner::ReconstructPath(const Vertex* goal) const
{
    Path path;
    const Vertex* curr = goal;

    while(curr)
    {
        path.emplace_back(curr->c);
        curr = curr->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

float ISingleAgentPathPlanner::gCost(const Vertex* parent, const Vertex* successor, const Graph& g) const
{
    return parent->g + g.WeightOf({parent->c, successor->c});
}