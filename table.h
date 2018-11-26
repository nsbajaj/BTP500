/*************************************************************/
/*                                                           */
/*    starter file for a2                                    */
/*    v 1.1: removed SimpleTable::grow() function def.       */
/*                                                           */
/*************************************************************/

#include <iostream>
#include <string>
#include <utility>
#include <functional>

using namespace std;

template <class TYPE>
class Table{
public:
	Table(){}
	virtual bool update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		string key_;
        TYPE data_;
		Record(const string& key, const TYPE& data){
			key_=key;
			data_=data;
		}

	};

	Record** records_;   //the table
	int max_;           //capacity of the array
	int size_;          //current number of records held
	int search(const string& key);

public:
	SimpleTable(int capacity);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual bool update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const{return size_==0;}
	virtual int numRecords() const{return size_;}
};


//returns index of where key is found.
template <class TYPE>
int SimpleTable<TYPE>::search(const string& key){
	int rc=-1;
	for(int i=0;i<size_;i++){
		if(records_[i]->key_==key){
			rc=i;
		}
	}
	return rc;
}


template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity): Table<TYPE>(){
	records_=new Record*[capacity];
	max_=capacity;
	size_=0;
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.max_];
	max_=other.max_;
	size_=0;
	for(int i=0;i<other.size_;i++){
		update(other.records_[i]->key_,other.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	size_=other.size_;
	max_=other.max_;
	records_=other.records_;
	other.records_=nullptr;
	other.size_=0;
	other.max_=0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx=search(key);
	if(idx==-1){
		if(size_ < max_){
			records_[size_++]=new Record(key,value);
			for(int i=0;i<size_-1;i++){
				for(int j=0;j<size_-1-i;j++){
					if(records_[j] > records_[j+1]){
						TYPE tmp=records_[j];
						records_[j]=records_[j+1];
						records_[j+1]=tmp;
					}
				}
			}
		}
	}
	else{
		records_[idx]->data_=value;
	}
	return true;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	int idx=search(key);
	if(idx!=-1){
		delete records_[idx];
		for(int i=idx;i<size_-1;i++){
			records_[i]=records_[i+1];
		}
		size_--;
		return true;
	}
	else{
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	int idx=search(key);
	if(idx==-1)
		return false;
	else{
		value=records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
	if(this!=&other){
		if(records_){
			int sz=size_;
			for(int i=0;i<sz;i++){
				remove(records_[0]->key_);
			}
			delete [] records_;
		}
		records_=new Record*[other.max_];
		max_=other.max_;
		size_=0;
		for(int i=0;i<other.size_;i++){
			update(other.records_[i]->key_,other.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_);
	swap(size_,other.size_);
	swap(max_,other.max_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		int sz=size_;
		for(int i=0;i<sz;i++){
			remove(records_[0]->key_);
		}
		delete [] records_;
	}
}

template <class TYPE>
class LPTable:public Table<TYPE>{
private:
	struct Record{
            TYPE data_;
            string key_;
            Record(const string& key, const TYPE& data){
                key_=key;
                data_=data;
            }
        };

        Record** records_;   //the table
        int max_;           //capacity of the array
        int size_;          //current number of records held
        double maxLoadFactor_;
        size_t search(const string& key); //Search

public:
	LPTable(int capacity,double maxLoadFactor); //Constructors
    	virtual bool update(const string& key, const TYPE& value); //Update
	virtual bool find(const string& key, TYPE& value); //Find
    	virtual bool remove(const string& key); //Remove
    	LPTable(const LPTable& other); //Copy Constructor
    	virtual const LPTable& operator=(const LPTable& other); //Assignment Operator
	LPTable(LPTable&& other); //Move Constructor
	virtual const LPTable& operator=(LPTable&& other); //Move Operator
	virtual bool isEmpty() const; //isEmpty
    	virtual int numRecords() const; //numRecords
    	void print(); //Print
    	virtual ~LPTable(); //Destructor
};

/*
 Constructor.
 Initializes an array of Records (Hash Table), and sets each element of the array to nullptr.
 It also sets the default values for the max, size, maxLoadFactor.
 */
template <class TYPE>
LPTable<TYPE>::LPTable(int capacity,double maxLoadFactor): Table<TYPE>(){
	records_ = new Record*[capacity];
	max_ = capacity;
	size_ = 0;
    	maxLoadFactor_ = maxLoadFactor;
	for(int i = 0; i < max_; i++){
		records_[i] = nullptr;
	}
}

/*
 Update.
 The function is passed a key of type string and a value of type "TYPE".
 A random number (index) between 0 and max_ (size of array) is generated. If that element in the array is empty (nullptr), and the current load factor is less than the maxLoadFactor, a new record will be inserted. If there is something at the element, at the key match's the key passed to the function, the record will be updated. If there is something other than the matching key, it will be a "Deleted" record. A record that used to be there, but isn't there anymore and a new record can be inserted in it's place. Else, it will increment the index variable and continue to probe through the array. The function returns true if a record was successfully inserted into an empty/deleted element or a record was updated. It will return false if a record was not inserted due to the load factor being maxed, or it was unable to find an empty/deleted spot and had already looped through the loop.
 */
template <class TYPE>
bool LPTable<TYPE>::update(const string& key, const TYPE& value){
    bool check = false;
    hash<string> hashFunction;
    size_t index = hashFunction(key) % max_;
    size_t initialIndex = index;
    
    while(!check){
        if(records_[index] == nullptr){
            double loadFactor = (double)size_/max_;
            //New Record
            if(loadFactor < maxLoadFactor_){
                records_[index] = new Record(key, value);
                size_++;
                check = true;
            }
            //If Load Factor the load factor set for the hashtable, it will return false and won't add any more records.
            else{
                break;
            }
        }
        else{
            //Update
            if(records_[index]->key_ == key){
                records_[index]->data_ = value;
                check = true;
            }
            //Inserting data into a "Deleted" slot. This statement will insert a new record, into an element where there used to be a record.
            else if(records_[index]->key_ == to_string(-1)){
                double loadFactor = (double)size_/max_;
                if(loadFactor < maxLoadFactor_){
                    delete records_[index];
                    records_[index] = new Record(key, value);
                    size_++;
                    check = true;
                }
            }
            else{
                //Probing to next element
                index++;
                //If reached end of the array, loop back to 0.
		if(index == max_){
                    index = 0;
                }
                //Looped around the loop and wasn't able to insert/update.
                if(index == initialIndex){
                    check = false;
                    break;
                }
        	/*
                //Also works
                index++;
                index = index % max_;
                */
            }
        }
    }
    return check;
}

/*
 Search.
 The function is passed a key of type string.
 A random number (index) between 0 and max_ (size of array) is generated. It then begins to loop through the array, while checking for elements that are not empty. If it finds a record that match's the key that was passed into the function, it will return that index. Else, it will continue to loop/probe through the loop. If it is unable to find a matching record, it will return -1. If an empty element is found, the loop ends and the function returns the empty element/index.
 */
template <class TYPE>
size_t LPTable<TYPE>::search(const string& key){
    hash<string> hashFunction;
    size_t index = hashFunction(key) % max_;
    size_t initialIndex = index;
    
    //Keep looping as long as there is something in the array (!= nullptr)
    while(records_[index] != nullptr){
        if(records_[index]->key_ == key){
            return index;
        }
        //Probing to next element
        index++;
        //If reached end of the array, loop back to 0.
	if(index == max_){
            index = 0;
        }
        //Looped around the loop.
        if(index == initialIndex){
            return -1;
        }
    }
    //Returns next empty spot
    return index;
}

/*
 Find.
 The function is passed a key of type string and a value of type "TYPE".
 The function calls the "search" function and is returned an index containing the record, which match's the "key" variable, an empty element or an empty element/index. If a record is found, it sets the "value" variable to the records "data_" value and returns true. Else, it returns false if it didn't find a matching element or an empty element.
 */
template <class TYPE>
bool LPTable<TYPE>::find(const string& key, TYPE& value){
    bool check = false;
    size_t result = search(key);
    if(result != -1){
        if(records_[result] != nullptr){
            value = records_[result]->data_;
            check = true;
        }
        //Empty Spot
        else{
            check = false;
        }
    }
    return check;
}

/*
 Remove.
 The function is passed a key of type string.
 The function calls the "search" function and is returned an index containing the record, which match's the "key" variable, an empty element or an empty element/index. If a record is found, it marks that element as a "Deleted" (Tombstone) record, reduces the size by 1 and retusn true. Else, it returns false if it didn't find a matching element or an empty element.
 */
template <class TYPE>
bool LPTable<TYPE>::remove(const string& key){
    bool check = false;
    size_t result = search(key);
    if(result != -1){
        if(records_[result] != nullptr){
            records_[result] = new Record(to_string(-1), -1);
            size_--;
            check = true;
        }
        //Empty Spot
        else{
            check = false;
        }
    }
    return check;
}

/*
 Copy Constructor.
 The copy constructor checks if the other object being passed in is different than the current object.
 If it is different, it "copies"/sets the values of the member variables of the other object to it's own (current). It allocates the size/memory for the record array and begins to copy/insert the other array's records into current objects array.
 */
template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other){
    if(this != &other && this->max_ != other.max_ && this->size_ != other.size_ && this->maxLoadFactor_ != other.maxLoadFactor_){
        this->max_ = other.max_;
        this->size_ = other.size_;
        this->maxLoadFactor_ = other.maxLoadFactor_;
        this->records_ = new Record*[this->max_];
        for(int i = 0; i < this->max_; i++){
            if(other.records_[i] != nullptr){
                this->records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
            }
            else{
                this->records_[i] = nullptr;
            }
        }
    }
}

/*
 Assignment Operator.
 The assignment operator checks if the other object being passed in is different than the current object. If it is different and different than nullptr, it begins to empty/delete the current objects record array. It then assigns the values of the other objects member variable to current objects and copies the elements in the record array to current objects array. It returns a dereferenced pointer of the current object.
 */
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){
    if(this != &other){
        if(records_ != nullptr){
            for (int i = 0; i < max_; i++){
                delete records_[i];
            }
            delete[] records_;
            
            this->max_ = other.max_;
            this->size_ = other.size_;
            this->maxLoadFactor_ = other.maxLoadFactor_;
            this->records_ = new Record*[this->max_];
            for(int i = 0; i < this->max_; i++){
                if(other.records_[i] != nullptr){
                    this->records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
                }
                else{
                    this->records_[i] = nullptr;
                }
            }
        }
    }
    return *this;
}

/*
 Move Constructor.
 The move constructor checks if the other object being passed in is different than the current object.
 If it is different, it sets/"moves" the values of the member variables of the other object to it's own (curret). Once the values have been set/moved, the other object's variables are set to defalut values.
 */
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other){
    if(this != &other && this->max_ != other.max_ && this->size_ != other.size_ && this->maxLoadFactor_ != other.maxLoadFactor_){
        this->max_ = other.max_;
        this->size_ = other.size_;
        this->maxLoadFactor_ = other.maxLoadFactor_;
        this->records_ = other.records_;
        
        other.max_ = 0;
        other.size_ = 0;
        other.maxLoadFactor_ = 0;
        other.records_ = nullptr;
    }
}

/*
 Move Operator.
 The move operator checks if the other object being passed in is different than the current object. If it is different, it makes a temporary records array and points to the current objects record array. It then points the current objects record array to the other objects record array and then the other objects record array to the temporary record array. It performs a swap. It returns a dereferenced pointer of the current object.
 */
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other){
    if(this != &other){
        Record** tempRecords_ = this->records_;
        this->records_ = other.records_;
        other.records_ = tempRecords_;
    }
    return *this;
}

/*
 This function returns true if the size of the Hash Table is 0, else false.
 * */
template <class TYPE>
bool LPTable<TYPE>::isEmpty() const {
	if(size_ == 0){
        return true;
    }
    else{
        return false;
    }
}

/*
 This function returns the size of the Hash table.
 */
template <class TYPE>
int LPTable<TYPE>::numRecords() const{
    return size_;
}

/*
 Print Function for the Hash Table.
 */
template <class TYPE>
void LPTable<TYPE>::print(){
    if(max_ > 0){
        for(int i = 0; i < max_; i++){
            if(records_[i] != nullptr){
                cout << "Key: " << records_[i]->key_ << ", Value: " << records_[i]->data_ << ", Element: " << i << endl;
            }
            else if(records_[i] == nullptr){
                cout << "Nullptr at Element: " << i << endl;
            }
        }
    }
}

/*
 Destructor.
 It checks if the current objects record array is not nullptr and if the max size of the array is more than 0. If valid, it begins to delete the records within the array and then deletes the array.
 */
template <class TYPE>
LPTable<TYPE>::~LPTable(){
    if(records_ != nullptr && max_ > 0){
        for (int i = 0; i < max_; i++){
            delete records_[i];
        }
        delete[] records_;
    }
}
