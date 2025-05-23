#pragma once
#include "TreeNode.h"

// DO NOT CHANGE SIGNATURE OF FUNCTIONS IN THIS FILE

class DistanceCalculator {
public:
  virtual double calculateDistance(const TreeNode &nodeA,
                                   const TreeNode &nodeB) const = 0;
  virtual double calculateVerticalDistance(const TreeNode &root,
                                           const TreeNode &target,
                                           int dim) const = 0;
  virtual ~DistanceCalculator() {}
};

class ManhattanDistanceCalculator : public DistanceCalculator {
public:
  double calculateDistance(const TreeNode &nodeA,
                           const TreeNode &nodeB) const override;//override the function
  double calculateVerticalDistance(const TreeNode &root,
                                   const TreeNode &target,
                                   int dim) const override;
};

class EuclideanDistanceCalculator : public DistanceCalculator {
public:
  double calculateDistance(const TreeNode &nodeA,
                           const TreeNode &nodeB) const override;
  double calculateVerticalDistance(const TreeNode &root,
                                   const TreeNode &target,
                                   int dim) const override;
};

class HaversineDistanceCalculator : public DistanceCalculator {
private:
  static constexpr double EARTH_RADIUS = 6371000.0;
  double deg2rad(double deg) const;

public:
  double calculateDistance(const TreeNode &nodeA,
                           const TreeNode &nodeB) const override;
  double calculateVerticalDistance(const TreeNode &root,
                                   const TreeNode &target, int dim) const override;
};