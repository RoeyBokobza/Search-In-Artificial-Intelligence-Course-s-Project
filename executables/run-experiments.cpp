#include <cstdlib>
#include <fstream>
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

void WriteDataframeRow(std::fstream& out_file, ISingleAgentPathPlanner* planner, const std::vector<struct Plan>& planner_plans, const Map& map, const Scenario& scenario, const std::string& file_delimitier);
std::fstream GenerateDataframe(const std::string& directory_path, const std::string& filename, const std::string& file_delimitier);
std::tuple<long, long, long, long, float> SummarizePlans(const std::vector<struct Plan>& planner_plans);

/*
    Create a dataframe of all scenarios of a given map
    Each dataframe row summarizes (aggregates) the reuslt of a planner (A*, IDA*) on a given scenario
    Each scenario contains mutliple start and goal location pairs
*/
int main(int argc, const char** argv)
{
    if(argc != 4)
    {
        Print(Red, "Expected 3 arguments: map_file_path, scenarios_directory_path, output_directory_path. Given: ", argc, '\n');
        exit(EXIT_FAILURE);
    }

    Map map(argv[1]);
    const auto graph = map.CreateGraph(true, true);

    std::vector<Scenario> scenarios = Scenario::LoadAllScenarios(argv[2]);
    std::vector<ISingleAgentPathPlanner*> planners = {new Astar(), new IDAstar(), new IBDAstar(), new IBDAstar2()};

    const auto out_directory_path = argv[3];
    const auto out_file_name = map.GetName() + ".dataframe";
    const auto file_delimitier = ",";
    auto out_file = GenerateDataframe(out_directory_path, out_file_name, file_delimitier);

    for(const auto& scenario: scenarios)
    {
        for(auto* planner: planners)
        {
            std::vector<struct Plan> planner_plans;

            for(const auto& agent: scenario.GetAgents())
            {
                auto plan = planner->Plan(graph, agent);
                planner_plans.push_back(plan);
                
                Print(planner->GetColor(), "Planner: ", planner->GetName(), '\n');
                Print(Default, plan, '\n');
            }

            WriteDataframeRow(out_file, planner, planner_plans, map, scenario, file_delimitier);

            planner_plans.clear();
        }
    }

    out_file.close();

    std::for_each(planners.begin(), planners.end(), [](auto* planner){ delete planner; });

    exit(EXIT_SUCCESS);
}

std::fstream GenerateDataframe(const std::string& directory_path, const std::string& filename, const std::string& file_delimitier)
{
    std::fstream file{directory_path + '/' + filename, std::ios::out};

    if(!file)
    {
        Print(Red, __PRETTY_FUNCTION__," Failed to create ", filename , "!\n");
        exit(EXIT_FAILURE);
    }
    else // write columns names
    {
        file << 
        "PlannerName" << file_delimitier <<
        "MapName" << file_delimitier <<
        "ScenarioName" << file_delimitier <<
        "NumberOfGeneratedNodes" << file_delimitier <<
        "NumberOfExpandedNodes" << file_delimitier <<
        "NumberOfReopenedNodes" << file_delimitier <<
        "AllPathLengths" << file_delimitier <<
        "Runtime" <<
        '\n';
    }
    
    return std::forward<std::fstream>(file);
}

void WriteDataframeRow(std::fstream& out_file, ISingleAgentPathPlanner* planner, const std::vector<struct Plan>& planner_plans, const Map& map, const Scenario& scenario, const std::string& file_delimitier)
{
    const auto& [total_generated_vertices, total_expanded_vertices, total_reopened_vertices, total_path_lengths, total_runtime] = SummarizePlans(planner_plans);

    out_file << 
    planner->GetName() << file_delimitier <<
    map.GetName() << file_delimitier <<
    scenario.ToString() << file_delimitier << 
    std::to_string(total_generated_vertices) << file_delimitier <<
    std::to_string(total_expanded_vertices) << file_delimitier <<
    std::to_string(total_reopened_vertices) << file_delimitier <<
    std::to_string(total_path_lengths) << file_delimitier <<
    std::to_string(total_runtime) <<
    '\n';

    out_file.flush();
}

std::tuple<long, long, long, long, float> SummarizePlans(const std::vector<struct Plan>& planner_plans)
{
    long total_generated_vertices = 0;
    long total_expanded_vertices = 0;
    long total_reopened_vertices = 0;
    long total_path_lengths = 0;
    float total_runtime = 0;

    for(const auto& plan: planner_plans)
    {
        total_generated_vertices += plan.number_of_generated_nodes;
        total_expanded_vertices += plan.number_of_expanded_nodes;
        total_reopened_vertices += plan.number_of_reopened_nodes;
        total_path_lengths += plan.path.size();
        total_runtime += plan.runtime;
    }

    return {total_generated_vertices, total_expanded_vertices, total_reopened_vertices, total_path_lengths, total_runtime};
}