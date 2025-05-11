#include <stdexcept>

#include "TreeNode.h"

TreeNode::TreeNode(initializer_list<double> coords) {
  // TODO:here, how can we accomplish error check?

  for (double coord : coords) {
    coordinates.push_back(coord);
  }
}

const double &TreeNode::operator[](int index) const {
  // TODO
  if (index < 0 || index >= coordinates.size())
    throw std::out_of_range("Index out of range");
  return coordinates[index];
}

int TreeNode::dimension() const {
  // TODO
  if (coordinates.empty())
  return 0;
  else return coordinates.size();
}

const vector<double> &TreeNode::getCoordinates() const {
  // TODO
  if (coordinates.empty())
    throw std::runtime_error("Coordinates are empty");
  return coordinates;
}

TreeNode::~TreeNode() {} // Even though the implementation might be empty, it's
                         // a good practice to define it.