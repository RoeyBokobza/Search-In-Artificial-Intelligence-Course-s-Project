#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include "../src/A*.h"
#include "../src/IDA*.h"
#include "../src/IBDA*.h"
#include "../src/IBDA*2.h"
#include "../src/ISingleAgentPathPlanner.h"
#include "../src/Map.h"
#include "../src/Scenario.h"
#include "../src/Printer.h"

/*
    run A*, IDA* search on a map with diferent start and goal location (defined in scenario file)
*/
int main(int argc, const char** argv)
{
    if(argc != 4)
    {
        Print(Red, "Expected 3 arguments: map_file_path, scenarios_file_path, number_of_runs. Given: ", argc, '\n');
        exit(EXIT_FAILURE);
    }

    Map map(argv[1]);
    const auto graph = map.CreateGraph(true, true);

    Scenario scenario(argv[2]);
    std::vector<ISingleAgentPathPlanner*> planners = {new Astar(), new IDAstar(), new IBDAstar(), new IBDAstar2()};

    const int number_of_runs = atoi(argv[3]);

    for(const auto& agent: scenario.GetAgents())
    {
        int runs = 0;

        for(auto* planner: planners)
        {
            auto plan = planner->Plan(graph, agent);
            
            Print(planner->GetColor(), "Planner: ", planner->GetName(), '\n', agent, '\n');
            Print(Default, plan.ToString(true), '\n');
            std::cout << map.ToString(plan.path) << '\n';
        }

        runs += 1;
        if(runs == number_of_runs)
        {
            break;
        }
    }

    std::for_each(planners.begin(), planners.end(), [](auto* planner){ delete planner; });

    exit(EXIT_SUCCESS);
}