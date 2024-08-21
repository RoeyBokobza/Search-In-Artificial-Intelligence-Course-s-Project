#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Types.h"

struct Plan
{
    static constexpr auto ROWS_DELIMITER = "\n";

    int number_of_generated_nodes = 0;
    int number_of_expanded_nodes = 0;
    int number_of_reopened_nodes = 0; // A node is reopened when the search regenerates an expanded state through a better path - relevant for IDA*/RBFS/ILBFS
    float runtime = 0.000;
    Path path;

    void Clear(void)
    {
        number_of_generated_nodes = 0;
        number_of_expanded_nodes = 0;
        number_of_reopened_nodes = 0;
        runtime = 0.000;
        path.clear();
    }

    std::string ToString(bool print_path = false) const
    {
        std::stringstream ss;

        ss << "Number of GENERATED nodes: " << std::to_string(number_of_generated_nodes) << ROWS_DELIMITER;
        ss << "Number of RE-OPENED nodes: " << std::to_string(number_of_reopened_nodes) << ROWS_DELIMITER;
        ss << "Number of EXPANDED nodes: " << std::to_string(number_of_expanded_nodes) << ROWS_DELIMITER;
        ss << "Path length: " << std::to_string(path.size()) << ROWS_DELIMITER;
        ss << std::fixed << std::setprecision(5) << "Runtime: " << std::to_string(runtime) << " (seconds)" << ROWS_DELIMITER;

        if(print_path)
        {
            ss << "Final path" << ROWS_DELIMITER;
            int j = 1;
            for(const auto& coordinate: path)
            {
                ss << "#" << std::to_string(j) << ")\t" << coordinate << ROWS_DELIMITER;
                j += 1;
            }
        }

        return ss.str();
    }

    friend std::ostream& operator << (std::ostream& out, const Plan& p)
    {
        return out << p.ToString();
    }
};