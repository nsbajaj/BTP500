#include <iostream>
using namespace std;

template <typename T>
class DList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	DList(){
		front_=nullptr;
		back_=nullptr;
	}
	void push_front(const T& data);
	void push_back(const T& data);
	void pop_front();
	void pop_back();
	void print() const;
	void reversePrint() const;
	~DList();
};


template <typename T>
void DList<T>::push_front(const T& data){
    Node* nn = new Node(data, front_);
    if(front_ != nullptr){
        front_->prev_ = nn;
    }
    else{
        back_ = nn;
    }
    front_ = nn;
}

template <typename T>
void DList<T>::push_back(const T& data){
    Node* nn = new Node(data);
    if(front_ != nullptr){ //If more than one node exists
        back_->next_ = nn; //Making the last node's next point to the new node
        nn->prev_ = back_; //Making the new node's previous point to the old last node
        back_ = nn; //Making the DList's back point to new new node, which is the new back
    }
    else{ //If one node exists
        front_ = nn;
        back_ = nn;
    }
}

template <typename T>
void DList<T>::pop_front(){
    if(front_ != nullptr){
        Node* rm = front_;
        front_ = front_->next_;
        if(front_ != nullptr){ //If more than one node left
            front_->prev_ = nullptr;
        }
        else{ //If one node left
            back_ = nullptr;
        }
        delete rm; //Deletes the thing it's pointing at, not rm
    }
}
template <typename T>
void DList<T>::pop_back(){
    if(back_ != nullptr){
		Node* rm = back_;
		if (front_ == back_) {
			front_ = nullptr;
			back_ = nullptr;
		}
		else {
			back_->prev_->next_ = nullptr;
			back_ = back_->prev_;
		}
		delete rm;
	}
}
template <typename T>
void DList<T>::print() const{
	Node* curr=front_;
	while(curr!=nullptr){
		std::cout << curr->data_ << " ";
		curr=curr->next_;
	}
	std::cout << std::endl;
}
template <typename T>
void DList<T>::reversePrint() const{
	Node* curr=back_;
	while(curr!=nullptr){
		std::cout << curr->data_ << " ";
		curr=curr->prev_;
	}
	std::cout << std::endl;
}
template <typename T>
DList<T>::~DList(){
	Node* curr=front_;
	while(curr){
		Node* rm=curr;
		curr=curr->next_;
		delete rm;
	}
}
template <typename T>
class Sentinel{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	Sentinel(){
		front_=new Node();
		back_=new Node();
		front_->next_=back_;
		back_->prev_=front_;
	}
	void push_front(const T& data);
	void push_back(const T& data);
	void pop_front();
	void pop_back();
	void print() const;
	void reversePrint() const;
	~Sentinel();
};

template <typename T>
void Sentinel<T>::push_front(const T& data){
    Node* nn = new Node(data, front_->next_, front_);
    front_->next_->prev_ = nn;
    front_->next_ = nn;
}
template <typename T>
void Sentinel<T>::push_back(const T& data){
	Node* nn = new Node(data, back_, back_->prev_);
	back_->prev_->next_ = nn;
	back_->prev_ = nn;
}
template <typename T>
void Sentinel<T>::pop_front(){
	if(front_->next_ == back_){
		//cout << ("empty list") << endl;
	}
	else{
		//cout << "Not empty" << endl;
		Node* rm = front_->next_;
		front_->next_ = rm->next_;
		front_->next_->prev_ = front_; 
		delete rm;	
	}
}
template <typename T>
void Sentinel<T>::pop_back(){
	if(front_->next_ == back_){
		//cout << "Empty List" << endl;
	}
	else{
		//cout << "Not Empty List" << endl;
		Node* rm = back_->prev_;	
		back_->prev_ = rm->prev_;
		back_->prev_->next_ = back_;
		delete rm;
	}
}
template <typename T>
void Sentinel<T>::print() const{
	Node* curr=front_->next_;
	while(curr!=back_){
		std::cout << curr->data_ << " ";
		curr=curr->next_;
	}
	std::cout << std::endl;
}
template <typename T>
void Sentinel<T>::reversePrint() const{
	Node* curr=back_->prev_;
	while(curr!=front_){
		std::cout << curr->data_ << " ";
		curr=curr->prev_;
	}
	std::cout << std::endl;
}
template <typename T>
Sentinel<T>::~Sentinel(){
	Node* curr=front_;
	while(curr){
		Node* rm=curr;
		curr=curr->next_;
		delete rm;
	}
}
