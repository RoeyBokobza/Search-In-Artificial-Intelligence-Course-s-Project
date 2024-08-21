#pragma once

#include "Coordinate.h"
#include "Types.h"

class Map;

class Neighborhood
{
public:
    static CoordinateSet FourPrincipleDirection(const Coordinate& c, size_t R=1);
    static CoordinateSet EightPrincipleDirection(const Coordinate& c, size_t R=1);
};

class Heuristic
{
public:
    static float ManhattanDistance(const Coordinate& c1, const Coordinate& c2);
    static float ChebyshevDistance(const Coordinate& c1, const Coordinate& c2);
};

class ObjectiveFunction
{
public:
    static long SumOfCost(const Paths&);
    static long Makespan(const Paths&);
    static long PathLength(const Path&);
};

class Validator
{
public:
    static Agents ValidAgents(const Agents& src, const Map& m);
};