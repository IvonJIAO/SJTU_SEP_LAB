# THIS MD IS USED TO RECORD THE DEVELOPEING PROGRESS
## Q1:apply the compare rule incorrectly    
if two points have the same distance, we should choose the one with the smaller x/y coordinate

## Q2:Too long time to run
for some test cases, the tree is too deep, 
so we need to use the iterative way to build the tree

## Q3:
#### test17: 
Wrong Answer: 
case:5373 5209,
expect:5373 5145,
actual:5380 5152.

*is this Manhattan or Euclidean distance?*

this is the Manhattan distance,
there is sth wrong with the compare rule
```
//Check if the other subtree could contain a closer point
double verticalDist = calculator->calculateVerticalDistance(*cur, target, axis);
if (isLessThanOrEqual(verticalDist, bestDist)) {
nearestSearch(secondarySubtree, target, depth + 1, guess, bestDist);
}
```

here I should use<=instead of <,
Even if the horizontal distance is equal,
according to the selection rule, 
if the smaller x/y coordinate appears in the other branch,
we should consider it