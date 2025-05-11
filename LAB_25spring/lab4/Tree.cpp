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

  // Step 2: read the number of nodes
  in >> nodeCount;
  // Step 3: read the nodes into a vector
  std::vector<TreeNode *> nodes;
  for (int i = 0; i < nodeCount; ++i) {
    double x, y;
    in >> x >> y;
    nodes.push_back(new TreeNode({x, y}));
  }

  // Step 4: build the tree using a balanced approach
  tree.buildTree(nodes);

  return in;
}

/*
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
    if (isLessThan( (*cur)[0],(*guess)[0]) ||
        (isEqual((*guess)[0], (*cur)[0]) && isLessThan((*cur)[1],(*guess)[1]))) {
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

  //!Check if the other subtree could contain a closer point
  double verticalDist = calculator->calculateVerticalDistance(*cur, target, axis);
  if (isLessThan(verticalDist, bestDist)) {
    if (cmp(&target, cur)) {
      nearestSearch(cur->right, target, depth + 1, guess, bestDist);
    }
    else {
      nearestSearch(cur->left, target, depth + 1, guess, bestDist);
    }
  }
}
*/

void TwoDimenTree::nearestSearch(TreeNode *cur,
                                  const TreeNode &target,
                                  int depth,
                                  TreeNode *&guess,
                                  double &bestDist) {
  if (cur == nullptr) {
    return;  // Base case: reached a leaf node or the tree is empty
  }

  // Calculate the distance between the current node and the target
  double curDist = calculator->calculateDistance(*cur, target);

  // Update the best guess if the current node is closer
  if (isLessThan(curDist, bestDist)) {
    bestDist = curDist;
    guess = cur;
  }

  // Tie-breaking: choose the lexicographically smaller node if distances are equal
  if (isEqual(curDist, bestDist)) {
    if (isLessThan((*cur)[0], (*guess)[0]) ||
        (isEqual((*cur)[0], (*guess)[0]) && isLessThan((*cur)[1], (*guess)[1]))) {
      guess = cur;
        }
  }

  // Determine the current axis (alternates between 0 and 1 for 2D)
  int axis = depth % 2;
  DimComparator cmp(axis);

  // Recursively search the subtree that is more likely to contain the nearest neighbor
  TreeNode *primarySubtree = cmp(&target, cur) ? cur->left : cur->right;
  TreeNode *secondarySubtree = cmp(&target, cur) ? cur->right : cur->left;

  nearestSearch(primarySubtree, target, depth + 1, guess, bestDist);

  // Check if the other subtree could contain a closer point
  double verticalDist = calculator->calculateVerticalDistance(*cur, target, axis);
  if (isLessThanOrEqual(verticalDist, bestDist)) {
    nearestSearch(secondarySubtree, target, depth + 1, guess, bestDist);
  }
}


//!added functions
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


void TwoDimenTree::buildTree(std::vector<TreeNode *> &nodes) {
  root = buildTreeRecursive(nodes, 0);
}

TreeNode *TwoDimenTree::buildTreeRecursive(std::vector<TreeNode *> &nodes, int depth) {
  if (nodes.empty()) {
    return nullptr;
  }

  // Determine the current axis
  int axis = depth % 2;
  DimComparator cmp(axis);

  // Sort nodes by the current axis
  std::sort(nodes.begin(), nodes.end(), cmp);

  // Find the median
  size_t medianIndex = nodes.size() / 2;
  TreeNode *medianNode = nodes[medianIndex];

  // Split the nodes into left and right subtrees
  std::vector<TreeNode *> leftNodes(nodes.begin(), nodes.begin() + medianIndex);
  std::vector<TreeNode *> rightNodes(nodes.begin() + medianIndex + 1, nodes.end());

  // Recursively build the left and right subtrees
  medianNode->left = buildTreeRecursive(leftNodes, depth + 1);
  medianNode->right = buildTreeRecursive(rightNodes, depth + 1);

  return medianNode;
}