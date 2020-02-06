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

       // Node* left; // std::shared_ptr<Node> left??
        //Node* right;

        std::shared_ptr<Node> left; // std::shared_ptr<Node> left??
        std::shared_ptr<Node> right;

        Node();

        Node(const T& x, Node *parent_in = nullptr): key{x}, parent{parent_in} 
        {
        } 
    };

   bool remove(const T& x, std::shared_ptr<Node>& p); 

   bool insert(const T& x, std::shared_ptr<Node>& p) noexcept;
   
   template<typename Functor> void inorder(Functor f, const std::shared_ptr<Node>& current) const noexcept; 
 
   std::shared_ptr<Node> root; // shared_ptr seemss better than 'Node *root', which the article had.
   std::size_t size;

 public:

    sbtree() : root{nullptr}
    {
    } 

   ~sbtree() = default;

    sbtree(const sbtree&);
    sbtree(const std::initializer_list<T>& list) noexcept
    {
        for (const auto& x : list)
            insert(x);
    }

    sbtree(sbtree&&);

    sbtree& operator=(const sbtree& lhs);

    sbtree& operator=(sbtree&& lhs);
    
    bool insert(const T& x) noexcept;
    
    bool remove(const T& x)
    {
      return remove(x, root); 
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
        this->inorder([](const auto& x) { std::cout << x << ", " << std::flush; });
        
        std::cout << std::endl;
        return ostr;
    }
    
    friend std::ostream& operator<<(std::ostream& ostr, const sbtree& tree)
    {
        return tree.print(ostr);
    }
};

template<typename T> bool sbtree<T>::insert(const T& x) noexcept
{
  if (!root) {
     root = std::make_shared<Node>(x);     
     return true;
  } 
  else
     return insert(x, root);
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
 */
template<typename T> bool sbtree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
{
   // If p is not nullptr and... 
   // ...if its key is less than current node and we still have nodes to search 
   if (p != nullptr && x < p->key) 
      return remove(x, p->left);

   // ...else if its key is greater than current node and we still have nodes to search  
   else if (p != nullptr && x > p->key)
      return remove(x, p->right);

   // ...else we found the key
   else if (p != nullptr && p->key == x) { 

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

         std::shared_ptr<Node> q = p->left;

         while (q->right != nullptr) // locate in-order predecessor
                q = q->right;

          p->key = q->key; // Swap its key with p's key and...

          remove(q->key, p->left); // delete the swapped key, which is x. Start searching at p->left, the root of the in-order predessor.  
       }

       return true;
   }

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
