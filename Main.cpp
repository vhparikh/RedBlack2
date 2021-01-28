//Created by Vatsal Parikh
//Date: 1/20/21
//Red Black Tree with insertion and deletion
//got cases for fixing from https://www.youtube.com/watch?v=5IBxA-bZZH8
//got deletion cases from Gabraiths video

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
void doubleBlack(Node* &head, Node* &black, bool del);
void remove(Node* n);

//main method
int main() {

  char input[80]; //stores user command
  int data; //value for node
  Node* head = NULL; //head node of tree
  bool quit = false; //tracks if the player wants to continue or stop

  //intro statement
  cout << "Welcome to RedBlack tree your commands are add, file, delete, search, print and quit" << endl;

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

    //else if user wants to search call search
    else if (strcmp(input, "search") == 0) {
      cout << "Enter the number you would like to seach for in the tree" << endl;
      cin >> data;
      cin.get();
      Node* temp = search(head, data);

      if (temp == NULL) {
	cout << "Value not in tree" << endl;
      }
      else {
	cout << "Value in tree" << endl;
      }
    }

    //else if user wants to delete call basicDelete
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

//removes node from memory and tree
void remove(Node* current) {
  cout << "remove" << endl;
  Node* p = current->parent;
  Node* c = NULL;
  
  if (current == NULL) {
    return;
  }

  if (current->right != NULL) {
    c = current->right;
  }
  else {
    c = current->left;
  }

  if (p->right == current) {
    p->right = c;
  }
  else {
    p->left = c;
  }

  delete current;
  
}

//cases for double black nodes where something must be done to maintain black node count
void doubleBlack(Node* &head, Node* &black, bool del) {

  //case 1
  if (head == black) {
    //    cout << "c1" << endl;
    if (del == true) {
      remove(black);
    }
    
    return;
  }

  //these names are from galbraiths video for ease of keeping track
  Node* p = black->parent;
  Node* gp = p->parent;
  Node* s = getSibling(black);
  Node* nl = s->left;
  Node* nr = s->right;
  
  //case 2 n is on the left
  if (getColor(s) == RED && getColor(p) == BLACK && p->left == black) {
    //    cout << "c2l" << endl;
    if (head == p) {
      head = s;
      s->parent = NULL;
    }
    else {

      if (gp->left == p) {
	gp->left = s;
	s->parent = gp;
      }
      else {
	gp->right = s;
	s->parent = gp;
      }
    }

    p->color = RED;
    s->color = BLACK;
 
    if (nl != NULL) {
      nl->parent = p;
    }
    
    s->left = p;
    p->parent = s;
    p->right = nl;

    doubleBlack(head, black, del);
    return;
    
  }

  //case 2 n is on the right
  else if (getColor(s) == RED && getColor(p) == BLACK && p->right == black) {
    //    cout << "c2r" << endl;
    if (head == p) {
      head = s;
      s->parent = NULL;
    }
    else {

      if (gp->left == p) {
	gp->left = s;
	s->parent = gp;
      }
      else {
	gp->right = s;
	s->parent = gp;
      }
    }

    p->color = RED;
    s->color = BLACK;

    if (nr != NULL) {
      nr->parent = p;
    }
    
    s->right = p;
    p->parent = s;
    p->left = nr;

    doubleBlack(head, black, del);
    return;
    
  }

  //case 3 n is on the right
  if (getColor(s) == BLACK && getColor(p) == BLACK && getColor(nr) == BLACK && getColor(nl) == BLACK && p->right == black) {
    //    cout << "c3r" << endl;
    s->color = RED;

    if (del == true) {
      remove(black);
    }

    doubleBlack(head, p, false);
    return;
  }

  //case 3 n is on the left
  else if (getColor(s) == BLACK && getColor(p) == BLACK && getColor(nr) == BLACK && getColor(nl) == BLACK && p->left == black) {
    //    cout << "c3l" << endl;
    s->color = RED;

    if (del == true) {
      remove(black);
    }
    
    doubleBlack(head, p, false);
    return;
  }

  //case 4 n is on the right includes wiki subcase
  if (getColor(p) == RED && getColor(s) == BLACK && p->right == black) {

    if (getColor(nr) == RED && getColor(nl) == BLACK) {
      //      cout << "c4rs" << endl;
      p->left = nr;
      nr->parent = p;
      Node* nrl = nr->left;

      if (nrl != NULL) {
	nrl->parent = s;
      }

      s->right = nrl;
      s->parent = nr;
      nr->left = s;

      nr->color = BLACK;
      s->color = RED;
      doubleBlack(head, black, del);
      return;
    }
    else if (getColor(nr) == BLACK && getColor(nl) == BLACK) {
      // cout << "c4r" << endl;
      s->color = RED;
      p->color = BLACK;

      if (del == true) {
	remove(black);
      }

      return;
    }
    
  }
  
  //case 4 n is on the left includes wiki subcase
  else if (getColor(p) == RED && getColor(s) == BLACK && p->left == black) {

    if (getColor(nl) == RED && getColor(nr) == BLACK) {
      //cout << "c4ls" << endl;
      p->right = nl;
      nl->parent = p;
      Node* nlr = nl->right;

      if (nlr != NULL) {
	nlr->parent = s;
      }

      s->left = nlr;
      s->parent = nl;
      nl->right = s;

      nl->color = BLACK;
      s->color = RED;
      doubleBlack(head, black, del);
      return;
    }
    else if (getColor(nl) == BLACK && getColor(nr) == BLACK) {
      //cout << "c4l" << endl;
      s->color = RED;
      p->color = BLACK;

      if (del == true) {
	remove(black);
      }

      return;
    }
    
  }

  //case 5 n is on the right
  if (getColor(p) == BLACK && getColor(s) == BLACK && getColor(nl) == BLACK && getColor(nr) == RED && p->right == black) {
    //cout << "c5r" << endl;
    Node* nrl = nr->left;
    p->left = nr;
    nr->parent = p;
    nr->color = BLACK;
    nr->left = s;

    s->right = nrl;

    if (nrl != NULL) {
      nrl->parent = s;
    }

    s->parent = nr;
    s->color = RED;

    //if (del == true) {
    //remove(black);
    //}

    doubleBlack(head, black, del);
    return;
  }

  //case 5 n is on the left
  else if (getColor(p) == BLACK && getColor(s) == BLACK && getColor(nr) == BLACK && getColor(nl) == RED && p->left == black) {
    //cout << "c5l" << endl;
    Node* nlr = nl->right;
    p->right = nl;
    nl->parent = p;
    nl->color = BLACK;
    nl->right = s;

    s->left = nlr;

    if (nlr != NULL) {
      nlr->parent = s;
    }

    s->parent = nl;
    s->color = RED;

    //if (del == true) {
    // remove(black);
    //}

    doubleBlack(head, black, del);
    return;
  }

  //case 6 n is on the right
  if (getColor(s) == BLACK && getColor(nl) == RED && p->right == black) {
    //cout << "c6r" << endl;
    if (head == p) {
      head = s;
      s->parent = NULL;
    }
    else {

      if (gp->left == p) {
	gp->left = s;
	s->parent = gp;
      }
      else {
	gp->right = s;
	s->parent = gp;
      }
      
    }

    s->color = p->color;
    p->color = BLACK;
    nl->color = BLACK;
    
    s->right = p;
    p->parent = s;
    p->left = nr;

    if (nr != NULL) {
      nr->parent = p;
    }

    if (del == true) {
      remove(black);
    }
    
    return;
  }

  //case 6 n is on the left
  else if (getColor(s) == BLACK && getColor(nr) == RED && p->left == black) {
    //cout << "c6l" << endl;
    if (head == p) {
      head = s;
      s->parent = NULL;
    }
    else {

      if (gp->left == p) {
	gp->left = s;
	s->parent = gp;
      }
      else {
	gp->right = s;
	s->parent = gp;
      }
      
    }

    s->color = p->color;
    p->color = BLACK;
    nr->color = BLACK;
    
    s->left = p;
    p->parent = s;
    p->right = nl;

    if (nl != NULL) {
      nl->parent = p;
    }

    if (del == true) {
      remove(black);
    }
    
    return;
  }
}

//finds node to delete if the node user wants to delete has two children
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

//starts off the delete process contains the simple cases from BST but also calls special cases for RBT
void basicDelete(Node* &head, int val) {

  //the tree is empty
  if (head == NULL) {
    cout << "The tree is empty" << endl;
    return;
  }

  //search for the node in the tree
  Node* n = search(head, val);

  //if its null then the node doesn't exist
  if (n == NULL) {
    cout << "This value doesn't exist in the tree" << endl;
    return;
  }

  //if the node is the head of the tree its a simple delete and its the only thing in the tree
  if (n == head && childrenCount(n) == 0) {
    head = NULL;
    delete n;
    return;
  }

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
  
  //red node with one child the color doesn't matter as removing the red node won't mess anything up
  if (getColor(n) == RED && childrenCount(n) == 1) {

    if (n->left != NULL) {
      Node* replace = n->left;
      replace->parent = n->parent;

      if (n->parent->left == n) {
	n->parent->left = replace;
      }
      else {
	n->parent->right = replace;
      }
    }
    else {
      Node* replace = n->right;
      replace->parent = n->parent;
      
      if (n->parent->left == n) {
	n->parent->left = replace;
      }
      else {
	n->parent->right = replace;
      }
    }

    return;
  }

  //if the node is black and doesn't have two children
  if (getColor(n) == BLACK && childrenCount(n) != 2) {

    //if it has 0 kids its a special case
    if (childrenCount(n) == 0) {
      doubleBlack(head, n, true);
      return;
    }

    //otherwise get its one child node
    Node* c = NULL;
    if (n->right != NULL) {
      c = n->right;
    }
    else {
      c = n->left;
    }

    //if the black node is not the head
    if (n != head) {
      Node* p = n->parent;
      
      if (p->left == n) {
	p->left = c;
	c->parent = p;
      }
      else {
	p->right = c;
	c->parent = p;
      }

      //if the childs color is red just delete the node and make child black
      if (getColor(c) == RED) {
	c->color = BLACK;
	delete n;
	return;
      }

      //otherwise its a special case
      else {
	delete n;
	doubleBlack(head, c, false);
	return;
      }
     
    }

    //if the node is the black and the head delete the head set the kid to head and make it black
    else {
      head = c;
      head->color = BLACK;
      head->parent = NULL;
      delete n;
      return;
    }
  }

  //if everything before didn't work the node has two children we need to find the replacement node
  Node* delNode = findDelNode(head, val);
  n->num = delNode->num;

  //if the replacement node only has one child we can try the same thing we did before
  if (childrenCount(delNode) == 1) {
    Node* c2 = NULL;
    if (delNode->left != NULL) {
      c2 = delNode->left;
    }
    else {
      c2= delNode->right;
    }

    if (getColor(delNode) != getColor(c2)) {

      if (delNode->parent->left == delNode) {
	delNode->parent->left = c2;
      }
      else {
	delNode->parent->right = c2;
      }

      c2->parent = delNode->parent;
      c2->color = BLACK;
      delete delNode;
    }
    return;
  }
  
  if (getColor(delNode) == RED) {

    if (delNode->parent->left == delNode) {
      delNode->parent->left = delNode->left;

      if (delNode->left != NULL) {
	delNode->left->parent = delNode->parent;
      }
    }
    else {
      delNode->parent->right = delNode->right;

      if (delNode->right != NULL) {
	delNode->right->parent = delNode->parent;
      }
    }
    
    return;
  }

  //if the replacement node is black and its child is red its fairly simple
  if (getColor(delNode) == BLACK && getColor(delNode->right) == RED) {

    if (delNode->num >= delNode->parent->num) {
      delNode->parent->right = delNode->right;

      if (delNode->right != NULL) {
	delNode->right->color = BLACK;
	delNode->right->parent = delNode->parent;
      }
    }
    else {
      delNode->parent->left = delNode->right;

      if (delNode->right != NULL) {
	delNode->left->color = BLACK;
	delNode->left->parent = delNode->parent;
      }
    }

    delete delNode;
    return;
  }

  //if nothing above worked its a special case
  doubleBlack(head, delNode, true);
}

//searchs for node in the tree and returns it
Node* search(Node* n, int val) {

  if (n == NULL) {
    return NULL;
  }

  else if (n->num == val) {
    return n;
  }

  else if (n->num > val) {

    if (n->left != NULL) {
      Node* temp = n->left;
      return search(temp, val);
    }

    else {
      return NULL;
    }
  }

  else {

    if (n->right != NULL) {
      Node* temp = n->right;
      return search(temp, val);
    }

    else {
      return NULL;
    }
  }
}

//returns number of children a node has
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
