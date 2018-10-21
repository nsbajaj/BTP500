#include <iostream>

template <typename T>
class CacheList{
    struct Node{
        T data_; //Node's data variable, used for searching.
        int access_; //Node's access count, used for swapping.
        Node* next_;
        Node* prev_;
        Node(const T& data=T{}, Node* nx=nullptr,Node* pr=nullptr){
            data_ = data;
            next_ = nx;
            prev_ = pr;
            access_ = 0;
        }
    };
    //Sentinel Nodes
    Node* front_;
    Node* back_;
public:
    class const_iterator{
        friend class CacheList<T>;
    protected:
        Node* curr_;
        const CacheList* theList_;
        const_iterator(Node* curr, const CacheList* theList){
            theList_ = theList;
            curr_ = curr;
        }
    public:
        const_iterator(){
            curr_ = nullptr;
            theList_ = nullptr;
        }
        //The iterators below provide the functionality to traverse through the list using iterators.
        //++x: prefix
        const_iterator operator++(){
            curr_=curr_->next_;
            return *this;
        }
        //x++: postfix
        const_iterator operator++(int){
            const_iterator old=*this;
            curr_=curr_->next_;
            return old;
        }
        //--x: prefix
        const_iterator operator--(){
            if(curr_!=nullptr){
                curr_=curr_->prev_;
            }
            else{
                if(theList_){
                    curr_=theList_->back_;
                }
            }
            return *this;
        }
        //x--: postfix
        const_iterator operator--(int){
            const_iterator old=*this;
            curr_=curr_->prev_;
            return old;
        }
        
        /*This operator is passed an rhs object.
          It is checked if the current object and rhs refer to the same node. Returns true if they do, else false.
         */
        bool operator==(const_iterator rhs){
            bool check = false;
            if(theList_ == rhs.theList_ && curr_ == rhs.curr_){
                check = true;
            }
            return check;
        }
        
        /*This operator is passed an rhs object.
          It is checked if the current object and rhs do not refer to the same node. Returns true if they do not.
         */
        bool operator!=(const_iterator rhs){
            return !(*this == rhs);
        }
        //Dereference operator returns a const reference to data in the node referred to by the iterator.
        const T& operator*()const{
            return curr_->data_;
        }
    };
    class iterator:public const_iterator{
        friend class CacheList<T>;
    protected:
        iterator(Node* curr,CacheList* theList):const_iterator(curr,theList){}
    public:
        iterator(){
            this->theList_ = nullptr;
            this->curr_ = nullptr;
        }
        //The iterators below provide the functionality to traverse through the list using iterators.
        //++x: prefix
        iterator operator++(){
            this->curr_ = this->curr_->next_;
            return *this;
        }
        //x++: postfix
        iterator operator++(int){
            iterator old = *this;
            this->curr_ = this->curr_->next_;
            return old;
        }
        //--x: prefix
        iterator operator--(){
            if(this->curr_ != nullptr){
                this->curr_ = this->curr_->prev_;
            }
            else{
                if(this->theList_){
                    this->curr_ = this->theList_->back_;
                }
            }
            return *this;
        }
        //x++: postfix
        iterator operator--(int){
            iterator old = *this;
            this->curr_ = this->curr_->prev_;
            return old;
        }
        //Dereference operator returns a reference to data in the node referred to by the iterator.
        T& operator*(){
            return this->curr_->data_;
        }
        //Dereference operator returns a const reference to data in the node referred to by the iterator.
        const T& operator*() const{
            return this->curr_->data_;
        }
    };
    CacheList();
    ~CacheList();
    CacheList(const CacheList& rhs);
    CacheList& operator=(const CacheList& rhs);
    CacheList(CacheList&& rhs);
    CacheList& operator=(CacheList&& rhs);
    /*The functions begin(), cbegin() return an iterator containing a node, which points to the node after the front sentinel in the list. The functions end(), cend() return an iterator containing a node, which points to the back sentinel. The only difference is between begin(), cbegin(), end(), cend() is that cbegin() and cend() are contants. Their iterators cannot be modified.
    */
    iterator begin(){
        return iterator(front_->next_, this);
    }
    iterator end(){
        return iterator(back_, this);
    }
    const_iterator cbegin() const{
        return const_iterator(front_->next_,this);
    }
    const_iterator cend() const{
        return const_iterator(back_, this);
    }
    void insert(const T& data);
    void insert(const T& data, int access);
    iterator search(const T& data);
    iterator erase(iterator it);
    iterator erase(iterator first, iterator last);
    bool empty() const;
    int size() const;
    void print();
};

/*
 Constructor for the CacheList class.
 Initializes the front_ and back_ (Sentinel) nodes and set's up as an empty linked list.
 */
template <typename T>
CacheList<T>::CacheList(){
    front_=new Node();
    back_=new Node();
    front_->next_=back_;
    back_->prev_=front_;
}

/*
 Destructor for the CacheList class.
 It checks if the list is empty. If not, it calls the erase function and passes the first and last (sentinel) node to empty the linked list.
 */
template <typename T>
CacheList<T>::~CacheList(){
    if(!empty()){
        erase(begin(), end());
    }
}

/*
 Copy Constructor.
 The copy constructor checks if the rhs object being passed in is different than current object.
 If it is different, it initializes the current objects front_ and back_ nodes and links them together.
 It then makes a deep copy of the rhs object.
 */
template <typename T>
CacheList<T>::CacheList(const CacheList& rhs){
    if(this != &rhs && front_ != rhs.front_ && rhs.back_ != back_){
        this->front_ = new Node();
        this->back_ = new Node();
        this->front_->next_ = this->back_;
        this->back_->prev_ = this->front_;
        
        Node* f = rhs.front_->next_;
        while (f != rhs.back_) {
            insert(f->data_, f->access_);
            f = f->next_;
        }
    }
}

/*
 Assignment Operator.
 The assignment operator checks if the rhs object being passed in is different than current object.
 If it is different, it empties the current linked list by using the erase function and passing in the begin() and end() iterators. It then makes a deep copy of the rhs object and returns the a dereferenced pointer of the current object.
 */
template <typename T>
CacheList<T>& CacheList<T>::operator=(const CacheList& rhs){
    if(this != &rhs && front_ != rhs.front_ && rhs.back_ != back_){
        erase(begin(), end());
        
        Node* t = rhs.front_->next_;
        while(t != rhs.back_){
            insert(t->data_);
            t = t->next_;
        }
    }
    return *this;
}

/*
 Move Constructor.
 The Move Constructor checks if the rhs object being passed in is different than current object.
 It points the current object's front_ and back_ nodes to rhs's front_ and back_ (Deep Copy).
 It then points the rhs front_ and back_ to an empty linked list. If rhs is deleted, the list will be lost.
 */
template <typename T>
CacheList<T>::CacheList(CacheList&& rhs){
    if(this != &rhs && front_ != rhs.front_ && rhs.back_ != back_){
        this->front_ = rhs.front_;
        this->back_ = rhs.back_;
        
        CacheList<T> temp;
        rhs.front_ = temp.front_;
        rhs.back_ = temp.back_;
    }
}

/*
 Move operator.
 The Move Operator checks if the rhs object being passed in is different than current object.
 It performs a swap between the current object and rhs's front_ and back_. It then returns a dereferenced pointer of the current object.
 */
template <typename T>
CacheList<T>& CacheList<T>::operator=(CacheList&& rhs){
    if(this != &rhs && front_ != rhs.front_ && rhs.back_ != back_){
        Node* f1 = this->front_;
        Node* l1 = this->back_;
        
        this->front_ = rhs.front_;
        this->back_ = rhs.back_;
        
        rhs.front_ = f1;
        rhs.back_ = l1;
    }
    return *this;
}

/*
 Insert.
 The function is passed a variable "data", type T.
 The function creates a temporary node, with the value of "data", sets the access_ variable count to 0.
 It then pushes the node to the back by assigning the second last node's next as temporary node and last node's prev as temporary node.
 */
template <typename T>
void CacheList<T>::insert(const T& data){
    Node* nn = new Node(data, back_, back_->prev_);
    nn->access_ = 0;
    back_->prev_->next_ = nn;
    back_->prev_ = nn;
}

/*
 Insert.
 The function is passed a variable "data", type T and an access variable.
 The function creates a temporary node, with the value of "data", sets the access_ variable of the node to the access variable being passed in. It then pushes the node to the back by assigning the second last node's next as temporary node and last node's prev as temporary node.
 */
template <typename T>
void CacheList<T>::insert(const T& data, int access){
    Node* nn = new Node(data, back_, back_->prev_);
    nn->access_ = access;
    back_->prev_->next_ = nn;
    back_->prev_ = nn;
}

/*
 Search.
 This function is passed a variable "data", of type T.
 It then creates an iterator and the back_ (end()) node is assigned to it. It checks if the list empty, if not, it begins to loop through the list. If it finds a node that contains the value of "data", the access count of that node is increased by 1, the node is moved closer to front of the list as long as the access count of the node being moved is bigger and equal than the node before it. The equal ensures if there is a node with the same access count, the node being moved to the front will be infront of the node with the same access count. Once the swap has been completed, the iterator is assigned the node that was moved to the front. If a node with the value of "data" is not found, the function returns iterator, which was assigned the value of back_ (end()) at the beginning of the function.
 */
template <typename T>
typename CacheList<T>::iterator CacheList<T>::search(const T& data){
    CacheList<T>::iterator it = end();
    if(!this->empty()){
        Node* curr = front_->next_;
        while(curr != back_){
            if(curr->data_ == data){
                curr->access_++;
                bool check = false;
                do{
                    if((curr->access_ >= curr->prev_->access_) && (curr->prev_ != front_)){ //If current access is bigger and equal to previous node's access && the previous node is different than the front node. If curr->prev_ is the front node, it means curr is already at the front.
                        
                        //Swap
                        curr->prev_->next_ = curr->next_; //2
                        curr->next_ = curr->prev_; //3
                        curr->prev_ = curr->next_->prev_; //4
                        curr->next_->prev_ = curr; //5
                        curr->prev_->next_ = curr; //6
                        curr->next_->next_->prev_ = curr->next_; //7
                    }
                    else{
                        check = true;
                    }
                }while(check == false);
                it = iterator(curr,this); //8
                break;
            }
            curr=curr->next_;
        }
    }
    return it;
}

/*
 Erase.
 This function accepts an iterator, which contains a node.
 The node is extracted from the iterator, the list is checked if it not empty and whether the node being deleted is not the sentinel nodes. If it passes the statement, the node's previous and front get linked, the iterator iterates++ and deletes the node. It then returns an iterator to the node that follows the removed node.
 */
template <typename T>
typename CacheList<T>::iterator CacheList<T>::erase(iterator it){
    Node* nn = it.curr_;
    if((nn != front_ && nn != back_) && (!this->empty())){
        nn->prev_->next_ = nn->next_;
        nn->next_->prev_ = nn->prev_;
        it++;
        delete nn;
    }
    return it;
}

/*
 Erase.
 This function accepts two iterators.
 The nodes are extracted from the iterators and assigne to temporary nodes. The list is checked if it not empty and whether the first iterator's node is not the front/back sentinel nodes. It then links the first iterator's node's previous next as the last iterator's node and the last iterator's node's previous as first iterator's node's previous. It then begins to loop through and delete the list by starting at the node pointing at first iterator until the node before last iterator is holding. The function then returns an iterator to the node that follows the last removed item.
 */
template <typename T>
typename CacheList<T>::iterator CacheList<T>::erase(iterator first, iterator last){
    Node* f = first.curr_;
    Node* l = last.curr_;
    if((f != front_ && f != back_) && (!this->empty())){
        f->prev_->next_ = l;
        l->prev_ = f->prev_;
        while(f != l){
            if(f != front_ && f != back_){
                Node* rm = f;
                f = f->next_;
                delete rm;
            }
        }
    }
    return last;
}

/*
 Empty.
 This function returns true if the front sentinel's next pointing at the back sentinel, meaning the list is empty.
 Else, it returns false.
 */
template <typename T>
bool CacheList<T>::empty() const{
    bool check = false;
    if(front_->next_ == back_){
        check = true;
    }
    return check;
}

/*
 Size.
 This funcion loops through the entire list and returns the size/number of nodes in the linked list (Not counting the sentinel nodes).
 */
template <typename T>
int CacheList<T>::size() const{
    int size = 0;
    Node* curr=front_->next_;
    while(curr!=back_){
        curr=curr->next_;
        size++;
    }
    return size;
}

/*
 Print.
 This function loops through the entire list and prints the data_ of the node as it loops through the list.
 */
template <typename T>
void CacheList<T>::print(){
    Node* curr=front_->next_;
    while(curr!=back_){
        std::cout << curr->data_ << std::endl;
        curr=curr->next_;
    }
}
