#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
  unordered_map<VertexT, unordered_map<VertexT,WeightT>> List;
  size_t CountofEdge;


//KNOCK ALL OF THIS OUT BY TUESDAY - means u got 2 days

 public:
  /// Default constructor
  graph() {
    CountofEdge = 0;
    
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    if(List.count(v) > 0){
      return false;
    }
    List[v] = unordered_map<VertexT,WeightT>();
    return true;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    if(List.count(from) == 0 || List.count(to) == 0){
      return false;
    }
    bool NewEdge = (List[from].count(to) == 0);
    List[from][to] = weight;
    if(NewEdge){
      CountofEdge++;
    }
    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    auto fromIt = List.find(from);
    if(fromIt == List.end()){
      return false;
    }
    auto toIt = fromIt->second.find(to);
    if(toIt == fromIt->second.end()){
      return false;
    }
    weight = toIt->second;
    return true;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    auto it = List.find(v);
    if(it != List.end()){
      for(const auto& pair : it->second){
        S.insert(pair.first);
      }
    }
    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {
    vector<VertexT> Vert;
    Vert.reserve(List.size());
    for(const auto& pair : List){
      Vert.push_back(pair.first);
    }
    return Vert;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    return List.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    return CountofEdge;
  }
};
