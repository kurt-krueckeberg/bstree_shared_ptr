#include "sbstree.h"
#include "test.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) 
{
  sbstree<Test> tree = {Test{50}, Test{39}, Test{15}, Test{65}, Test{69}, Test{50}, Test{25}, Test{20}, Test{70}, Test{100}, Test{40}, Test{34}, Test{37}, Test{30}, Test{10}, Test{33}, Test{36}, Test{38}, Test{85}, Test{90}, Test{60}, Test{35}, Test{80}, Test{89} };  
  cout << tree;

  for (const auto& x :  {Test{50}, Test{39}, Test{15}, Test{65}, Test{69}, Test{50}, Test{25}, Test{20}, Test{70}, Test{100}, Test{40}, Test{34}, Test{37}, Test{30}, Test{10}, Test{33}, Test{36}, Test{38}, Test{85}, Test{90}, Test{60}, Test{35}, Test{80}, Test{89}}) {

      tree.remove(x);
  
      cout << "Tree after removing " << x << '\n';
      
      cout << tree;  
      
      cout << "\nPrinting tree level-order\n";
      
      tree.printlevelOrder(cout);
      
      tree.debug_printlevelOrder(cout);
  }

  return 0;
}
