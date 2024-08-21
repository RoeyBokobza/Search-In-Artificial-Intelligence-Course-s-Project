#pragma once

#include <boost/unordered/unordered_set.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <tuple>
#include <vector>
#include "Agent.h"
#include "Coordinate.h"
#include "Edge.h"
#include "Terrain.h"

using CoordinateSet = boost::unordered_set<Coordinate, Coordinate::Hasher, Coordinate::Equal>;
using CoordinateMap = boost::unordered_map<Coordinate, std::vector<int>, Coordinate::Hasher, Coordinate::Equal>;
using ConflictsMap = boost::unordered_map<Coordinate, int, Coordinate::Hasher, Coordinate::Equal>;
using GridRow = std::vector<Terrain>;
using Grid = std::vector<GridRow>;
using Path = std::vector<Coordinate>;
using Paths = std::vector<Path>;
using NeighborhoodFunction = CoordinateSet(*)(const Coordinate& c, size_t R);
using HeuristicFunction = float(*)(const Coordinate& c1, const Coordinate& c2);
using AdjacencyList = boost::unordered::unordered_map<Coordinate, CoordinateSet, Coordinate::Hasher, Coordinate::Equal>;
using EdgeWeights = boost::unordered::unordered_map<Edge, float, Edge::Hasher, Edge::Equal>;
using Agents = std::vector<Agent>;
using AgentsIndicesSet = boost::unordered::unordered_set<int>;
using PlanResult = std::tuple<bool, Paths, unsigned long, float>;
using ScenarioResult = std::tuple<bool, Paths, float, int, unsigned long>;
using EdgeSet = boost::unordered::unordered_set<Edge, Edge::Hasher, Edge::Equal>;
using EdgeWeightFunction = boost::unordered::unordered_map<Edge, float, Edge::Hasher, Edge::Equal>;