template<typenameT> class sbstree {

    struct Node{T key;
        Node* left;
        Node* right;
        Node();
        Node(T);
    };


   Node *root; // shared_ptr is better--right?

 public:

    sbstree() : root{nullptr}
    {
    } 

    ~bstree() = default;

    sbstree(const sbstree&);

    sbstree(sbstree&&);

    sbstree& operator=(const sbstree& lhs);

    sbstree& operator=(sbstree&& lhs);

    bool insert(const T&);

    bool remove(const T&);

    template<typename Functor> inorder(Functor f); 

    template<typename Functor> preorder(Functor f);

    template<typename Functor> postorder(Functor f);

    void breath_first(void(*)(pNode&));

    size_t height();

    Node* find(const T&);
};
/*
 * Returns true if found and removed, false if nt found
 */
template<typename T> bool sbstree<T>::remove(const T& x, std::shared_ptr<Node>& p) 
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
