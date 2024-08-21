#include "Utils.h"
#include "Types.h"
#include <algorithm>
#include <cstdlib>
#include "Map.h"
#include "A*.h"

CoordinateSet Neighborhood::FourPrincipleDirection(const Coordinate& c, size_t R)
{
    CoordinateSet s;

    for(int r = 1; r <= R; r++)
    {
        s.emplace(c.row - r, c.column); // up
        s.emplace(c.row + r, c.column); // down
        s.emplace(c.row, c.column - r); // left
        s.emplace(c.row, c.column + r); // right
        s.emplace(c.row, c.column); // wait
    }
    
    return s;
}

CoordinateSet Neighborhood::EightPrincipleDirection(const Coordinate& c, size_t R)
{
    CoordinateSet s = FourPrincipleDirection(c, R);

    for(int r = 1; r <= R; r++)
    {
        s.emplace(c.row - r, c.column - r); // up-left
        s.emplace(c.row - r, c.column + r); // up-right
        s.emplace(c.row + r, c.column - r); // down-left
        s.emplace(c.row + r, c.column + r); // down-right
    }
    
    return s;
}

float Heuristic::ManhattanDistance(const Coordinate& c1, const Coordinate& c2)
{
    return abs(c1.row - c2.row) + abs(c1.column - c2.column);
}

float Heuristic::ChebyshevDistance(const Coordinate& c1, const Coordinate& c2)
{
    return std::max(abs(c1.row - c2.row), abs(c1.column - c2.column));
}

long ObjectiveFunction::PathLength(const Path& p)
{
    return p.empty() ? 0 : (long)p.size() - 1;
}

long ObjectiveFunction::SumOfCost(const Paths& ps)
{
    long soc = 0;

    for(const auto& p: ps)
    {
        soc += PathLength(p);
    }
    
    return soc;
}

long ObjectiveFunction::Makespan(const Paths& ps)
{
    long longest = -1;

    for(const auto& p: ps)
    {
        longest = std::max(longest, PathLength(p));
    }

    return longest;
}

Agents Validator::ValidAgents(const Agents& src, const Map& m)
{
    Astar astar;
    Agents valid_agents;
    Graph g = m.CreateGraph(false, false);

    for(const auto& a: src)
    {
        if(m.IsValidCoordinate(a.start) && m.IsValidCoordinate(a.goal))
        {
            auto agent_start_terrain = m.GetTerrain(a.start), agent_goal_terrain = m.GetTerrain(a.goal);
            bool is_agent_start_at_legal_coordinates = (agent_start_terrain == Terrain::empty && agent_goal_terrain == Terrain::empty);
            
            if( is_agent_start_at_legal_coordinates && 
                !astar.Plan(g, a).path.empty() && 
                std::none_of(valid_agents.begin(), valid_agents.end(), [&a](const auto& b){return b.start == a.start || b.goal == a.goal;}))
            {
                valid_agents.push_back(a);
            }
        }
    }

    for(int i = 0; i < (int)valid_agents.size(); i++)
    {
        valid_agents[i].index = i;
    }

    return valid_agents;
}