Implementations of  A*, IDA*. IBDA* 

Maps & scenarios were taken from: https://movingai.com/benchmarks/mapf/index.html

To generate a dataframe, modify the bash script executables/run-experiments.sh where first parameter is map path, second scenario directory path, third is output directory path.
This script will run ALL scenarios of a given map (usually 25). Each scenario file defines multiple start and goal locations for an agent.


To run a scenario over a given map, modify the bash script executables/run-single-scenario.sh where first parameter is map path, second is scenario file path, third is number of runs (i.e, number of different start and goal locations).
This script will print to terminal plan statistics and visualize the path on the grid, for example:
![image](https://github.com/user-attachments/assets/15d2d2b9-461f-4180-bc08-036dd4c240e3)
![image](https://github.com/user-attachments/assets/5da3c9e7-e6fd-4bc8-acca-d59e3a09bd21)


Under outputs you can see an example of a generated dataframe
