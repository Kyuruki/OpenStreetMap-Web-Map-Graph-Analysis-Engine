#include "application.h"
#include <iostream>
#include <limits>
#include <map>
#include <queue> // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

double INF = numeric_limits<double>::max();
class prioritize {
  public:
  bool operator()(const pair< long long, double>& p1, const pair<long long, double>& p2) const{
    return p1.second > p2.second;
  }
};

void buildGraph(istream &input, graph<long long, double> &g,
                vector<BuildingInfo> &buildings,
                unordered_map<long long, Coordinates> &coords) {
  json data = json::parse(input);
  for(const auto& building : data["buildings"]){
    BuildingInfo info;
    info.id = building["id"];
    info.location.lat = building["lat"];
    info.location.lon = building["lon"];
    info.name = building["name"];
    if (building.contains("abbr")) {
      info.abbr = building["abbr"];
    }
    buildings.push_back(info);
    g.addVertex(info.id);
  }
  for (const auto& waypoint : data["waypoints"]) {
    long long id = waypoint["id"];
    Coordinates coord;
    coord.lat = waypoint["lat"];
    coord.lon = waypoint["lon"];
    g.addVertex(id);
    coords[id] = coord;
  }
  for (const auto& footway : data["footways"]) {
    vector<long long> nodes;
    for (const auto& nodeId : footway) {
      nodes.push_back(nodeId);
    }
    for (size_t i = 0; i + 1 < nodes.size(); i++) {
      long long node1 = nodes[i];
      long long node2 = nodes[i + 1];
      Coordinates coord1, coord2;
      bool found1 = false, found2 = false;
      if (coords.count(node1) > 0) {
        coord1 = coords[node1];
        found1 = true;
      } else {
        for (const auto& b : buildings) {
          if (b.id == node1) {
            coord1 = b.location;
            found1 = true;
            break;
          }
        }
      }
      
      if (coords.count(node2) > 0) {
        coord2 = coords[node2];
        found2 = true;
      } else {
        for (const auto& b : buildings) {
          if (b.id == node2) {
            coord2 = b.location;
            found2 = true;
            break;
          }
        }
      } 
      if (found1 && found2) {
        double dist = distBetween2Points(coord1, coord2);
        g.addEdge(node1, node2, dist);
        g.addEdge(node2, node1, dist);
      }
    }
  }
  for (const auto& building : buildings) {
    for (const auto& [waypointId, waypointCoord] : coords) {
      double dist = distBetween2Points(building.location, waypointCoord);
      if (dist <= 0.036) {
        g.addEdge(building.id, waypointId, dist);
        g.addEdge(waypointId, building.id, dist);
      }
    }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings,
                             const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double> &G, long long start,
                           long long target,
                           const set<long long> &ignoreNodes) {
  priority_queue<pair<long long, double>, vector<pair<long long, double>>,
  prioritize> QUEUELIST;
  unordered_map< long long, double> distances;
  unordered_map<long long, long long> predecessors;
  set<long long> visited;
  if(start == target){
    return vector<long long>{start};
  }
  vector<long long> Vert = G.getVertices();
  for(long long k : Vert){
    distances[k] = INF;
  }
  distances[start] = 0.0;
  QUEUELIST.push({start, 0.0});
  while(!QUEUELIST.empty()){
    auto [current, currentDist] = QUEUELIST.top();
    QUEUELIST.pop();
    if(visited.count(current)>0){
      continue;
    }
    visited.insert(current);
    if(current == target){
      break;
    }
    if(ignoreNodes.count(current) > 0){
      if(current != start && current != target){
        continue;
      }
    }
    set<long long> neighbors = G.neighbors(current);
    for ( long long K : neighbors){
      if(ignoreNodes.count(K) > 0 && K != target){
        continue;
      }
      if (visited.count(K) > 0){
        continue;
      }
      double WeightofEdge;
      if(G.getWeight(current,K,WeightofEdge)){
        double NewDistance = distances[current]+WeightofEdge;
        if(NewDistance < distances[K]){
          distances[K] = NewDistance;
          predecessors[K] = current;
          QUEUELIST.push({K,NewDistance});
        }
      }
    }
  }
  //OUTSIDE OF WHILE LOOP
  if(distances[target] == INF){
    return vector< long long>{};
  }
  stack<long long> StackofPath;
  long long current = target;
  while(current != start){
    StackofPath.push(current);
    if(predecessors.count(current) == 0){
      return vector<long long>{};
    }
    current = predecessors[current];
  }
  StackofPath.push(start);
  vector<long long> NewPath;
  while(!StackofPath.empty()){
    NewPath.push_back(StackofPath.top());
    StackofPath.pop();
  }
  return NewPath;
}

double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
