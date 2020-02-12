#ifndef sbstree_h
#define sbstree_h
#include <memory>
#include <utility>
#include <iostream>
#include <algorithm>
#include <queue>
#include <initializer_list>

template<typename T> class sbstree {

    struct Node{
        T key;
        Node *parent;

        std::shared_ptr<Node> left; 
        std::shared_ptr<Node> right;

        Node();

        Node(const T& x, Node *parent_in = nullptr) noexcept : key{x}, parent{parent_in} 
        {
        } 

        Node(const Node& lhs) noexcept;

        Node& operator=(const Node& lhs) noexcept;
         
        Node(Node&& lhs) noexcept : key{lhs.key}, left{std::move(lhs.left)}, right{lhs.right}, parent{lhs.parent} 
        { 
        }

        Node& operator=(Node&& lhs) noexcept
        { 
           key = lhs.key;

           left = std::move(lhs.left);
           right = lhs.right;

           parent = lhs.parent; 
        } 
        
        friend std::ostream& operator<<(std::ostream& ostr, const Node& node) 
        {
            return node.print(ostr);
        }

        std::ostream& print(std::ostream& ostr) const noexcept
        {
            return ostr << key << ", " << std::flush;
        }
        
        bool isLeaf() const noexcept
        {
           return (!left && !right) ? true : false;
        }

        std::ostream& debug_print(std::ostream& ostr) const noexcept;
    };

   bool remove(const T& x, std::shared_ptr<Node>& p); 

   bool insert(const T& x, std::shared_ptr<Node>& p) noexcept;

   void move(sbstree&& lhs) noexcept
   {
       root = std::move(lhs.root);
       size = lhs.size;
       lhs.size = 0;
   }
   
   template<typename Functor> void in_order(Functor f, const std::shared_ptr<Node>& current) const noexcept; 
   template<typename Functor> void post_order(Functor f, const std::shared_ptr<Node>& current) const noexcept; 
   template<typename Functor> void pre_order(Functor f, const std::shared_ptr<Node>& current) const noexcept; 
 
   std::shared_ptr<Node> root; 
   std::size_t size;

   class NodeLevelOrderPrinter {
   
      std::ostream& ostr;
      int current_level;
      int height_;
       
      std::ostream& (Node::*pmf)(std::ostream&) const noexcept;

      void display_level(std::ostream& ostr, int level) const noexcept
      {
        ostr << "\n" << "current level = " <<  level << '\n'; 
         
        // Provide some basic spacing to tree appearance.
        /*
        std::size_t num = height_ - level + 1;
      
        std::string str( num, ' ');
      
        ostr << str; 
         */ 
      }
      
      public: 
      
      NodeLevelOrderPrinter (const sbstree<T>& tree, std::ostream& (Node::*pmf_)(std::ostream&) const noexcept, std::ostream& ostr_in):  ostr{ostr_in}, current_level{0}, pmf{pmf_}
      { 
          height_ = tree.height(); 
      }

      NodeLevelOrderPrinter (const NodeLevelOrderPrinter& lhs): ostr{lhs.ostr}, current_level{lhs.current_level}, height_{lhs.height_}, pmf{lhs.pmf} {}
      
      void operator ()(const Node *pnode, int level)
      { 
          // Did current_level change?
          if (current_level != level) { 
         
              current_level = level;
         
              display_level(ostr, level);       
          }
         
          (pnode->*pmf)(std::cout);
         
          std::cout << '\n' << std::flush;
      }
   };
 
   std::size_t height(const std::shared_ptr<Node>& node) const noexcept;
 
  public:

    sbstree() : root{nullptr}, size{0}
    {
    } 

   ~sbstree() = default;

    sbstree(const sbstree& lhs);

    sbstree(const std::initializer_list<T>& list) noexcept : size{0}
    {
        for (const auto& x : list)
            insert(x);
    }

    sbstree(sbstree&& lhs)
    {
       move(std::move(lhs));
    }

    sbstree& operator=(const sbstree& lhs);

    sbstree& operator=(sbstree&& lhs);

    void printlevelOrder(std::ostream& ostr) const noexcept;

    void debug_printlevelOrder(std::ostream& ostr) const noexcept;
    
    bool empty() const noexcept
    {
        return (size == 0) ? true : false;
    }
    
    std::size_t height() const noexcept
    {
       if (empty()) 
          return 0;
       else
          return height(root);
    }

    bool insert(const T& x) noexcept;
    
    bool remove(const T& x)
    {
      bool rc = remove(x, root); 
      if (rc) --size;
      return rc; 
    }

    template<typename Functor> void in_order(Functor f) const noexcept
    {
        return in_order(f, root);
    }
 
    template<typename Functor> void post_order(Functor f) const noexcept
    {
        return post_order(f, root);
    }
 
    template<typename Functor> void pre_order(Functor f) const noexcept
    {
        return pre_order(f, root);
    }
  
    template<typename Functor> void breath_first(Functor f) const noexcept;

    //void breath_first();

    size_t height();

    Node* find(const T&);
    
    std::ostream& print(std::ostream& ostr) const noexcept
    {
        std::cout << "tree::size = " << size << ". contents = { ";

        in_order([](const auto& x) { std::cout << x << ", " << std::flush; });
        
        std::cout << "} " << std::endl;
        return ostr;
    }
    
    friend std::ostream& operator<<(std::ostream& ostr, const sbstree& tree)
    {
        return tree.print(ostr);
    }
};

template<class T> std::ostream& sbstree<T>::Node::debug_print(std::ostream& ostr) const noexcept
{
   ostr << " {["; 
 
   ostr << key;
 
   ostr << "]:parent=" << parent << ", this=" << this;
 
   ostr << ", left=";
   
   if (!left) 
     ostr << "nullptr";
   else
      ostr << left.get();
   
   ostr << ", right=";
   
   if (!right) 
     ostr << "nullptr";
   else
      ostr << right.get();
   
   ostr << "}";
 
   return ostr;
}


template<typename T> sbstree<T>::Node::Node(const typename sbstree<T>::Node& lhs) noexcept : key{lhs.key}, left{nullptr}, right{nullptr}
{
   if (lhs.parent == nullptr) // If we are copying a root pointer, then set parent.
       parent = nullptr;

   // The make_shared<Node> calls below results in the entire tree rooted at lhs being copied.
   if (lhs.left  != nullptr) { 

       left = std::make_shared<Node>(*lhs.left);    
       left->parent = this;
   }
   
   if (lhs.right != nullptr) {

       right = std::make_shared<Node>(*lhs.right); 
       right->parent = this;
   }
}

template<typename T> typename sbstree<T>::Node& sbstree<T>::Node::operator=(const typename sbstree<T>::Node& lhs) noexcept
{
   if (&lhs == this) return *this;

   key = lhs.key;

   if (lhs.parent == nullptr) // If we are copying the root node, then set parent.
       parent = nullptr;

   // The make_shared<Node> calls below creates a copy of the entire tree at lhs.root
   if (lhs.left  != nullptr) { 

       left = std::make_shared<Node>(*lhs.left);    
       left->parent = this;
   }
   
   if (lhs.right != nullptr) {

       right = std::make_shared<Node>(*lhs.right); 
       right->parent = this;
   }
  
   return *this;
}

template<typename T> sbstree<T>::sbstree(const sbstree& lhs)
{
   // This will invoke Node(const Node&), passing *lhs.root, which will duplicate the entire tree rooted at lhs.root.
   root = std::make_unique<Node>(*lhs.root); 
   size = lhs.size;
}

template<typename T> bool sbstree<T>::insert(const T& x) noexcept
{
  if (!root) {
     root = std::make_shared<Node>(x);     
     ++size;
     return true;
  } 
  else {

     auto rc = insert(x, root);
     if (rc) ++size;
     return rc;
  }
};

template<typename T> bool sbstree<T>::insert(const T& x, std::shared_ptr<Node>& current) noexcept
{
  if (x < current->key) {

       if (!current->left) 
            current->left =  std::make_shared<Node>(x, current.get());
       else 
           insert(x, current->left);
   
  } else if (x > current->key) {

        if (!current->right) { 
            current->right = std::make_shared<Node>(x, current.get());
        }
        else
            insert(x, current->right);

  } else if (x == current->key) 
         return false; 
  
  return true;
}

/*
 * Returns true if found and removed, false if not found

bool sbstree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
{
   // If p is not nullptr and... 
   // ...if its key is less than current node and we still have nodes to search 
   if (p && x < p->key) 
      return remove(x, p->left);

   // ...else if its key is greater than current node and we still have nodes to search  
   else if (p && x > p->key)
      return remove(x, p->right);

   // ...else we found the key
   else if (p && p->key == x) { 

       // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...

       // ...If p doesn't have a left child, then...
       if (!p->left) 

           // ...remove p by replacing it with right child
           p = p->right; 

       // ...else If p doesn't have a right child, then...
       else if (!p->right) 

            // ...remove p by replacing it with left child
            p = p->left; 
       
       // 2. Else if p has two children (ttat aren't nullptr). Swap the found key with its in-order predecessor

       else { // p is an internal node with two children. 

         std::shared_ptr<Node> q = p->left; // Note: This line not possible with unique_ptr

         while (q->right != nullptr) // locate in-order predecessor
                q = q->right;

          p->key = q->key; // Swap its key with p's key and...

          remove(q->key, p->left); // delete the swapped key, which is x. Start searching for x at p->left,
                                   // the root of the in-order predessor.  
       }
       return true;
   }
   return false;
}
 */
template<typename T> bool sbstree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
{
   // If p is not nullptr and... 
   // ...if its key is less than current node and we still have nodes to search 
   if (p && x < p->key) 
      return remove(x, p->left);

   // ...else if its key is greater than current node and we still have nodes to search  
   else if (p && x > p->key)
      return remove(x, p->right);

   // ...else we found the key
   else if (p && p->key == x) { 

       // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...

       if (!p->left) 

           // ...remove p by replacing it with right child
           p = p->right; 

       // ...else if p doesn't have a right child, then...
       else if (!p->right) 

            // ...remove p by replacing it with left child
            p = p->left; 
       
       // 2. Else if p has two non-nullptr children, swap x with its in-order predecessor

       else { 

         std::shared_ptr<Node> q = p->right; // Note: This line not possible with unique_ptr

         while (q->left != nullptr) // locate in-order successor leaf node.
                q = q->left;

          p->key = q->key; // Set in-order successor p's key and...

          remove(q->key, p->right); // ...now delete the swapped key, x. Start searching for x at p->left,
                                   // the root node of the in-order predessor.  
       }
       return true;
   }
   // Could not find x in p or any of its children
   return false;
}

template<typename T>
template<typename Functor> void sbstree<T>::in_order(Functor f, const std::shared_ptr<Node>& current) const noexcept 
{
   if (current == nullptr) {

      return;
   }

   in_order(f, current->left);

   f(current->key); 

   in_order(f, current->right);
}

template<typename T>
template<typename Functor> void sbstree<T>::pre_order(Functor f, const std::shared_ptr<Node>& current) const noexcept 
{
   if (current == nullptr) {

      return;
   }

   f(current->key); 
   pre_order(f, current->left);
   pre_order(f, current->right);
}

template<typename T>
template<typename Functor> void sbstree<T>::post_order(Functor f, const std::shared_ptr<Node>& current) const noexcept 
{
   if (current == nullptr) {

      return;
   }

   post_order(f, current->left);
   post_order(f, current->right);

   f(current->key); 
}

template<typename T> inline void  sbstree<T>::printlevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(*this, &Node::print, ostr);  
  
  breath_first(tree_printer);
  
  std::cout << std::endl;
}

template<typename T> void sbstree<T>::debug_printlevelOrder(std::ostream& ostr) const noexcept
{
  NodeLevelOrderPrinter tree_printer(*this, &Node::debug_print, ostr);  
  
  breath_first(tree_printer);
  
  ostr << std::flush;
}

template<typename T> std::size_t sbstree<T>::height(const std::shared_ptr<Node>& current) const noexcept
{
  // From: algorithmsandme.com/level-order-traversal-of-binary-tree
  if (!current) return 0;
 
  int lh = height(current->left);
  int rh = height(current->right);
 
  return 1 + std::max(lh, rh);
}

template<typename T> template<typename Functor> void sbstree<T>::breath_first(Functor f) const noexcept
{
   std::queue< std::pair<const Node*, int> > queue; 

   const Node* proot = root.get();

   if (!proot) return;
      
   auto initial_level = 1; // initial, top root level is 1.
   
   // 1. pair.first  is: const bstree<T>::Node*, the current node to visit.
   // 2. pair.second is: current level of tree.
   queue.push(std::make_pair(proot, initial_level));

   /*
    * TODO: I think this code assumes a balanced tree.
    * We may need to use the tree height instead of isLeaf()
    */ 
   
   while (!queue.empty()) {

       /*
        std::pair<const Node *, int> pair_ = queue.front();
        const Node *current = pair_.first;
        int current_level = pair_.second;
       */

        auto[current, current_level] = queue.front(); 

        f(current, current_level);  

        if(current->left)
            queue.push(std::make_pair(current->left.get(), current_level + 1));  

        if(current->right)
            queue.push(std::make_pair(current->right.get(), current_level + 1));  

        queue.pop(); 
   }

}

#endif
