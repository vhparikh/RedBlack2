//Created by Vatsal ParikOAh
//Date: 12/10/20
//Red Black Tree with insertion
//got cases for fixing from https://www.youtube.com/watch?v=5IBxA-bZZH8

//imports
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

//constants for color
const int RED = 0;
const int BLACK = 1;

//node struct
struct Node {
  Node* left; //pointer to left child
  Node* right; //pointer to right child
  Node* parent; //pointer to parent node
  int num; //value of node
  int color; //color of node

  //basic constructor sets all pointers to null and ints to 0
  Node() {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 0;
    num = 0;
  }

  //sets all pointers to null and sets num to param value
  Node(int n) {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 0;
    num = n;
  }
  
};

//prototypes
void print(Node* n, int space);
Node* add(Node* &n, int val);
void fix(Node* &head, Node* &n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
int getColor(Node* n);
Node* search(Node* n, int val);
int childrenCount(Node* n);
void basicDelete(Node* &head, int val);
Node* findDelNode(Node* n, int val);

//main method
int main() {

  char input[80]; //stores user command
  int data; //value for node
  Node* head = NULL; //head node of tree
  bool quit = false; //tracks if the player wants to continue or stop

  //intro statement
  cout << "Welcome to RedBlack tree your commands are add, file, print and quit" << endl;

  //while user doesn't want to quit
  while(!quit) {

    //get input
    cin.get(input, 80);
    cin.get();
    
    //if user wants to add get the number they want to add and call add and then fix the tree
    if (strcmp(input, "add") == 0) {
      cout << "Enter the number you would like to add to the tree" << endl;
      cin >> data;
      cin.get();
      Node* node = add(head, data);
      fix(head, node);
    }

    //else if user wants to add with file get file path and add each value and fix the tree everytime
    else if (strcmp(input, "file") == 0) {
      cout << "Enter the file path" << endl;
      cin.get(input, 80);
      cin.get();
      ifstream file;
      file.open(input);

      while (file >> data) {
	Node* node = add(head, data);
	fix(head, node);
      }

      file.close();
      
    }

    //else if user wants to print the tree call print
    else if (strcmp(input, "print") == 0) {
      print(head, 0);
    }

    else if (strcmp(input, "search") == 0) {
      cout << "Enter the number you would like to seach for in the tree" << endl;
      cin >> data;
      cin.get();
      Node* temp = search(head, data);
    }

    else if (strcmp(input, "delete") == 0) {
      cout << "Enter the number you would like to delete from the tree" << endl;
      cin >> data;
      cin.get();
      basicDelete(head, data);
    }
    
    //else if user wants to quit set quit to true
    else if (strcmp(input, "quit") == 0) {
      quit = true;
    }
    
  }
  
}

Node* findDelNode(Node* n, int val) {

  Node* current = search(n, val);

  if (current->right == NULL) {
    return NULL;
  }
  else {
    current = current->right;
  }

  while (current->left != NULL) {
    current = current->left;
  }

  return current;
  
}

void basicDelete(Node* &head, int val) {

  if (head == NULL) {
    cout << "The tree is empty" << endl;
    return;
  }

  Node* n = search(head, val);

  if (n == NULL) {
    cout << "This value doesn't exist in the tree" << endl;
    return;
  }

  if (n == head && childrenCount(n) == 0) {
    head = NULL;
    delete n;
    cout << "The value has been removed from the tree" << endl;
    return;
  }

  Node* delNode = findDelNode(head, val);

  //red node with no children is a simple BST delete
  if (getColor(n) == RED && childrenCount(n) == 0) {
    Node* p = n->parent;

    if (p->left == n) {
      p->left = NULL;
      delete n;
      return;
    }
    else {
      p->right = NULL;
      delete n;
      return;
    }    
  }

  
  
}

Node* search(Node* n, int val) {

  if (n == NULL) {
    cout << "The tree is empty" << endl;
    return NULL;
  }

  else if (n->num == val) {
    cout << "The number exists" << endl;
    return n;
  }

  else if (n->num > val) {

    if (n->left != NULL) {
      Node* temp = n->left;
      return search(temp, val);
    }

    else {
      cout << "The number isn't on the tree" << endl;
      return NULL;
    }
  }

  else {

    if (n->right != NULL) {
      Node* temp = n->right;
      return search(temp, val);
    }

    else {
      cout << "The number isn't on the tree" << endl;
      return NULL;
    }
  }

}

int childrenCount(Node* n) {
  int count = 0;

  if (n->left != NULL) {
    count++;
  }

  if (n->right != NULL) {
    count++;
  }

  return count;
  
}

//returns color of node if it is null it is black
int getColor(Node* n) {
  if (n == NULL) {
    return BLACK;
  }
  return n->color;
}

//returns uncle of node
Node* getUncle(Node* n) {
  if (getSibling(n->parent) == NULL) {
    return NULL;
  }
  return getSibling(n->parent);
}

//returns sibling of node
Node* getSibling(Node* n) {
  
  if (n->parent->num > n->num) {

    if (n->parent->right != NULL) {
      return n->parent->right;
    }
  }

  else {

    if (n->parent->left != NULL) {
      return n->parent->left;
    }
  }

  return NULL;

}

//fixes the tree after a node is added
void fix(Node* &head, Node* &n) {

  //if the node was the head node make it black and return
  if (head == n) {
    n->color = 1;
    return;
  }

  //if the parent and uncle are red we need to recolor the nodes
  if (getColor(n->parent) == RED && getColor(getUncle(n)) == RED) {
    n->parent->color = BLACK;
    getUncle(n)->color = BLACK;
    n->parent->parent->color = RED;
    Node* temp = n->parent->parent;
    fix(head, temp);
    return;
  }

  //if the parent is red and the uncle is black we got some interesting things to do
  if (getColor(n->parent) == RED && getColor(getUncle(n)) == BLACK) {
    
    //triangle right rotate n parent so n is now parent of n->parent
    if (n->parent->num > n->num && n->parent->parent->num <= n->parent->num) {
      Node* parentN = n->parent;
      Node* grandparentN = parentN->parent; 
      
      n->right = parentN;
      grandparentN->right = n;
      parentN->left = NULL;
      parentN->parent = n;
      n->parent = grandparentN;
      fix(head, parentN);
      return;
    }

    //triangle left rotate n parent so n is now parent of n->parent
    else if (n->parent->num <= n->num && n->parent->parent->num > n->parent->num){
      Node* parentN = n->parent;
      Node* grandparentN = parentN->parent;

      n->left = parentN;                                                                                                                          
      grandparentN->left = n;                                                                                                                     
      parentN->right = NULL;                                                                                                                       
      parentN->parent = n;                                                                                                                         
      n->parent = grandparentN;
      fix(head, parentN);
      return;
    }

    //line right rotate n grandparent
    if (n->parent->num <= n->num && n->parent->parent->num <= n->num) {
      Node* parentA = n->parent;
      Node* grandparentB = parentA->parent;
      Node* siblingD = getSibling(n);
      
      parentA->left = grandparentB;
      grandparentB->right = siblingD;

      if (siblingD != NULL) {
	siblingD->parent = grandparentB;
      }

      if (grandparentB == head) {
	head = parentA;
	parentA->parent = NULL;
	grandparentB->parent = parentA;
      }

      else {
	parentA->parent = grandparentB->parent;
	Node* greatgrandN = grandparentB->parent;
	grandparentB->parent = parentA;

	if (greatgrandN->num > parentA->num) {
	  greatgrandN->left = parentA;
	}

	else {
	  greatgrandN->right = parentA;
	}
	
      }

      parentA->color = BLACK;
      grandparentB->color = RED;
      
    }

    //line left rotate n grandparent
    if (n->parent->num > n->num && n->parent->parent->num > n->num) {
      Node* parentA = n->parent;                                                                                                                   
      Node* grandparentB = parentA->parent;                                                                                                       
      Node* siblingD = getSibling(n);
      
      parentA->right = grandparentB;                                                                                                               
      grandparentB->left = siblingD;                                                                                                              
      if (siblingD != NULL) {
	siblingD->parent = grandparentB;
      }
      
      if (grandparentB == head) {                                                                                                                  
        head = parentA;                                                                                                                            
        parentA->parent = NULL;                                                                                                                    
        grandparentB->parent = parentA;                                                                                                            
      }
      else {                                                                                                                                       
        parentA->parent = grandparentB->parent;                                                                                                    
        Node* greatgrandN = grandparentB->parent;                                                                                                  
        grandparentB->parent = parentA;                                                                                                            

	if (greatgrandN->num > parentA->num) {                                                                                                     
          greatgrandN->left = parentA;                                                                                                             
        }                                                                                                                                          
        else {                                                                                                                                     
          greatgrandN->right = parentA;                                                                                                            
        }                                                                                                                                          
	
      }
      
      parentA->color = BLACK;
      grandparentB->color = RED;
      
    }
  }
  
}

//simple add function
Node* add(Node* &n, int val) {

  //if location is null add node here
  if (n == NULL) {
    n = new Node(val);
    return n;
  }

  //if the current node val is > val traverse left
  if (n->num > val) {

    if (n->left == NULL) {
      Node* temp = new Node(val);
      n->left = temp;
      temp->parent = n;
      return temp;
    }
    
    else {
      Node* temp = n->left;
      return add(temp, val);
    }
  }

  //if the current node val is < val traverse right
  if (n->num <= val) {

    if (n->right == NULL) {
      Node* temp = new Node(val);
      n->right = temp;
      temp->parent = n;
      return temp;
    }

    else {
      Node* temp = n->right;
      return add(temp, val);
    }
  }

  //just a safety return so code doesn't crash
  return NULL;
}

//prints out the tree sideways                                                                                      
void print(Node* n, int space) {                                                                                    
                                                                                                                    
  //if the node is null we end the print                                                                            
  if (n == NULL) {                                                                                                  
    return;                                                                                                         
  }                                                                                                                 
                                                                                                                    
  //add 10 spaces for printing                                                                                      
  space += 10;                                                                                                      
                                                                                                                    
  //print right subtree                                                                                             
  print(n->right, space);                                                                                      
  cout << endl;                                                                                                     
                                                                                                                    
  //print spaces until space limit is met                                                                           
  for (int i = 10; i < space; i++) {                                                                                
    cout<<" ";                                                                                                      
  }                                                                                                                 
                                                                                                                    
  //print number                                                                                                    
  cout << n->num;

  if (n->color == RED) {
    cout << " (R)" << "\n";
  }
  else {
    cout << " (B)" << "\n";
  }
  
  //print right subtree                                                                                             
  print(n->left, space);                                                                                       
}        
