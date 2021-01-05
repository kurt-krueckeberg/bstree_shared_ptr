/*
https://thesai.org/Downloads/Volume6No3/Paper_9-Implementation_of_Binary_Search_Trees_Via_Smart_Pointers.pdf
and https://opendatastructures.org/ods-cpp/6_1_Basic_Binary_Tree.html
*/


template<typename T> class Tree {

  struct Node { 

     T key;
     std::shared_ptr<Node> left;
     std::shared_ptr<Node> right;

     Node() : key(), left(), right(){}Node(Tx):key(x),left(), right(){}

     Node *parent; // ??
  };

  std::shared_ptr<Node> root;

  int size(const Node& n)
  {
     if (n) return 0;

     return 1 + size(n->left) + size(n->right);
  }

  public:
    Tree() : root() { }
   ~Tree();

    Tree(Tree&&) = default;

    Tree& operator=(Tree&&) = default;

    Tree(const Tree&);

    Tree& operator=(constTree&);

     bool push(T);

     bool remove(T x)
     {
        remove(x, root);
     }    

     template<Functor> void inorder(Functor f);

     std::shared_ptr<Node> find(Tx)
     {
        returnfind(x, root);
     }

     void breath_first();

     int size()
     {
       return size(root);
     }

     int height()
     {
       return height(root);
     }

};

