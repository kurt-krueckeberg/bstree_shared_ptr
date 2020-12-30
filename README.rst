shared_ptr Implementation of Binary Search Tree
-----------------------------------------------

Some recursive binary search tree algorithms cannot easily be implemented when the nest Node class uses ``unique_ptr`` for ``left`` and ``right``:

.. code-block:: cpp

    template<typename T> class sbtree {
        struct Node{
            T key;
            Node *parent;
            std::shared_ptr<Node> left; 
            std::shared_ptr<Node> right;
            Node();
            //..snip
        };
        
In the sbtree class below, in which Node uses ``shared_ptr`` instead, the **remove** method can be implemented recursively using ``std::shared_ptr<Node>&``. The sbtree class looks like this

.. code-block:: cpp

    // Basics of sbtree
    template<typename T> class sbtree {
    
        struct Node{
            T key;
            Node *parent;
    
            std::shared_ptr<Node> left; 
            std::shared_ptr<Node> right;
    
            Node();
    
            Node(const T& x, Node *parent_in = nullptr): key{x}, parent{parent_in} 
            {
            } 
            Node(const Node& lhs); 
            Node(Node&& lhs);     
        };
    
       bool remove(const T& x, std::shared_ptr<Node>& p); 
     
       std::shared_ptr<Node> root; 
       std::size_t size;
       // ...snip
    
     public:
        sbtree() : root{nullptr} {} 
       ~sbtree() = default;
        sbtree(const sbtree& lhs);
        sbtree(const std::initializer_list<T>& list) noexcept;
        sbtree& operator=(const sbtree& lhs);
        sbtree& operator=(sbtree&& lhs);
        
        bool remove(const T& x)
        {
          bool bRc = remove(x, root); 
          if (bRc) --size;
        }
    
        template<typename Functor> void inorder(Functor f) const noexcept;
        template<typename Functor> void preorder(Functor f) const noexcept; 
        template<typename Functor> void postorder(Functor f) const noexcept; 
        size_t height();
        const Node* find(const T&);
    };
    
and the **remove** method is implemented

.. code-block:: cpp

template<typename T> bool sbstree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
{
   // If we are not done--that is, p is not the child of a leaf node (and so equals nullptr)--and p's key is
   // less than current key, recurse the left child.
   if (p && x < p->key) 
      return remove(x, p->left);

   // ...else if we are not done--p is not the child of a leaf node (and so equals nullptr)--and p's key is
   // greater than current key, recurse the right child.
   else if (p && x > p->key)
      return remove(x, p->right);

   // ...else we found the key/node to remove.
   else if (p && p->key == x) { 

       // 1. If p has only one child (that is not nullptr), then we can remove node p immediately...
       // Question: Do you know that the right child is not nullptr, or does that not even matter?

       if (!p->left) // ...if there is no left child...

           // ...replace p with its right child
           p = p->right; 

       // ...else if p has no right child (and it does have a left child) then...
       else if (!p->right) 

            // ...remove p by replacing it with its left child
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

**remove** could not be implemented like this if we had used ``unique_ptr`` instead. This section of its code

.. code-block:: cpp

      std::shared_ptr<Node> q = p->left; // <-- Error if unique_ptr used instead

      while (q->right != nullptr) // locate in-order predecessor leaf node.
           q = q->right;

      p->key = q->key; // Swap leaf node key with p's key and...

      // ...now delete the swapped key, x. Start searching for x at p->left,
      // the root node of the in-order predessor.  
      remove(q->key, p->left);  // Error: p->left would have already been moved from, if it was a unique_ptr.
    }

    return true;

would not compile. But with ``shared_ptr`` a clear recursive remove algorithm like that able can easily be implemented.

The complete code is:

.. code-block:: cpp

    #ifndef sbtree_h
    #define sbtree_h
    #include <memory>
    #include <utility>
    #include <iostream>
    #include <initializer_list>
    
    template<typename T> class sbtree {
    
        struct Node{
            T key;
            Node *parent;
    
            std::shared_ptr<Node> left; 
            std::shared_ptr<Node> right;
    
            Node();
    
            Node(const T& x, Node *parent_in = nullptr): key{x}, parent{parent_in} 
            {
            } 
            Node(const Node& lhs); //TODO	
            Node(Node&& lhs);      //TODO
        };
    
       bool remove(const T& x, std::shared_ptr<Node>& p); 
    
       bool insert(const T& x, std::shared_ptr<Node>& p) noexcept;
    
       void move(sbtree&& lhs) noexcept
       {
           root = std::move(lhs.root)
           size = lhs.size;
           lhs.size = 0;
       }
       
       template<typename Functor> void inorder(Functor f, const std::shared_ptr<Node>& current) const noexcept; 
     
       std::shared_ptr<Node> root; 
       std::size_t size;
    
     public:
    
        sbtree() : root{nullptr}
        {
        } 
    
       ~sbtree() = default;
    
        sbtree(const sbtree& lhs);
    
        sbtree(const std::initializer_list<T>& list) noexcept
        {
            for (const auto& x : list)
                insert(x);
        }
    
        sbtree(sbtree&& lhs)
        {
          move(std::move(lhs));
        }
    
        sbtree& operator=(const sbtree& lhs);
    
        sbtree& operator=(sbtree&& lhs);
        
        bool insert(const T& x) noexcept;
        
        bool remove(const T& x)
        {
          bool bRc = remove(x, root); 
          if (bRc) --size;
        }
    
        template<typename Functor> void inorder(Functor f) const noexcept
        {
            return inorder(f, root);
        }
        
        template<typename Functor> void preorder(Functor f) const noexcept; 
    
        template<typename Functor> void postorder(Functor f) const noexcept; 
    
        //void breath_first();
    
        size_t height();
    
        Node* find(const T&);
        
        std::ostream& print(std::ostream& ostr) const noexcept
        {
            inorder([](const auto& x) { std::cout << x << ", " << std::flush; });
            
            std::cout << std::endl;
            return ostr;
        }
        
        friend std::ostream& operator<<(std::ostream& ostr, const sbtree& tree)
        {
            return tree.print(ostr);
        }
    };
    
    
    template<typename T> sbtree<T>::sbtree(const sbtree& lhs)
    {
       // This will invoke Node(const Node&), passing *lhs.root, which will duplicate the entire tree rooted at lhs.root.
       root = std::make_unique<Node>(*lhs.root); 
       size_ = lhs.size_;
    }
    
    template<typename T> bool sbtree<T>::insert(const T& x) noexcept
    {
      if (!root) {
         root = std::make_shared<Node>(x);     
         ++size;
         return true;
      } 
      else {
         auto bRc = insert(x, root);
         if (bRc) ++size;
         return bRc;
      }
    };
    
    template<typename T> bool sbtree<T>::insert(const T& x, std::shared_ptr<Node>& current) noexcept
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
    
    bool sbtree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
    {
    
       // If p is not nullptr and... 
       // ...if its key is less than current node and we still have nodes to search 
       if (!p && x < p->key) 
          return remove(x, p->left);
    
       // ...else if its key is greater than current node and we still have nodes to search  
       else if (!p && x > p->key)
          return remove(x, p->right);
    
       // ...else we found the key
       else if (!p && p->key == x) { 
    
           // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...
    
           // ...If p doesn't have a left child, then...
           if (p->left == nullptr) 
    
               // ...remove p by replacing it with right child
               p = p->right; 
    
           // ...esle If p doesn't have a right child, then...
           else if (p->right == nullptr) 
    
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
    template<typename T> bool sbtree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
    {
       // If p is not nullptr and... 
       // ...if its key is less than current node and we still have nodes to search 
       if (!p && x < p->key) 
          return remove(x, p->left);
    
       // ...else if its key is greater than current node and we still have nodes to search  
       else if (!p && x > p->key)
          return remove(x, p->right);
    
       // ...else we found the key
       else if (!p && p->key == x) { 
    
           // 1. If p has only one child (that is not nullptr), then we can remove node p immediately by...
    
           if (p->left == nullptr) 
    
               // ...remove p by replacing it with right child
               p = p->right; 
    
           // ...else if p doesn't have a right child, then...
           else if (p->right == nullptr) 
    
                // ...remove p by replacing it with left child
                p = p->left; 
           
           // 2. Else if p has two non-nullptr children, swap x with its in-order predecessor
    
           else { 
    
             std::shared_ptr<Node> q = p->left; // Note: This line not possible with unique_ptr
    
             while (q->right != nullptr) // locate in-order predecessor leaf node.
                    q = q->right;
    
              p->key = q->key; // Swap leaf node key with p's key and...
    
              remove(q->key, p->left); // ...now delete the swapped key, x. Start searching for x at p->left,
                                       // the root node of the in-order predessor.  
           }
           return true;
       }
       // Could not find x in p or any of its children
       return false;
    }
    
    template<typename T>
    template<typename Functor> void sbtree<T>::inorder(Functor f, const std::shared_ptr<Node>& current) const noexcept 
    {
       if (current == nullptr) {
    
          return;
       }
    
       inorder(f, current->left);
    
       f(current->key); 
    
       inorder(f, current->right);
    }
    #endif
