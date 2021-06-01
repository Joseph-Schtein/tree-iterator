#pragma once
#include <iostream>
#include <stack>
#include <queue>
#include <stdio.h>

namespace ariel{
    
    template <typename T>
    class BinaryTree{

        
        struct Node{
            T value;
            Node* left;
            Node* right;
            Node(const T& v): value(v), left(nullptr), right(nullptr){}

            bool operator==(const Node& other) const{
				return this->value == other->value && this->left == other->left && this->right == other.right;
			}

			bool operator!=(const Node& other) const{
				return this->value != other->value || this->left != other->left || this->right != other.right;
			}
        };

        Node* root;

        

        BinaryTree<T>& add_leftPrivate(Node* tmp , const T& insertFrom, T input){
           Node* par = search(tmp, insertFrom);
            if(par == nullptr){
                throw std::invalid_argument(" we can't add to right because there isn't a node with a certain value");
            }

            if(par->left == nullptr){
                Node* inNode = new Node(input);
                par->left = inNode;
            }

            if(par->left != nullptr){
               par->left->value = input; 
            }

            return *this;
        }

        BinaryTree<T>& add_rightPrivate(Node* tmp , const T& insertFrom, T input){
            Node* par = search(tmp, insertFrom);
            if(par == nullptr){
                throw std::invalid_argument(" we can't add to right because there isn't a node with a certain value");
            }

            if(par->right == nullptr){
                Node* inNode = new Node(input);
                par->right = inNode;
            }

            else{
               par->right->value = input; 
            }

            return *this;
        }

        Node* search(Node* tmp , const T& insertFrom){// https://www.geeksforgeeks.org/search-a-node-in-binary-tree/
            if(tmp == nullptr){
                return nullptr;
            }
            
            if(tmp->value == insertFrom){
                return tmp;
            }
            
            
            Node* tmpLeft = search(tmp->left, insertFrom);

            if(tmpLeft){
                return tmpLeft;
            }
            
            Node* tmpRight = search(tmp->right, insertFrom);
                    

            return tmpRight;
        
        }
       
        void deleteTree(Node *tmp){
            if(tmp != nullptr){
		        deleteTree(tmp->left);
		        deleteTree(tmp->right);
		        delete tmp;
	        }
        }

        void copyTree(Node* mine, const Node* other){
            if(other->left != nullptr){
                mine->left = new Node(other->left->value);
                copyTree(mine->left, other->left);
            }

            if(other->right == nullptr){
                mine->right = new Node(other->right->value);
                copyTree(mine->right, other->right);

            }
        }

    
        public:

        BinaryTree(): root(nullptr){}


        BinaryTree(const BinaryTree& other){
            if(other.root == nullptr){
                root = nullptr;
                
            }

            else{
                this->root = new Node(other.root->value);
                copyTree(root, other.root);
            }
        }

        BinaryTree(BinaryTree &&other) noexcept {
            this->root = other.root;
            other.root = nullptr;
        }  


        ~BinaryTree(){
            deleteTree();
        }

        void deleteTree(){
            deleteTree(this->root);
        }

        BinaryTree& operator=(BinaryTree &&other) noexcept {
            if (root){ delete root;}
            root = other.root;
            other.root = nullptr;
            return *this;
        }


        BinaryTree& operator=(const BinaryTree<T>& other){
            if(this == &other){
                return *this;
            }

            if(this->root != nullptr){
                delete root;
            }

            if(other.root != nullptr){
                root = new Node(other.root->value);
                copyTree(root, other.root);
            }

            return *this;
        }



        BinaryTree<T>& add_root(T v){
            
            if(this->root == nullptr){
                root = new Node(v);
            }

            else{
                root->value = v;
            }

            return *this;
        }


        BinaryTree<T>& add_left(T insertFrom, T input){
            
            if(root == nullptr){
                throw std::invalid_argument("We cna't insert to left because there isn't a root");
            }

            if(root != nullptr){
                add_leftPrivate(root, insertFrom, input);
            }
            return *this;
        }

        BinaryTree<T>& add_right(T insertFrom, T input){
            if(root == nullptr){
                throw std::invalid_argument("We cna't insert to right because there isn't a root");
            }
            
            if(root != nullptr){
                add_rightPrivate(root, insertFrom, input);
            }

            return *this;

        }
        
        friend std::ostream& operator<< (std::ostream& os, BinaryTree<T> tree){
            return os << " hello there " << std::endl;
        }


        
        struct preOrderIterator{

            Node* pointerToNode;
            std::queue<Node *> contain;

            preOrderIterator(Node* ptr){ //https://www.techiedelight.com/preorder-tree-traversal-iterative-recursive/
                
                if (ptr == nullptr){
                    pointerToNode = ptr;
                }
                    
                else{
                    std::stack<Node*> tmpContain;
                    tmpContain.push(ptr);
 
                    while (!tmpContain.empty()) {
                        
                        Node* tmp = tmpContain.top();
                        tmpContain.pop();
                        contain.push(tmp);

 
                        if (tmp->right){
                            tmpContain.push(tmp->right);
                        }

                        if (tmp->left){
                            tmpContain.push(tmp->left);
                        }
                    }

                    pointerToNode = contain.front();
                }    
            }

            T& operator*() const {
			    return pointerToNode->value;
		    }

            T* operator->() const {
			    return &(pointerToNode->value);
		    }

            preOrderIterator& operator++() {
			    if(!contain.empty()){
                    this->contain.pop();
        
                }

                if(!contain.empty()){
                    pointerToNode = contain.front();
                }

                else{
                    pointerToNode = nullptr; 
                }
                
                return *this;
		    }

		    preOrderIterator operator++(int) {
			   if(*this == nullptr){
                    return *this;
                }
                    
                preOrderIterator tmp = *this; 
                ++*this;
                return tmp;
                
            }
            bool operator==(const preOrderIterator& rhs) const {
                return pointerToNode == rhs.pointerToNode;
            }

            bool operator!=(const preOrderIterator& rhs) const {
			    return pointerToNode != rhs.pointerToNode;
		    }

        };


        struct inorderIterator{

            Node* pointerToNode;
            std::queue<Node *> contain;

            public:

            inorderIterator(Node* ptr){ //https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
                
                if(ptr == nullptr){
                    pointerToNode = ptr;
                }

                else{  
                    std::stack<Node*> tmpContain;
                    Node* tmp = ptr;  
                    while (tmp != nullptr || !tmpContain.empty()){
        
                        while (tmp !=  nullptr){
                            tmpContain.push(tmp);
                            tmp = tmp->left;
                        }

                        tmp = tmpContain.top();
                        tmpContain.pop(); 
                        contain.push(tmp);
                        tmp = tmp->right;
 
                    }

                    pointerToNode = contain.front();
                }
            }

            T& operator*() const {
			    return pointerToNode->value;
		    }

            T* operator->() const {
			    return &(pointerToNode->value);
		    }

            inorderIterator& operator++() {
			    if(!contain.empty()){
                    contain.pop();
                }

                if(!contain.empty()){
                    pointerToNode = contain.front();
                }

                else{
                   pointerToNode = nullptr; 
                }
                
                return *this;
		    }

		    inorderIterator operator++(int) {
			   if(*this == nullptr){
                    return *this;
                }
                
                inorderIterator tmp = *this; 
                ++*this;
                return tmp;
                
            }

            bool operator==(const inorderIterator& rhs) const {
                return pointerToNode == rhs.pointerToNode;
            }

            bool operator!=(const inorderIterator& rhs) const {
			    return pointerToNode != rhs.pointerToNode;
		    }

        };


        struct postorderIterator{// https://www.techiedelight.com/postorder-tree-traversal-iterative-recursive/

            Node* pointerToNode;
            std::stack<Node *> contain;

            public:

            postorderIterator(Node* ptr){
                if (ptr == nullptr){
                    pointerToNode = ptr;
                }   
                
                else{
                    std::stack<Node*> tmpContain;
                    tmpContain.push(ptr);

                    while (!tmpContain.empty()) {
                        
                        Node* tmp = tmpContain.top();
                        tmpContain.pop();
                        contain.push(tmp);

 
                        if (tmp->left){
                            tmpContain.push(tmp->left);
                        }

                        if (tmp->right){
                            tmpContain.push(tmp->right);
                        }
                    }
                    
                    pointerToNode = contain.top();
                }    
            }

            T& operator*() const {
			    return pointerToNode->value;
		    }

            T* operator->() const {
			    return &(pointerToNode->value);
		    }

            postorderIterator& operator++() {
			    if(!contain.empty()){
                    this->contain.pop();
                }

                if(!contain.empty()){
                    pointerToNode = contain.top();
                }

                else{
                    pointerToNode = nullptr; 
                }
                
                return *this;
		    }

		    postorderIterator operator++(int) {
			    if(this->pointerToNode == nullptr){
                    return *this;
                }
                
                postorderIterator tmp = *this; 
                ++*this;
                return tmp;
                
            }

            bool operator==(const postorderIterator& rhs) const {
                return pointerToNode == rhs.pointerToNode;
            }

            bool operator!=(const postorderIterator& rhs) const {
			    return pointerToNode != rhs.pointerToNode;
		    }

        };




        preOrderIterator begin_preorder() {
		    return preOrderIterator{root};
	    }
	
	    preOrderIterator end_preorder() {
		    return preOrderIterator{nullptr};
	    }


        inorderIterator begin_inorder() {
		    return inorderIterator{root};
	    }
	
	    inorderIterator end_inorder() {
		    return inorderIterator{nullptr};
	    }


        inorderIterator begin() {
		    return inorderIterator{root};
	    }
	
	    inorderIterator end() {
		    return inorderIterator{nullptr};
	    }


        postorderIterator begin_postorder() {
		    return postorderIterator{root};
	    }
	
	    postorderIterator end_postorder() {
		    return postorderIterator{nullptr};
	    }
    };
}; 