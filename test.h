#ifndef test_h_
#define test_h_

#include <iostream>

struct Test {

   int i;

   bool operator!=(const Test& lhs) const { return !(operator==(lhs)); }

   Test() : i{0} {}
   Test(int in) : i{in} {}

   Test(const Test& ) = default;

   Test(Test&& ) = default;

   Test& operator=(const Test&) = default;
   
   Test& operator=(Test&&) = default;
   
   bool operator<(const Test& t) const noexcept
   {
       return (i < t.i) ? true : false;
   }
   
   bool operator>(const Test& t) const noexcept
   {
       return (i > t.i) ? true : false;
   }
   
   bool operator==(const Test& t) const noexcept
   {
       return (i == t.i) ? true : false;
   }

   friend std::ostream& operator<<(std::ostream& ostr, const Test& lhs) 
   {
     ostr << lhs.i << std::flush; 
     return ostr;
   }

  ~Test()
   {
      std::cout << "Test::~Test():i = " << i << ", " << std::endl << std::flush;
   }
 
};
#endif
