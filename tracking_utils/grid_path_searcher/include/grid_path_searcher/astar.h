#ifndef _ASTAR_H
#define _ASTAR_H

#include <Eigen/Eigen>
#include <iostream>
#include <map>
#include <ros/console.h>
#include <ros/ros.h>
#include <string>
#include <unordered_map>
// #include "grad_spline/sdf_map.h"
#include <plan_env/grid_map.h>
#include <boost/functional/hash.hpp>
#include <queue>
#define IN_CLOSE_SET 'a'
#define IN_OPEN_SET 'b'
#define NOT_EXPAND 'c'
#define inf 1 >> 30
enum { REACH_HORIZON = 1, REACH_END = 2, NO_PATH = 3};


class Node {
public:
  /* -------------------- */
  Eigen::Vector3i index;
  Eigen::Vector3d position;
  double g_score, f_score;
  Node* parent;
  char node_state;

  double time;  // dyn
  int time_idx;

  /* -------------------- */
  Node() {
    parent = NULL;
    node_state = NOT_EXPAND;
  }
  ~Node(){};
};
typedef Node* NodePtr;

class NodeComparator0 {
public:
  bool operator()(NodePtr node1, NodePtr node2) {
    return node1->f_score > node2->f_score;
  }
};

template <typename T>
struct matrix_hash0 : std::unary_function<T, size_t> {
  std::size_t operator()(T const& matrix) const {
    size_t seed = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
      auto elem = *(matrix.data() + i);
      seed ^= std::hash<typename T::Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};

class NodeHashTable0 {
private:
  /* data */
  std::unordered_map<Eigen::Vector3i, NodePtr, matrix_hash0<Eigen::Vector3i>> data_3d_;
  std::unordered_map<Eigen::Vector4i, NodePtr, matrix_hash0<Eigen::Vector4i>> data_4d_;

public:
  NodeHashTable0(/* args */) {
  }
  ~NodeHashTable0() {
  }
  void insert(Eigen::Vector3i idx, NodePtr node) {
    data_3d_.insert(make_pair(idx, node));
  }
  void insert(Eigen::Vector3i idx, int time_idx, NodePtr node) {
    data_4d_.insert(make_pair(Eigen::Vector4i(idx(0), idx(1), idx(2), time_idx), node));
  }

  NodePtr find(Eigen::Vector3i idx) {
    auto iter = data_3d_.find(idx);
    return iter == data_3d_.end() ? NULL : iter->second;
  }
  NodePtr find(Eigen::Vector3i idx, int time_idx) {
    auto iter = data_4d_.find(Eigen::Vector4i(idx(0), idx(1), idx(2), time_idx));
    return iter == data_4d_.end() ? NULL : iter->second;
  }

  void clear() {
    data_3d_.clear();
    data_4d_.clear();
  }
};

class Astar {
private:
  /* ---------- main data structure ---------- */
  vector<NodePtr> path_node_pool_;
  int use_node_num_, iter_num_;
  NodeHashTable0 expanded_nodes_;
  std::priority_queue<NodePtr, std::vector<NodePtr>, NodeComparator0> open_set_;
  std::vector<NodePtr> path_nodes_;

  /* ---------- record data ---------- */
  GridMap::Ptr  edt_environment_;
  bool has_path_ = false;
  bool has_map = false;

  /* ---------- parameter ---------- */
  /* search */
  double lambda_heu_ = 1;
  int allocate_num_;
  double tie_breaker_;
  /* map */
  double resolution_, inv_resolution_, time_resolution_, inv_time_resolution_;
  Eigen::Vector3d origin_, map_size_3d_;
  double time_origin_;

  /* helper */
  Eigen::Vector3i posToIndex(Eigen::Vector3d pt);
  Eigen::Vector3d IndexTopos(Eigen::Vector3i index);
  int timeToIndex(double time);
  void retrievePath(NodePtr end_node);

  /* heuristic function */
  double getDiagHeu(Eigen::Vector3d x1, Eigen::Vector3d x2);
  double getManhHeu(Eigen::Vector3d x1, Eigen::Vector3d x2);
  double getEuclHeu(Eigen::Vector3d x1, Eigen::Vector3d x2);

public:
  Astar(){};
  ~Astar();

  enum { REACH_END = 1, NO_PATH = 2 };

  /* main API */
  void setParam(ros::NodeHandle& nh);
  void reset();
  int search(Eigen::Vector3d start_pt, Eigen::Vector3d end_pt, bool dynamic = false,
             double time_start = -1.0);

  void setEnvironment(const GridMap::Ptr& env);
  std::vector<Eigen::Vector3d> getPath();
  std::vector<NodePtr> getVisitedNodes();

  typedef shared_ptr<Astar> Ptr;
};

// namespace fast_tracker

#endif