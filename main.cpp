#include <cstdlib>
#include <utility>
#include <iostream>
#include <initializer_list>
#include "test.h"
#include "sbstree.h"
using namespace std;


int main(int argc, char** argv) 
{
  std::initializer_list<int> lst = {50, 39, 15, 65, 69, 50, 25, 20, 70, 100, 40, 34, 37, 30, 10, 33, 36, 38, 85, 90, 60, 35, 80, 89};
  sbstree<Test> tree;

  for (const auto& i : lst) tree.insert(Test{i});
        
  cout << "This is the input tree " << tree << " printed in debug level order:\n";
  
  tree.debug_printlevelOrder(cout);

  cout << "We now start the removal loop\n";
  
  for (const auto& key : lst) {
      
      cout << "Tree before removal of Test{" << key << "} " << tree; 
               
      tree.remove(Test{key});

      cout << "Tree after removal of Test{" << key << "}. " << tree; 
      cout << "\nLevel-order print after the removal of Test{" << key << "}\n";
      
      tree.printlevelOrder(cout);

      cout << flush << "\nDebug print\n";

      tree.debug_printlevelOrder(cout);
  }

  return 0;


    sbstree<Test> tree1 = { 5, 15, 7, 17, 3, 13, 4, 14,  2, 12,  6, 16, 9, 19}; 
    
    cout << "tree1 = " << tree1 << '\n' << "Printing tree1 level order:\n";
    
    tree1.printlevelOrder(cout);
        
    sbstree<Test> tree2 = { Test{15}, Test{25}, Test{17}, Test{27}, Test{13}, Test{23}, Test{14}, Test{24},  Test{12}, Test{41},  Test{16}, Test{6}, Test{19}, Test{20}}; 
    
    cout << "\ntree2 = " << tree2 << "\nPrinting tree2 level order:\n";
    
    tree2 = tree1; 
    tree2.printlevelOrder(cout);
    
   cout << "\nAfter 'tree2 = tree1', tree2 = " << tree2 << "\nPrinting tree2 level order:\n" << flush;
   tree2.printlevelOrder(cout);
   
   vector<int> vec = {-5, -15, -7, -17, -3, -13, -4, -14,  -2, -12,  -6, -16, -9, -19}; 
   for (const auto& ref : vec) {

     tree2.remove(ref); 

     cout << "After removing " << ref << " from tree2, tree2 = \n";
     
     tree2.printlevelOrder(cout);
     cout << flush;
   }
   
   return 0;
}

