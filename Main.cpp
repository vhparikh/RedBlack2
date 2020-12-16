#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

const int RED = 0;
const int BLACK = 1;

struct Node {
  Node* left;
  Node* right;
  Node* parent;
  int num;
  int color;

  Node() {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 0;
    num = 0;
  }

  Node(int n) {
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 0;
    num = n;
  }
  
};

void print(Node* n, int space);
Node* add(Node* &n, int val);
void fix(Node* &head, Node* &n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
int getColor(Node* n);

int main() {

  //Node* n = new Node();
  char input[80];
  int data;
  Node* head = NULL;
  bool quit = false;

  cout << "Welcome to RedBlack tree" << endl;

  /*for (int i = 0; i < 30; i++) {
    Node* node = add(head, i);
    fix(head, node);
    //  Node* temp = add(head, 100-i);
    //fix(head, temp);
  }
  print(head, 0);*/
  
  while(!quit) {
    cin.get(input, 80);
    cin.get();

    if (strcmp(input, "add") == 0) {
      cout << "Enter the number you would like to add to the tree" << endl;
      cin >> data;
      cin.get();
      Node* node = add(head, data);
      //cout << "a" << endl;
      //cout << getColor(node->parent) << endl;
      fix(head, node);
    }

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

    else if (strcmp(input, "print") == 0) {
      print(head, 0);
    }
    
    else if (strcmp(input, "quit") == 0) {
      quit = true;
    }
    
  }
  
}

int getColor(Node* n) {
  if (n == NULL) {
    return BLACK;
  }
  return n->color;
}

Node* getUncle(Node* n) {
  //cout << getSibling(n->parent)->num << endl;
  if (getSibling(n->parent) == NULL) {
    return NULL;
  }
  return getSibling(n->parent);
}

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

void fix(Node* &head, Node* &n) {

  //Node* n = add(head, val);
  
  //  cout << "d" << endl;

  if (head == n) {
    //cout << "case 0" << endl;
    n->color = 1;
    return;
  }

  //cout << "f" << endl;
  //cout << n->parent->num << endl;
  if (getColor(n->parent) == RED && getColor(getUncle(n)) == RED) {
    // cout << "case 1" << endl;
    n->parent->color = BLACK;
    getUncle(n)->color = BLACK;
    n->parent->parent->color = RED;
    Node* temp = n->parent->parent;
    fix(head, temp);
    return;
  }

  //cout << "h" << endl;
  if (getColor(n->parent) == RED && getColor(getUncle(n)) == BLACK) {
    //cout << "g" << endl;
    //triangle right
    if (n->parent->num > n->num && n->parent->parent->num <= n->parent->num) {
      //cout << "tr" << endl;
      Node* parentN = n->parent;
      Node* grandparentN = parentN->parent; 
      
      n->right = parentN;
      grandparentN->right = n;
      parentN->left = NULL;
      parentN->parent = n;
      n->parent = grandparentN;
      //n = parentN;
      fix(head, parentN);
      return;
    }

    //triangle left
    else if (n->parent->num <= n->num && n->parent->parent->num > n->parent->num){
      //cout << "tl" << endl;
      Node* parentN = n->parent;
      Node* grandparentN = parentN->parent;

      n->left = parentN;                                                                                                                          
      grandparentN->left = n;                                                                                                                     
      parentN->right = NULL;                                                                                                                       
      parentN->parent = n;                                                                                                                         
      n->parent = grandparentN;
      // n = parentN;
      fix(head, parentN);
      return;
    }

    //line right
    if (n->parent->num <= n->num && n->parent->parent->num <= n->num) {
      //cout << "lr" << endl;
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

    //line left
    if (n->parent->num > n->num && n->parent->parent->num > n->num) {
      //cout << "ll" << endl;
      Node* parentA = n->parent;                                                                                                                   
      Node* grandparentB = parentA->parent;                                                                                                       
      Node* siblingD = getSibling(n);
      
      parentA->right = grandparentB;                                                                                                               
      grandparentB->left = siblingD;                                                                                                              
      //siblingD->parent = grandparentB;
      if (siblingD != NULL) {
	siblingD->parent = grandparentB;
      }
      //cout << "b" << endl;
      
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

Node* add(Node* &n, int val) {

  if (n == NULL) {
    n = new Node(val);
    //cout << "head" << endl;
    return n;
  }

  if (n->num > val) {

    if (n->left == NULL) {
      Node* temp = new Node(val);
      n->left = temp;
      temp->parent = n;
      //cout << "left" << endl;
      return temp;
    }
    
    else {
      Node* temp = n->left;
      return add(temp, val);
    }
  }

  if (n->num <= val) {

    if (n->right == NULL) {
      Node* temp = new Node(val);
      n->right = temp;
      temp->parent = n;
      //cout << "right" << endl;
      return temp;
    }

    else {
      Node* temp = n->right;
      return add(temp, val);
    }
  }

  //cout << "safety" << endl;
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
