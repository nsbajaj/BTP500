#include <iostream>
#include <iomanip>

using namespace std;

/*forward declaration*/
template <class T>
class Queue;

template <class T>
class BST{
    struct Node{
        T data_;
        Node* left_;
        Node* right_;
        Node(const T& data,Node* lt=nullptr,Node* rt=nullptr){
            data_=data;
            left_=lt;
            right_=rt;
        }
    };
    Node* root_;
    
    void copy(Node* node){
        if(node != nullptr){
            insert(node->data_);
            copy(node->left_);
            copy(node->right_);
        }
    }
    
    Node* detachSmallest(Node*& subroot){
    	Node* temp = subroot;
	while(temp != nullptr){
		temp = temp->left_;
	}
	return temp;
    }

    void printPreOrder(Node* subroot)const{
        if(subroot){
            std::cout << subroot->data_ << " ";
            printPreOrder(subroot->left_);
            printPreOrder(subroot->right_);
        }
    }
    void printInOrder(Node* subroot)const{
        if(subroot){
            printInOrder(subroot->left_);
            std::cout << subroot->data_ << " ";
            printInOrder(subroot->right_);
        }
    }
    void destroy(Node* subroot){
        if(subroot){
            destroy(subroot->left_);
            destroy(subroot->right_);
            delete subroot;
        }
    }
    
    bool isSame(const Node* left, const Node* right) const;
    
    /*used by print() to print all nodes at same level*/
    void printLine(Node* data[],int numNodes,int width) const{
        int half=width/2;
        int firsthalf=width%2?half+1:half;
        
        if(numNodes > 1){
            for(int i=0;i<numNodes;i++){
                if(i%2==0){
                    if(data[i]){
                        std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< "-";
                        std::cout << std::left << std::setfill('-') << std::setw(half) << "-";
                    }
                    else{
                        std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< " ";
                        std::cout << std::left << std::setfill(' ') << std::setw(half) << " ";
                    }
                }
                else{
                    if(data[i]){
                        std::cout << std::right << std::setfill('-') << std::setw(firsthalf) << "-";
                        std::cout << std::left <<std::setfill(' ') << std::setw(half)<<"-";
                    }
                    else{
                        std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
                        std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
                    }
                }
            }
            std::cout << std::endl;
        }
        for(int i=0;i<numNodes;i++){
            if(data[i]){
                if(i%2==0){
                    std::cout << std::right << std::setw(firsthalf) << "|";
                    std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
                }
                else{
                    std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
                    std::cout << std::left <<std::setfill(' ') << std::setw(half)<< "|";
                }
            }
            else{
                std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";
            }
        }
        std::cout << std::endl;
        for(int i=0;i<numNodes;i++){
            if(data[i]){
                if(i%2==0){
                    std::cout << std::right << std::setw(firsthalf) << data[i]->data_;
                    std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
                }
                else{
                    std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
                    std::cout << std::left <<std::setfill(' ') << std::setw(half)<< data[i]->data_;
                }
            }
            else{
                std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";
            }
        }
        std::cout << std::endl;
    }

public:
    BST(){
        root_=nullptr;
    }
    
    //Copy Constructor -
    BST(const BST& rhs){
        if(this != &rhs){
            if(rhs.root_ != nullptr){
                root_ = nullptr;
                copy(rhs.root_);
            }
        }
    }

    void remove(const T& data){
    	if(root_ != nullptr){
            Node* curr = root_;
            Node* prev = nullptr; //Parent of node to be found
            while(curr != nullptr){
                if(data == curr->data_){
                    break;
                }
                else if(data < curr->data_){
                    if(curr->left_ != nullptr){
                        prev = curr;
                        curr = curr->left_;
                    }
                }
                else if(data > curr->data_){
                    if(curr->right_ != nullptr){
                        prev = curr;
                        curr = curr->right_;
                    }
                }
            }
            //if found
            if(curr != nullptr){
                //Leaf Node
                if(curr->left_ == nullptr && curr->right_ == nullptr){
                    //Root node is the node to be deleted and is the only node in the tree
                    if(curr == root_){
                        root_ = nullptr;
                        delete curr;
                    }
                    else{
                        if(prev != nullptr){
                            //If the parent of curr has a child curr
                            if(prev->left_ == curr){
                                prev->left_ = nullptr;
                                delete curr;
                            }
                            else if(prev->right_ == curr){
                                prev->right_ = nullptr;
                                delete curr;
                            }
                        }
                    }
                }
                //1 child
                else if((curr->left_ != nullptr && curr->right_ == nullptr) || (curr->right_ != nullptr && curr->left_ == nullptr)){
                    Node* temp = nullptr;
                    if(curr->left_ != nullptr && curr->right_ == nullptr){
                        temp = curr->left_;
                    }
                    else if(curr->right_ != nullptr && curr->left_ == nullptr){
                        temp = curr->right_;
                    }
                    if(curr == root_){
                        root_ = temp;
                    }
                    else{
                        if(prev->left_ == curr){
                            prev->left_ = temp;
                            delete curr;
                        }
                        else if(prev->right_ == curr){
                            prev->right_ = temp;
                            delete curr;
                        }
                    }
                }
                //2 child
                else{
                    Node* temp = curr->right_;
                    Node* tempPrev = nullptr;
                    while(temp->left_ != nullptr){
                        tempPrev = temp;
                        temp = temp->left_;
                    }
                    Node* rightSub = nullptr;
                    if(temp != nullptr){
                        rightSub = temp->right_;
                        if(prev != nullptr && prev->right_ == curr){
                            prev->right_ = temp;
                            temp->left_ = curr->left_;
                            temp->right_ = curr->right_;
                            tempPrev->left_ = rightSub;
                        }
                        if(prev != nullptr && prev->left_ == curr){
                            prev->left_ = temp;
                            temp->left_ = curr->left_;
                        }
                        //Node to be removed is root
                        if(curr == root_){
                            tempPrev->left_ = rightSub;
                            temp->right_ = root_->right_;
                            temp->left_ = root_->left_;
                            root_ = temp;
                        }
                        delete curr;
                    }
                }
            }
        }
    }
 
    int depth(const T& data){
        int depth = -1;
        
        if(root_ != nullptr){
            if(root_->data_ == data){
                depth = 0;
            }
            else{
                depth++;
                Node* curr = root_;
                while(curr != nullptr){
                    if(data == curr->data_){
                        return depth;
                    }
                    else if(data < curr->data_){
                        if(curr->left_ != nullptr){
                            curr = curr->left_;
                            depth++;
                        }
                        //If less than left but null (not found/can't go left any further).
                        else{
                            depth = -1;
                            return depth;
                        }
                    }
                    else if(data > curr->data_){
                        if(curr->right_ != nullptr){
                            curr = curr->right_;
                            depth++;
                        }
                        //If less than right but null (not found/can't go right any further).
                        else{
                            depth = -1;
                            return depth;
                        }
                    }
                }
            }
        }
        return depth;
    }
    
    void printPreOrder() const{
        printPreOrder(root_);
        std::cout << std::endl;
    }
    void printInOrder() const{
        printInOrder(root_);
        std::cout << std::endl;
    }
    
    void insert(const T& data){
        if(root_==nullptr){
            root_=new Node(data);
        }
        else{
            Node* curr=root_;
            while(curr != nullptr){
                if(data < curr->data_){
                    //go left
                    if(curr->left_){
                        curr=curr->left_;
                    }
                    else{
                        curr->left_=new Node(data);
                        curr=nullptr;
                    }
                }
                else{
                    //go right
                    if(curr->right_){
                        curr=curr->right_;
                    }
                    else{
                        curr->right_=new Node(data);
                        curr=nullptr;
                    }
                }
            }
            
        }
        
    }
    
    bool operator==(const BST& rhs) const;
    void print() const{
        struct Output{
            Node* node_;
            int lvl_;
            int position_;
            Output(Node* n=nullptr,int l=0, int p=0){
                node_=n;
                lvl_=l;
                position_=p;
            }
            void set(Node* n=nullptr,int l=0, int p=0){
                node_=n;
                lvl_=l;
                position_=p;
            }
        };
        Queue<Output> theNodes;
        Node* line[16];
        if(root_){
            for(int i=0;i<16;i++){
                line[i]=nullptr;
            }
            theNodes.enqueue(Output(root_,0,0));
            int currline=0;
            int width=80;
            int numInLine=1;
            while(theNodes.isEmpty()==false){
                Output curr=theNodes.front();
                if(curr.node_->left_)
                    theNodes.enqueue(Output(curr.node_->left_,curr.lvl_+1,curr.position_*2));
                if(curr.node_->right_)
                    theNodes.enqueue(Output(curr.node_->right_,curr.lvl_+1,curr.position_*2+1));
                theNodes.dequeue();
                
                
                if(curr.lvl_>currline){
                    printLine(line,numInLine,width);
                    width=width/2;
                    numInLine=numInLine*2;
                    for(int i=0;i<16;i++){
                        line[i]=nullptr;
                    }
                    currline++;
                    
                }
                line[curr.position_]=curr.node_;
            }
            printLine(line,numInLine,width);
            std::cout << std::endl;
            
        }
        else{
            std::cout << "tree is empty" << std::endl;
        }
    }
    ~BST(){
        destroy(root_);
    }
};

template <class T>
class Queue{
    T* theQueue_;
    int capacity_;
    int used_;
    int front_;
    int back_;
    void grow(){
        T* tmp=new T[capacity_*2];
        int j;
        for(int i=0,j=front_;i<used_;i++,j=(j+1)%capacity_){
            tmp[i]=theQueue_[j];
        }
        delete [] theQueue_;
        theQueue_=tmp;
        capacity_=capacity_*2;
        front_=0;
        back_=used_;
    }
public:
    Queue(){
        theQueue_=new T[50];
        capacity_=50;
        used_=0;
        front_=0;
        back_=0;
    }
    void enqueue(const T& data){
        if(used_==capacity_){
            grow();
        }
        theQueue_[back_]=data;
        back_=(back_+1)%capacity_;
        used_++;
    }
    void dequeue(){
        if(!isEmpty()){
            used_--;
            front_=(front_+1)%capacity_;
        }
    }
    T front() const{
        if(!isEmpty()){
            return theQueue_[front_];
        }
        return T{};
        
    }
    bool isEmpty() const{
        return used_==0;
    }
    int size(){
        return used_;
    }
    void print(){
        if(used_ > 0){
            for(int i = 0; i < used_; i++){
                if(theQueue_[i] != nullptr){
                    cout << theQueue_[i]->data_ << ", ";
                }
            }
        }
    }
    ~Queue(){
        delete [] theQueue_;
    }
};

