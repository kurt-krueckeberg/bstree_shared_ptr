#include "sbtree.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) 
{
  initializer_list<int> lst{50, 39, 15, 65, 69, 150, 125, 20, 70, 100, 40, 34, 37, 30, 10, 33, 36, 38, 85, 90, 60, 35, 80, 89};  
  
  sbtree<int> tree{lst};

  cout << tree;

  for (const auto& x : lst) {

      tree.remove(x);
      cout << "Tree after removing " << x << '\n';
      cout << tree;  
  }

  return 0;
}
