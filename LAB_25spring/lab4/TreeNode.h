#pragma once

#include <initializer_list>
#include <vector>

using namespace std;

class TreeNode {
  friend class TwoDimenTree;

private:
  vector<double> coordinates;
  TreeNode *left;
  TreeNode *right;

public:
  /* DO NOT CHANGE SIGNATURE*/
  TreeNode(initializer_list<double> coords);

  /* DO NOT CHANGE SIGNATURE*/
  const double &operator[](int index) const;
  //?returns the corresponding dimension.
  //?For example, we have two dimensions, the first dimension is 0,
  //?the second dimension is 1.


  /* DO NOT CHANGE SIGNATURE*/
  int dimension() const;
  //?this is used to get the dimension of the node
  //?in this case ,it is always 2

  /* DO NOT CHANGE SIGNATURE*/
  const vector<double> &getCoordinates() const;
  //?this is used to get the coordinates of the node

  /* DO NOT CHANGE SIGNATURE*/
  ~TreeNode(); // Even though empty, defined for completeness.
};