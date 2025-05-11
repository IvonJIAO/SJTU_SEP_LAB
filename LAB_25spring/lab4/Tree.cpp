#include "Tree.h"
#include <algorithm>
#include <cassert>
#include <limits>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;

/*
 * You can use this struct to compare a certain dimension of the tree nodes!
 * For example, if you want to compare the first dimension, you can simply call
 * DimComparator(0), such as DimComparator(0)(node_a, node_b).
 */
struct DimComparator {
  int dim;
  DimComparator(int d) : dim(d) {}
  bool operator()(const TreeNode *a, const TreeNode *b) const {
    assert(a->dimension() == b->dimension() && a->dimension() > dim);
    //check if dimension is right
    return isLessThan((*a)[dim], (*b)[dim]);
  }
};

TwoDimenTree::TwoDimenTree() {
  // TODO:constructor DistanceCalculator *calculator;TreeNode *root;
  calculator = nullptr;
  root = nullptr;
}

TwoDimenTree::~TwoDimenTree() {
  // TODO:destructor
  makeEmpty(root);

}

TreeNode *TwoDimenTree::findNearestNode(const TreeNode &target) {
  // TODO: recursively search the tree
  //? if two distances are equal, we should choose the one with smaller coordinates
 /* if (!root) {
    return nullptr;
  }*/  //*this corner case is contained into the inner function
  TreeNode *cur=root;
  TreeNode *guess=nullptr;
  double bestDist=std::numeric_limits<double>::max(); //?we need to make bestDist positive infinite
  nearestSearch(cur,target,0,guess,bestDist);
  //the depth is 0 at the beginning
  return guess;
}

istream &operator>>(istream &in, TwoDimenTree &tree) {
  string distanceType;
  int nodeCount;

  //* Step 1

  in >> distanceType;
  if (distanceType == "Manhattan") {
    tree.setDistanceCalculator(new ManhattanDistanceCalculator());
  } else if (distanceType == "Euclidean") {
    tree.setDistanceCalculator(new EuclideanDistanceCalculator());
  } else if (distanceType == "Haversine") {
    tree.setDistanceCalculator(new HaversineDistanceCalculator());
  } else {
    throw runtime_error("Unsupported distance type: " + distanceType);
  }

  // Step 2: 读取树中的节点数量
  in >> nodeCount;
  for (int i = 0; i < nodeCount; ++i) {
    double x, y;
    in >> x >> y;
    TreeNode *node = new TreeNode({x, y});
    tree.insert(*node); // 假设 TwoDimenTree 有插入节点的方法
  }

  return in;
}


void TwoDimenTree::nearestSearch(TreeNode *cur,
                                  const TreeNode &target,
                                  int depth,
                                  TreeNode *&guess,
                                  double &bestDist)
{
  if (cur == nullptr) {
    return;  // Reached a leaf node or the tree is empty
  }

  // Update guess and bestDist if current node is closer
  double curDist = calculator->calculateDistance(*cur, target);
  if (isLessThan(curDist, bestDist)) {
    bestDist = curDist;
    guess = cur;
  }

  // Tie-breaker: compare coordinates in lexicographical order
  if (isEqual(curDist, bestDist)) {
    if (isLessThan((*guess)[0], (*cur)[0]) ||
        (isEqual((*guess)[0], (*cur)[0]) && isLessThan((*guess)[1], (*cur)[1]))) {
      guess = cur;
        }
  }

  // Determine current axis
  int axis = depth % 2;
  DimComparator cmp(axis);

  // Recursive call: go down the tree along the splitting axis
  if (cmp(&target, cur)) {
    nearestSearch(cur->left, target, depth + 1, guess, bestDist);
  } else {
    nearestSearch(cur->right, target, depth + 1, guess, bestDist);
  }

  // Check if the other subtree could contain a closer point
  double verticalDist = abs((*cur)[axis] - target[axis]);
  if (isLessThan(verticalDist, bestDist)) {
    if (cmp(&target, cur)) {
      nearestSearch(cur->right, target, depth + 1, guess, bestDist);
    } else {
      nearestSearch(cur->left, target, depth + 1, guess, bestDist);
    }
  }
}


//!added function
void TwoDimenTree::setDistanceCalculator(DistanceCalculator *NewCreate_calculator) {
  this->calculator = NewCreate_calculator;
}

void TwoDimenTree::insert(TreeNode &node) {
  insertRecursive(root, node, 0);  // 根节点从第0个维度开始
}

void TwoDimenTree::insertRecursive(TreeNode *&cur, TreeNode &node, int depth) {
  // Base case: If the current node is null, insert the node here
  if (cur == nullptr) {
    cur = &node;
    return;
  }

  // Get the current dimension to compare
  int dim = depth % 2;  // Use modulo to alternate between 0 and 1 for dimensions (x, y)

  // Compare the node's value along the current dimension
  if (isLessThan(node[dim], (*cur)[dim])) {
    // If the target node's value is less than current node's value, go left
    insertRecursive(cur->left, node, depth + 1);
  } else {
    // Otherwise, go right
    insertRecursive(cur->right, node, depth + 1);
  }
}

void TwoDimenTree::makeEmpty(TreeNode*&node) {
  if (node == nullptr) {
    return;
  }
  makeEmpty(node->left);
  makeEmpty(node->right);
  delete node;
  node = nullptr;
}
