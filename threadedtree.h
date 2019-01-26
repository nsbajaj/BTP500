template <class T>
class ThreadedTree{
    struct Node{
        T data_;
        Node* left_;
        Node* right_;
        bool leftThread_;
        bool rightThread_;
        Node(const T& data=T{},Node* left=nullptr, Node* right=nullptr){
            data_ = data;
            left_ = left;
            right_ = right;
            leftThread_ = false;
            rightThread_ = false;
        }
    };
    Node* root_;
    
public:
    class const_iterator{
        friend class ThreadedTree;
        protected:
            Node* curr_;
            const ThreadedTree* theTree_;
            const_iterator(Node* curr, const ThreadedTree* theTree){
                theTree_ = theTree;
                curr_ = curr;
            }
        public:
            const_iterator(){
                theTree_ = nullptr;
                curr_ = nullptr;
            }
        
            //++x: prefix
            /*
             Iterates to the next node in the tree. If the next node is null, returns nullptr.
             */
            const_iterator operator++(){
                if(this->curr_->rightThread_ == false){
                    if(this->curr_->right_ != nullptr){
                        this->curr_ = this->curr_->right_;
                        
                        while(this->curr_->left_ != nullptr && this->curr_->leftThread_ == false){
                            this->curr_ = this->curr_->left_;
                        }
                    }
                    //End of the tree
                    else{
                        this->curr_ = nullptr;
                    }
                }
                else if(this->curr_->rightThread_ == true){
                    this->curr_ = this->curr_->right_;
                }
                return *this;
            }
        
            //x++: postfix
            /*
             Iterates to the next node in the tree. If the next node is null, returns old value.
             */
            const_iterator operator++(int){
                const_iterator old = *this;
                if(this->curr_->rightThread_ == false){
                    if(this->curr_->right_ != nullptr){
                        this->curr_ = this->curr_->right_;
                        
                        while(this->curr_->left_ != nullptr && this->curr_->leftThread_ == false){
                            this->curr_ = this->curr_->left_;
                        }
                    }
                    //End of the tree
                    else{
                        this->curr_ = nullptr;
                    }
                }
                else if(this->curr_->rightThread_ == true){
                    this->curr_ = this->curr_->right_;
                }
                return old;
            }
         
            //--x: prefix
            /*
             Iterates to the previous node in the tree. If the next node is null, it iterates to the most right node in the tree (node before nullptr).
             */
            const_iterator operator--(){
                //At the end of the tree
                if(this->curr_ == nullptr){
                    Node* curr = this->theTree_->root_;
                    if(curr != nullptr){
                        while(curr->right_ != nullptr){
                            curr = curr->right_;
                        }
                        this->curr_ = curr;
                    }
                }
                else{
                    if(this->curr_->leftThread_ == true){
                        this->curr_ = this->curr_->left_;
                    }
                    else{
                        if(this->curr_->left_ != nullptr){
                            this->curr_ = this->curr_->left_;
                            while(this->curr_->right_ != nullptr && this->curr_->rightThread_ == false){
                                    this->curr_ = this->curr_->right_;
                            }
                        }
                    }
                }
                return *this;
            }
         
            //x--: postfix
            /*
             Iterates to the previous node in the tree. If the next node is null, it iterates to the most right node in the tree (node before nullptr) and returns the old value.
             */
            const_iterator operator--(int){
                const_iterator old = *this;
                //At the end of the tree
                if(this->curr_ == nullptr){
                    Node* curr = this->theTree_->root_;
                    if(curr != nullptr){
                        while(curr->right_ != nullptr){
                            curr = curr->right_;
                        }
                        this->curr_ = curr;
                    }
                }
                else{
                    if(this->curr_->leftThread_ == true){
                        this->curr_ = this->curr_->left_;
                    }
                    else{
                        if(this->curr_->left_ != nullptr){
                            this->curr_ = this->curr_->left_;
                            while(this->curr_->right_ != nullptr && this->curr_->rightThread_ == false){
                                this->curr_ = this->curr_->right_;
                            }
                        }
                    }
                }
                return old;
            }
        
            //Returns a const reference to data in the node referred to by the iterator.
            const T& operator*() const{
                return this->curr_->data_;
            }
        
            //Returns true if rhs and current object refer to the same node.
            bool operator==(const const_iterator& rhs) const{
                bool check = false;
                if(theTree_ == rhs.theTree_ && curr_ == rhs.curr_){
                    check = true;
                }
                return check;
            }
        
            //Returns true if rhs and current object does not refer to the same node
            bool operator!=(const const_iterator& rhs) const{
                return !(*this == rhs);
            }
    };
    class iterator:public const_iterator{
        friend class ThreadedTree;
        protected:
            iterator(Node* curr,ThreadedTree* theTree):const_iterator(curr,theTree){}
        public:
            iterator():const_iterator(){
                this->theTree_ = nullptr;
                this->curr_ = nullptr;
            }
        
            //Returns a const reference to data in the node referred to by the iterator.
            const T& operator*() const{
                return this->curr_->data_;
            }
        
            //Returns a reference to data in the node referred to by the iterator.
            T& operator*(){
                return this->curr_->data_;
            }
        
            //++x: prefix
            /*
             Iterates to the previous node in the tree. If the next node is null, it iterates to the most right node in the tree (node before nullptr).
             */
            iterator operator++(){
                if(this->curr_->rightThread_ == false){
                    if(this->curr_->right_ != nullptr){
                        this->curr_ = this->curr_->right_;
                        
                        while(this->curr_->left_ != nullptr && this->curr_->leftThread_ == false){
                            this->curr_ = this->curr_->left_;
                        }
                    }
                    //End of the tree
                    else{
                        this->curr_ = nullptr;
                    }
                }
                else if(this->curr_->rightThread_ == true){
                    this->curr_ = this->curr_->right_;
                }
                return *this;
            }
        
            //x++: postfix
            /*
             Iterates to the next node in the tree. If the next node is null, returns old value.
             */
            iterator operator++(int){
                iterator old = *this;
                if(this->curr_->rightThread_ == false){
                    if(this->curr_->right_ != nullptr){
                        this->curr_ = this->curr_->right_;
                        
                        while(this->curr_->left_ != nullptr && this->curr_->leftThread_ == false){
                            this->curr_ = this->curr_->left_;
                        }
                    }
                    //End of the tree
                    else{
                        this->curr_ = nullptr;
                    }
                }
                else if(this->curr_->rightThread_ == true){
                    this->curr_ = this->curr_->right_;
                }
                return old;
            }
        
            //--x: prefix
            /*
             Iterates to the previous node in the tree. If the next node is null, it iterates to the most right node in the tree (node before nullptr).
             */
            iterator operator--(){
                //At the end of the tree
                if(this->curr_ == nullptr){
                    Node* curr = this->theTree_->root_;
                    if(curr != nullptr){
                        while(curr->right_ != nullptr){
                            curr = curr->right_;
                        }
                        this->curr_ = curr;
                    }
                }
                else{
                    if(this->curr_->leftThread_ == true){
                        this->curr_ = this->curr_->left_;
                    }
                    else{
                        if(this->curr_->left_ != nullptr){
                            this->curr_ = this->curr_->left_;
                            while(this->curr_->right_ != nullptr && this->curr_->rightThread_ == false){
                                this->curr_ = this->curr_->right_;
                            }
                        }
                    }
                }
                return *this;
            }
        
            //x--: postfix
            /*
             Iterates to the previous node in the tree. If the next node is null, it iterates to the most right node in the tree (node before nullptr) and returns the old value.
             */
            iterator operator--(int){
                iterator old = *this;
                //At the end of the tree
                if(this->curr_ == nullptr){
                    Node* curr = this->theTree_->root_;
                    if(curr != nullptr){
                        while(curr->right_ != nullptr){
                            curr = curr->right_;
                        }
                        this->curr_ = curr;
                    }
                }
                else{
                    if(this->curr_->leftThread_ == true){
                        this->curr_ = this->curr_->left_;
                    }
                    else{
                        if(this->curr_->left_ != nullptr){
                            this->curr_ = this->curr_->left_;
                            while(this->curr_->right_ != nullptr && this->curr_->rightThread_ == false){
                                this->curr_ = this->curr_->right_;
                            }
                        }
                    }
                }
                return old;
            }
    };
    
    /*
     Constructor.
     */
    ThreadedTree(){
        this->root_ = nullptr;
    }

    /*
     insert.
     This function is passed in a data/value to insert into the threade binary search tree. It inserts the node where it belongs, and sets it's Predecessor and Successor.
     */
    void insert(const T& data){
        if(root_ == nullptr){
            root_ = new Node(data);
        }
        else{
            Node* curr = root_;
            while(curr != nullptr){
                if(data < curr->data_){
                    //go left
                    if(curr->left_ != nullptr){
                        //If thread node, stop
                        if(curr->leftThread_ == true){
                            curr->leftThread_ = false;
                            curr->left_ = new Node(data);
                            curr = curr->left_;
                            break;
                        }
                        else{
                            curr=curr->left_;
                        }
                    }
                    else{
                        curr->left_ = new Node(data);
                        curr = curr->left_;
                        break;
                    }
                }
                else{
                    //go right
                    if(curr->right_ != nullptr){
                        //If thread node, stop
                        if(curr->rightThread_ == true){
                            curr->rightThread_ = false;
                            
                            curr->right_ = new Node(data);
                            curr = curr->right_;
                            
                            break;
                        }
                        //Else, keep going
                        else{
                            curr=curr->right_;
                        }
                    }
                    //Can't go right any further.
                    else{
                        curr->right_ = new Node(data);
                        curr = curr->right_;
                        break;
                    }
                }
            }
            Node* pre = findPredecessor(curr->data_);
            Node* suc = findSuccessor(curr->data_);
            if(pre != nullptr){
                curr->left_ = pre;
                curr->leftThread_ = true;
            }
            else{
                curr->left_ = nullptr;
                curr->leftThread_ = false;
            }
            
            if(suc != nullptr){
                curr->right_ = suc;
                curr->rightThread_ = true;
            }
            else{
                curr->right_ = nullptr;
                curr->rightThread_ = false;
            }
        }
    }
    
    /*
     findPredecessor.
     This function is passed a value/data of a node, and it searches through the tree and returns the Predecessor to the node/value. The predecessor is the node with the highest/maximum value in left subtree of the node/data passed. If it doesn't exist, it's the parent then.
     */
    Node* findPredecessor(const T& data){
        Node* predecessor = nullptr;
        Node* curr = root_;
        
        while(curr != nullptr){
            if(curr->data_ == data){
                if(curr->left_ != nullptr){
                    Node* temp = curr->left_;
                    while(temp->right_ != nullptr){
                        temp = temp->right_;
                    }
                    predecessor = temp;
                }
                break;
            }
            else if(curr->data_ < data){
                predecessor = curr;
                curr = curr->right_;
            }
            else if(curr->data_ > data){
                curr = curr->left_;
            }
        }
        return predecessor;
    }
    
    /*
     findSuccessor.
     This function is passed a value/data of a node, and it searches through the tree and returns the Successor to the node/value. The successor is the node with the lowest/minimum value in right subtree of the node/data passed. If it doesn't exist, it's the parent then.
     */
    Node* findSuccessor(const T& data){
        Node* successor = nullptr;
        Node* curr = root_;
        while (curr != nullptr){
            if(curr->data_ == data){
                if (curr->right_ != nullptr){
                    Node* temp = curr->right_;
                    while(temp->left_){
                        temp = temp->left_;
                    }
                    successor = temp;
                }
                break;
            }
            else if(curr->data_ > data){
                successor = curr;
                curr = curr->left_;
            }
            else if(curr->data_ < data){
                curr = curr->right_;
            }
        }
        return successor;
    }
    
    /*
     find.
     This function accepts a data/value, which represts the node to be found. If found, it returns an iterator which holds the node, else end()/nullptr.
     */
    iterator find(const T& data){
        iterator it = end();
      
        if(root_ != nullptr){
            Node* curr = root_;
            while(curr != nullptr){
                if(curr->data_ == data){
                    it = iterator(curr, this);
                    break;
                }
                else if(data < curr->data_){
                    //go left
                    if(curr->left_){
                        curr = curr->left_;
                    }
                }
                else{
                    //go right
                    if(curr->right_){
                        curr = curr->right_;
                    }
                }
            }
        }
        return it;
    }
    
    /*
     find.
     This function accepts a data/value, which represts the node to be found. If found, it returns a const_iterator which holds the node, else end()/nullptr.
     */
    const_iterator find(const T& data) const{
        const_iterator it = cend();
        
        if(root_ != nullptr){
            Node* curr = root_;
            while(curr != nullptr){
                if(curr->data_ == data){
                    it = const_iterator(curr, this);
                    break;
                }
                else if(data < curr->data_){
                    //go left
                    if(curr->left_){
                        curr = curr->left_;
                    }
                }
                else{
                    //go right
                    if(curr->right_){
                        curr = curr->right_;
                    }
                }
            }
        }
        return it;
    }
    
    /*
     begin.
     This function returns an iterator to the smallest node in the tree.
     */
    iterator begin(){
        Node* curr = nullptr;
        if(root_ != nullptr){
            //Only root Node, no children
            if(root_->left_ == nullptr && root_->right_ == nullptr){
                curr = root_;
            }
            else if(root_->left_ != nullptr){
                curr = root_->left_;
                while(curr->left_ != nullptr){
                    curr = curr->left_;
                }
            }
        }
        return iterator(curr, this);
    }
    
    /*
     end.
     This function returns an iterator to the node, just past the last node (nullptr).
     */
    iterator end(){
        return iterator(nullptr, this);
    }
    
    /*
     cbegin.
     This function returns a const_iterator to the smallest node in the tree.
     */
    const_iterator cbegin()const{
        Node* curr = nullptr;
        if(root_ != nullptr){
            //Only root Node, no children
            if(root_->left_ == nullptr && root_->right_ == nullptr){
                curr = root_;
            }
            else if(root_->left_ != nullptr){
                curr = root_->left_;
                while(curr->left_ != nullptr){
                    curr = curr->left_;
                }
            }
        }
        return const_iterator(curr, this);
    }
    
    /*
     cend.
     This function returns a const_iterator to the node, just past the last node (nullptr).
     */
    const_iterator cend() const{
        return const_iterator(nullptr, this);
    }
    
    /*
     Destructor.
     Calls the remove function to recursively delete the nodes in the tree, using post order traversal and sets the root_ as nullptr.
     */
    ~ThreadedTree(){
        remove(root_);
        root_ = nullptr;
    }
    
    /*
     Remove.
     This function is called recursively to delete the nodes in the tree, in post order traversal.
     */
    void remove(Node* curr){
        if(curr != nullptr){
            if(curr->leftThread_ == false){
                remove(curr->left_);
            }
            if(curr->rightThread_ == false){
                remove(curr->right_);
            }
            delete curr;
        }
    }
};

