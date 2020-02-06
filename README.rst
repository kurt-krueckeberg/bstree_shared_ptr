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
        
In the sbtree class below, in which Node uses ``shared_ptr`` instead, the **remove** method can be implemented recursively using ``std::shared_ptr<Node>&``:

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
    
        Node* find(const T&);
    };
    
The **remove** method

.. code-block:: cpp

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

             // ...now delete the swapped key, x. Start searching for x at p->left,
             // the root node of the in-order predessor.  
             remove(q->key, p->left);            
           }

           return true;
       }
       // Could not find x in p or any of its children
       return false;
    }

cannot be implemented if we had to use ``unique_ptr`` instead because this block

.. code-block:: cpp

      std::shared_ptr<Node> q = p->left; // <-- Error if unique_ptr used instead

      while (q->right != nullptr) // locate in-order predecessor leaf node.
           q = q->right;

      p->key = q->key; // Swap leaf node key with p's key and...

      // ...now delete the swapped key, x. Start searching for x at p->left,
      // the root node of the in-order predessor.  
      remove(q->key, p->left);            
    }

    return true;

would not compile. But with ``shared_ptr`` a clear recursive remove algorithm like that able can easily be implemented.

The complete code is:
