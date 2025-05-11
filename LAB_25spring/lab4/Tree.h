#pragma once

#include <iostream>
#include <vector>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;

class TwoDimenTree {
  /* DO NOT CHANGE SIGNATURE */
  friend istream &operator>>(istream &in,
                             TwoDimenTree &tree); 

private:
  /* data */
  /* calculator should indicate the distance calculation method being used */
  DistanceCalculator *calculator;
  TreeNode *root;

  //! these two functions are added by me to accomplish the recursive function
  void nearestSearch(TreeNode *cur,
                   const TreeNode &target,
                   int depth,
                   TreeNode *& guess,
                   double &bestDist);

  void insertRecursive(TreeNode *&cur, TreeNode &node, int depth);

  void makeEmpty(TreeNode*&node);

  TreeNode * buildTreeRecursive(std::vector<TreeNode *> &nodes, int depth);



public:
  /* methods */
  /* DO NOT CHANGE SIGNATURE */
  TwoDimenTree();

  /* DO NOT CHANGE SIGNATURE */
  TreeNode *findNearestNode(const TreeNode &target);
  //! there are too little parameters, so we use it as a wrapper

  /* DO NOT CHANGE SIGNATURE */
  ~TwoDimenTree(); /* DO NOT CHANGE */

  //! these two are added by me
  void setDistanceCalculator(DistanceCalculator *calculator);

  void insert(TreeNode &node);

  void buildTree(std::vector<TreeNode *> &nodes);
};
