#include <stdexcept>
#include<cassert>
#include "TreeNode.h"

TreeNode::TreeNode(initializer_list<double> coords) {
  // TODO:here, how can we accomplish error check?

  for (double coord : coords) {
    coordinates.push_back(coord);
  }
  assert(coordinates.size() == 2);
  left = nullptr;
  right = nullptr;
}

const double &TreeNode::operator[](int index) const {
  assert(index >= 0 && index < coordinates.size());
  return coordinates[index];
}


int TreeNode::dimension() const {
  // !here we don't need to check if coordinates is empty
  //! because we have already checked it in the constructor
  //assert (coordinates.empty());
  return coordinates.size();
}

const vector<double> &TreeNode::getCoordinates() const {
  // TODO
  if (coordinates.empty())
    throw std::runtime_error("Coordinates are empty");
  return coordinates;
}

TreeNode::~TreeNode() {} // Even though the implementation might be empty, it's
                         // a good practice to define it.