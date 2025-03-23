#ifndef RECCHECK
#include <iostream>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkEqualPaths(Node* node, int currDepth, int& depth){
  if(node == nullptr){
    return true; 
  }
  if(node->left == nullptr && node->right == nullptr){
    if(depth == -1){
      depth = currDepth; 
      return true; 
    } else {
      return currDepth == depth; 
    }
  }
  return checkEqualPaths(node->left, currDepth + 1, depth) && checkEqualPaths(node->right, currDepth + 1, depth); 
}


bool equalPaths(Node * root)
{
    // Add your code below
    int depth = -1; 
    return checkEqualPaths(root, 0, depth);
}

