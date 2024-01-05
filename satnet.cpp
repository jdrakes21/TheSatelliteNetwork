/*****************************************
** File:    satnet.cpp
** Project: CMSC 341 Project 2, Fall 2023
** Author:  Jervon Drakes
** Date:    10/24/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the satnet.cpp file for Project 2.
** This program reads the satnet.h file and its functions and
** executes those functions for implementation
**
**
**
***********************************************/

// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"

// SatNet()
// The constructor performs the required initializations. It creates an empty object.
SatNet::SatNet(){

  m_root = nullptr; // initialize the root of the AVL tree to nullptr, creating an empty tree
}

// ~SatNet(destructor)
// The destructor performs the required cleanup including memory deallocations and re-initializing
SatNet::~SatNet(){

  // clear the AVL tree and set the root to nullptr
  clear();
  // m_root = nullptr;
    
}

// insert(const Sat& satellite)
// This function inserts a Sat object into the tree in the proper position. The Sat::m_id should be used as the key to traverse the SatNet tree and abide by BST traversal rules. The comparison operators (>, <, ==, !=) work with the int type in C++. A Sat id is a unique number in the range MINID - MAXID. 
void SatNet::insert(const Sat& satellite){

  // create a new node for the satellite data
  Sat* node = new Sat(satellite.getID(), satellite.getAlt(), satellite.getInclin(), satellite.getState());

  // call the helperInsert function to insert the node into the AVL tree
  m_root = helperInsert(m_root, node);

  // update the height of the root node based on the maxmimum height of children
  m_root->setHeight(1 + max((m_root->getLeft() ? m_root->getLeft()->getHeight() : -1), (m_root->getRight() ? m_root->getRight()->getHeight() : -1)));
  
}

// helperInsert(Sat* node, Sat* nodeNew)
// helper function for the insert function
Sat* SatNet::helperInsert(Sat* node, Sat* nodeNew){

  if(node == nullptr){

    // if the current node is full return the new node
    return nodeNew;

  }

  // perform Binary Search Tree insertion on m_id
  if(nodeNew->getID() < node->getID()){
    node->setLeft(helperInsert(node->getLeft(), nodeNew));
 
  } else if (nodeNew->getID() > node->getID()){

    node->setRight(helperInsert(node->getRight(), nodeNew));

  } else {

    // reject the insertion as duplicate ID
    delete nodeNew;
    return node;

  }

  //update the current node's height
  int heightLeft = (node->getLeft() == nullptr) ? -1 : node->getLeft()->getHeight();

  int heightRight = (node->getRight() == nullptr) ? -1 : node->getRight()->getHeight();
  node->setHeight(max(heightLeft, heightRight) + 1);

  // determine if there is any imbalance
  int bstBalance = heightLeft - heightRight;

  // left heavy imbalance
  if(bstBalance > 1){

    if(nodeNew->getID() < node->getLeft()->getID()){

      // right single rotation occurs
      return rotateRight(node);

    } else {

      // doublt left-right rotation occurs
      node->setLeft(rotateLeft(node->getLeft()));

      return rotateRight(node);

    }
  }

  // right heavy imbalance
  if(bstBalance < -1){

    if(nodeNew->getID() > node->getRight()->getID()){

      // left single rotation occurs
      return rotateLeft(node);

    } else {

    // double right-left rotation occurs
    node->setRight(rotateRight(node->getRight()));
    return rotateLeft(node);
    }
    
  } 

  // no imbalance return the node as it is
  return node;
}

// rotateLeft(Sat* node)
// helper function which rotates the node to the left
Sat* SatNet::rotateLeft(Sat* node){

  // retrieve the root of thr right subtree
  Sat* root = node->getRight();

  // update the right child of the current node to be the left child of the new root
  node->setRight(root->getLeft());

  //update the new root's left child to be the current node
  root->setLeft(node);

  // update heights for the current node and new root
  int heightLeft = (node->getLeft() == nullptr) ? -1 : node->getLeft()->getHeight();
  int heightRight = (node->getRight() == nullptr) ? -1 : node->getRight()->getHeight();
  node->setHeight(max(heightLeft, heightRight) + 1);

  heightLeft = (root->getLeft() == nullptr) ? -1 : root->getLeft()->getHeight();
  heightRight = (root->getRight() == nullptr) ? -1 : root->getRight()->getHeight();
  root->setHeight(max(heightLeft, heightRight) + 1);

  return root;

}

// rotateRight(Sat* node)
// helper function which rotates a node to the right
Sat* SatNet::rotateRight(Sat* node){
  
  // retrieve the root of the left subtree
  Sat* root = node->getLeft();

  // update the left child of the current node to be the right child of the new root
  node->setLeft(root->getRight());

  // update the new root's right child to be the current node
  root->setRight(node);

  // update the heights for the current node and new root
  int heightLeft = (node->getLeft() == nullptr) ? -1 : node->getLeft()->getHeight();
  int heightRight = (node->getRight() == nullptr) ? -1 : node->getRight()->getHeight();
  node->setHeight(max(heightLeft, heightRight) + 1);

  heightLeft = (root->getLeft() == nullptr) ? -1 : root->getLeft()->getHeight();
  heightRight = (root->getRight() == nullptr) ? -1 : root->getRight()->getHeight();
  root->setHeight(max(heightLeft, heightRight) + 1);



  return root;

}

// clear
// The clear function deallocates all memory in the tree and makes it an empty tree.
void SatNet::clear(){

  // start clearung from the root
  helperClear(m_root);

  // set the root to nullptr indicating a tree is empty
  m_root = nullptr;
    
}

// helperClear( Sat* node)
// helper function for the clear function
void SatNet::helperClear(Sat* node){

  if(node == nullptr){

    // if the node is null return (base case)
    return;

  }

  // recursively clear the left and right subtrees
  helperClear(node->getLeft());
  helperClear(node->getRight());

  // deallocate memory from current node
  delete node;

}

// remove(int id)
// The remove function traverses the tree to find a node with the id and removes it from the tree.
void SatNet::remove(int id){

  // call the helperRemove function of the root and id
  m_root = helperRemove(m_root, id);


}

// helperRemove(Sat* treeRoot, int ID)
// helper function for the remove function
Sat* SatNet::helperRemove(Sat* treeRoot, int ID){

  // if tree is empty or the node that should be deleted is found(base case)
  if(treeRoot == nullptr){

    return treeRoot;

  }

  // determine if the id to be deleted is smaller which indicates that it lies in the left subtree
  if(ID < treeRoot->getID()){

    treeRoot->setLeft(helperRemove(treeRoot->getLeft(), ID));

  }

  // determine if the id to be deleted is larger which indicates that it lies in the right subtree
  else if (ID > treeRoot->getID()){

    treeRoot->setRight(helperRemove(treeRoot->getRight(), ID));

  }

  // if the id is the same as the root's id then this node will be deleted
  else {

    // node that contains only one child or none
    if(treeRoot->getLeft() == nullptr || treeRoot->getRight() == nullptr){

      Sat* tempNode = treeRoot->getLeft() ? treeRoot : treeRoot->getRight();

      // case of no child
      if(tempNode == nullptr){

	tempNode = treeRoot;
	treeRoot = nullptr;

      }  else {

	*treeRoot = *tempNode; // case of one child, copy the contents of the non emoty child

      }

      delete tempNode;

    } else {

      Sat* tempNode = minValue(treeRoot->getRight()); // retrieve the inorder successor (Node with two children)
      treeRoot->setID(tempNode->getID());
      treeRoot->setRight(helperRemove(treeRoot->getRight(), tempNode->getID()));

    }

  }

  // determine if tree has only one node
  if(treeRoot == nullptr){

    return treeRoot;

  }
  
  // update the height of the current node
  treeRoot->setHeight(1 + max(treeRoot->getLeft() ? treeRoot->getLeft()->getHeight() : 0, treeRoot->getRight() ? treeRoot->getRight()->getHeight() : 0 ));

  int nodeBalance = getBalance(treeRoot);// determine the balance factor

  // left left case
  if(nodeBalance > 1 && getBalance(treeRoot->getLeft()) >= 0){

      return rotateRight(treeRoot);

  }

  // Left Right Case
  if(nodeBalance > 1 && getBalance(treeRoot->getLeft()) < 0){
    treeRoot->setLeft(rotateLeft(treeRoot->getLeft()));
    return rotateRight(treeRoot);

  }

  // Right Right Case
  if(nodeBalance < -1 && getBalance(treeRoot->getRight()) <= 0){
    return rotateLeft(treeRoot);

  }

  //Right LeftCase
  if(nodeBalance < -1 && getBalance(treeRoot->getRight()) > 0){

    treeRoot->setRight(rotateRight(treeRoot->getRight()));
    return rotateLeft(treeRoot);

  }

  return treeRoot;

}

// getBalance(Sat* node)
// helper function which gets the balance factor of the tree
int SatNet:: getBalance(Sat* node){

  
  if(node == nullptr){

    // if the node is null, its balance factor is 0
    return 0;

  }

  int heightLeft = (node->getLeft()!= nullptr) ? node->getLeft()->getHeight() : 0;
  int heightRight = (node->getRight() != nullptr) ? node->getRight()->getHeight(): 0;

  // calculate the balance factor by subtracting the height of the right subtree from the height of the 
  // left subtree
  int treeBalance = heightLeft - heightRight;

  return treeBalance;

}

// minValue(Sat* node)
// helper function to retrieve the minimum node
Sat* SatNet::minValue(Sat* node){

  // determine if tree is empty
  if(node == nullptr){

    return nullptr;

  }

  // continue traversing to the left child until minimum node is found
  while(node->getLeft() != nullptr){

    node = node->getLeft();

  }

  return node;

}


void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

// listSatellites
// This function prints a list of all satellites in the tree to the standard output in the ascending order of IDs. The information for every Sat object will be printed in a new line.
void SatNet::listSatellites() const {

  // call the helperListSatellites function on the root
  helperListSatellites(m_root);
}

// helperListSatellites(Sat* node) 
// helper function for the ListSatellites function
void SatNet::helperListSatellites(Sat* node) const{

  if(node == nullptr){

    return; // base case (if the node is null)

  }

  // traverse left subtree
  helperListSatellites(node->getLeft());

  // print the satellite's information
  cout << node->getID() << ": " << node->getStateStr() << ": " << node->getInclinStr() << ": " << node->getAltStr() << endl;

  // traverse right rubtree
  helperListSatellites(node->getRight());

}

// setState(int id, STATE satelliteState)
//This function finds the node with id in the tree and sets its Sat::m_state member variable to state. If the operation is successful, the function returns true otherwise it returns false. For example, when the satellite with id does not exist in the tree the function returns false.
bool SatNet::setState(int id, STATE satelliteState){

  // call the helperSetState function on the root, id and satelliteState)
  return helperSetState(m_root, id, satelliteState);
}


// helperSetState(Sat* node, int id, STATE satelliteState)
// helper function for setState function
bool SatNet:: helperSetState(Sat* node, int id, STATE satelliteState){

  if(node == nullptr){

    // Satellite with the specified ID was node found
    return false;

  }

  if(id < node->getID()){

    // search the left subtree
    return helperSetState(node->getLeft(), id, satelliteState);

  } else if(id > node->getID()){

    // search right subtree
    return helperSetState(node->getRight(), id, satelliteState);

  } else {

    // found the satellite with the specified ID.
    node->setState(satelliteState);

    return true;

  }

}

// removeDeorbited
// This function traverses the tree, finds all satellites with DEORBITED state and removes them from the tree. The final tree must be a balanced AVL tree.
void SatNet::removeDeorbited(){

  // call the helperRemoveDeorbited function on the root
  m_root = helperRemoveDeorbited(m_root);
}

// helperRemoveDeorbited(Sat* node)
// helper function for the removeDeorbited function
Sat* SatNet::helperRemoveDeorbited(Sat* node){

  // determine if subtree is empty
  if(node == nullptr){

    return nullptr;

  }

  // recursivley remove deorbited satellites from the right and elft subtrees
  node->setLeft(helperRemoveDeorbited(node->getLeft()));
  node->setRight(helperRemoveDeorbited(node->getRight()));

  // determine if the current node is deorbited and if so remove it by returning the non null child
  if(node->getState() == DEORBITED){

    // replace the current node with its non null child
    if(node->getLeft() == nullptr){

      Sat* rightTreeChild = node->getRight();
      delete node;
      return rightTreeChild;

    } else if (node->getRight() == nullptr){

      Sat* leftTreeChild = node->getLeft();
      delete node;
      return leftTreeChild;

    }

    // both children are present results in replacing the current node with the minimum node value from the right subtree and remove the minimum node value from the right subtree
    Sat* nodeMin = minValue(node->getRight());
    node->setID(nodeMin->getID());
    node->setRight(helperRemoveDeorbited(node->getRight()));
  }

  // update height of current node
  int heightLeft = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1;
  int heightRight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1;
  node->setHeight(1 + max(heightLeft, heightRight));

  // rebalance tree
  int treeBalance = getBalance(node);

  // left heavy
  if(treeBalance > 1) {

    if(getBalance(node->getLeft()) >= 0){

      // left-left case
      return rotateRight(node);

    } else {

      // left-right case
      node->setLeft(rotateLeft(node->getLeft()));
      return rotateRight(node);

    }
  }

  // right heavy
  if(treeBalance < -1) {
    if(getBalance(node->getRight()) <= 0){

      // right-right case
      return rotateLeft(node);
    } else {

      // right-left case
      node->setRight(rotateRight(node->getRight()));
      return rotateLeft(node);

    }
  }

  return node;

}

// findSatellite(int id)
// This function returns true if it finds the node with id in the tree, otherwise it returns false.
bool SatNet::findSatellite(int id) const {

  // begin the search from root of the AVL tree
  return findSatelliteHelper(m_root, id);
}

// findSatelliteHelper(Sat* node, int id)
// helper function for the findSatellite function
bool SatNet::findSatelliteHelper(Sat* node, int id) const {

  // if node is nullptr, the satellite is not found (base case)
  if(node == nullptr){

    return false;

  }

  // determine if the node's ID matches the target ID
  if(node->getID() == id){

    return true;

  }

  // determine if the target ID is less than the current node's ID(saecrh in left subtree)
  if(id < node->getID()){

    return findSatelliteHelper(node->getLeft(), id);

  }

  // if the target ID is greater than the current node's ID, search in the right subtree
  return findSatelliteHelper(node->getRight(), id);

}

// Assignment operator
// This function overloads the assignment operator for the class SatNet. It creates an exact deep copy of the rhs.
const SatNet & SatNet::operator=(const SatNet & rhs){

  if(this != &rhs){

    // clear the current tree for the deep copy
    clear();

    if(rhs.m_root){

      // perform deep copy(recursively copy each node from rhs)
      m_root = new Sat(*rhs.m_root);

      // copy left subtree
      if(rhs.m_root->m_left){
	m_root->m_left = new Sat(*rhs.m_root->m_left);

      }

      // copy right subtree
      if(rhs.m_root->m_right){
	m_root->m_right = new Sat(*rhs.m_root->m_right);
      }
    }
  }

  return *this;
}


// countSatellites(INCLIN degree)
// This function traverses the tree, finds all satellites with the inclination specified by degree variable and returns the total number of 
// satellites with that inclination in the network.
int SatNet::countSatellites(INCLIN degree) const{

  // begin the counting process from the root
  return helperCountSatellites(m_root, degree);
}

// helperCountSatellites(Sat* node, INCLIN treeDegree)
// helpfer function for the countStaellites function
int SatNet:: helperCountSatellites(Sat* node, INCLIN treeDegree) const {

  // no nodes, so no satellites with the specific inclination
  if(node == nullptr){

    return 0;

  }

  // initalize the count
  int satelliteCount = 0;

  // determine if the current node's inclination matches the specifc degree
  if(node->getInclin() == treeDegree){

    satelliteCount = 1; // increment count by 1

  }

  // count the satellites witht the specific inclination in the left and right subtrees
  satelliteCount += helperCountSatellites(node->getLeft(), treeDegree);
  satelliteCount += helperCountSatellites(node->getRight(), treeDegree);

  return satelliteCount;

}
    
