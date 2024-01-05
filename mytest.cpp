/*****************************************
** File:    mytest.cpp
** Project: CMSC 341 Project 2, Fall 2023
** Author:  Jervon Drakes
** Date:    10/24/2023
** E-mail:  jdrakes1@umbc.edu
** 
**
** This file contains the mytest.cpp file for Project 2.
** This program implements functions for the tester class to 
** implement numerous test cases for the functions in satnet.cpp
**
**
**
***********************************************/
#include "satnet.h"


class Tester{
public:
  // Function: SatNet::insert(const Sat& satellite)
  // Case: Tests the insert function for an normal case 
  // Expected Result: The function should return true as the satellite should be inserted into the network 
  bool testInsertNormal(){

    // create a SatNet insatnce
    SatNet bstSat;

    // create a satellite with an ID and insert it into the SatNet instance
    Sat mySatellite1(12345, MI208, I53, ACTIVE);
    bstSat.insert(mySatellite1);

    // determine if the satellites were inserted
    return bstSat.findSatellite(12345); 

  }

  //Function: SatNet::insert(const Sat& satellite)
  // Case: Tests the insert function for an edge case 
  // Expected Result: The function should return true as the satellite with the minimum allowed ID should be inserted into the network 
  bool testEdgeInsert(){

    // create a SatNet instance
    SatNet bstSat;

    // create a satelllite with the minimum allowed ID and insert it into the SatNet instance
    Sat mySatellite(MINID, MI208, I53, ACTIVE);
    bstSat.insert(mySatellite);
   
    // determine if the satellite with the ID MINDID was inserted and can be found
    return bstSat.findSatellite(MINID);
  }

  //Function: SatNet::insert(const Sat& satellite)
  // Case: Tests the insert function for an error case 
  // Expected Result: The function should return false as the first insertion should succeed while the second insertion should not be allowed 
  bool testErrorInsert(){

    SatNet bstSat;

    Sat mySatellite(12345, MI208, I53, ACTIVE);

    bstSat.insert(mySatellite);
  

    // attempt to insert a satellite with the same ID (should not be inserted)
    Sat mySatellite2(12345, MI215, I70, DECAYING);
    bstSat.insert(mySatellite2);

    // determine if the third satellite was not insert
    return !bstSat.findSatellite(12345);

  }

  // Function to test the tree is balanced after a decent number of insertions
  // Case: Tests whether the tree is balanced after a number of insertions 
  // Expected Result: The function should return true if the AVL tree is balanced after insert 300 insertions
  // meaning that the balance mechanism works property 
  bool balanceTest(){

    SatNet sat;

    int insertions = 300;
    for(int id = MINID; id < MINID + insertions; id++){
      Sat satellite(id, MI208, I48, ACTIVE);
      sat.insert(satellite);

    }

    // determine if the tree is balanced after insertions
    return treeBalanced(sat, sat.m_root);

  }
 
    

  // Test the AVL tree for preserving the BST property
  // Case: Tests whether the BST property is preserved after all insertions
  // Expected result: The funcion should return true as the BST property is preserved after inserting a number of satellites
  // meaning that the structure remains a valid BST
  bool bstPropertyTest(SatNet &Satellite, int insertions){


    for(int i = 0; i < insertions; i++){
      Sat mySatellite(i);
      Satellite.insert(mySatellite);

    }

    // determine if the BST property is preserved after insertions
    return propertyBST(Satellite.m_root);
  }


		       
  // Helper function to test the tree is balanced after a decent number of insertions
  // Case: Tests whether the tree is balanced after a number of insertions
  // Expected Result: The function should return true if the balnce factor of each node in the AVL tree is within the range -1 and 1 which
  // indicates that the tree is balanced 
  bool treeBalanced(SatNet& netSat, Sat* node){

    // SatNet mySatellite;
    if(node == nullptr){

      return true; // empty tree always balanced

    }

    int bstBalance = netSat.getBalance(node);// calculate the balance factor of current node

    // determine if balance factor is within range
    if( bstBalance < -1 || bstBalance > 1){

      return false;

    }

    // check the balance of both subtrees
    bool leftTree = treeBalanced(netSat, node->getLeft());
    bool rightTree = treeBalanced(netSat, node->getRight());


    return leftTree && rightTree;
    
    // return (bstBalance >= - 1 || bstBalance <= 1 ) && treeBalanced(node->getLeft()) && treeBalanced(node->getRight());

      

  }

  // Helper function to test the AVL tree for preserving the BST property
  // Case: Tests whether the BST property is preserved after all insertions
  // Recursively validates that the left child of each node has a smaller ID and the right child has a greater ID
  // than the current node
  // Expected result: The funcion should return true as the BST property is preserved after inserting a number of satellites
  // meaning that the structure remains a valid BST
  bool propertyBST(Sat* node){

    if(node == nullptr){

      // empty tree is always a BST
      return true;

    }

    bool leftValidTree = true;
    bool rightValidTree = true;

    if(node->getLeft() != nullptr){
            
      leftValidTree = (node->getLeft()->getID() < node->getID()) && propertyBST(node->getLeft()); // determine if the left child has a smaller ID than the current node
    }

    if(node->getRight()){
      rightValidTree = (node->getRight()->getID() > node->getID()) && propertyBST(node->getRight()); // determine if the right child has a greater ID than the current node
		   
    }

    // return true only if both left and right subtrees are valid BSTs
    return leftValidTree && rightValidTree;
    


  }
  // Function: SatNet::remove(int id)
  // Case: Test the normal case of the remove function
  // Expected Result: The function should return true if the satellite was successfully removed
  bool testNormalRemove(){

    SatNet satellite;

    // insert multple satellites
    satellite.insert(Sat(12345));
    satellite.insert(Sat(54321));
    satellite.insert(Sat(11111));
    satellite.insert(Sat(99999));

    // remove a satellite
    satellite.remove(54321);

    // determine if the satellite was removed
    bool removed = !satellite.findSatellite(54321);

    return removed;

  }

  // Function: SatNet::remove(int id)
  // Case: Test the edge case for the remove function
  // Expected Result: The function should return true if the tree becomes empty after removing the single satellite
  bool testEdgeCaseRemove(){

    SatNet sat;

    sat.insert(Sat(12345));

    // remove the only satellite
    sat.remove(12345);

    // determine if the tree is empty
    bool empty = !sat.findSatellite(12345);

    return empty;

  }

  // Function to test the if the tree reamins balances after multiple removals
  // Case: Tests whether the tree reamins balances after multiple removals
  //Expected Result: The function should return true if the tree is still balanced after 
  // a number of removals vareifying that the self balancing mechanism works
  bool testBalanceTree(){

    SatNet sat;

    // insert 300 nodes
    for(int i = 1; i <= 300; i++){
      int ID = MINID + i;
      INCLIN inclin = static_cast<INCLIN>(i % 4);// rotate inclin between I48, I53, I70 AND I97
      sat.insert(Sat(ID, DEFAULT_ALT, inclin, DEFAULT_STATE));
      

    }

    // remove 150 nodes
    for(int i = 1; i <= 150; i++){
      int ID = MINID + i;
      sat.remove(ID);
      //sat.remove(i);
    }

    // determine if the tree is balanced
    return treeBalanced(sat, sat.m_root);

  }

  // Function to tests that the BST property is preserved after removing satellites
  // Case: Tests whether the BST property is preserved after removals
  // Expected Result: The function should return treu as the BST property is preserved after removing a number of satellites
  bool bstPropertyPreservedTest(){

    //Create a SatNet instance
    SatNet sat;

    sat.insert(Sat(5, DEFAULT_ALT, DEFAULT_INCLIN, DEFAULT_STATE));
    sat.insert(Sat(3, DEFAULT_ALT, DEFAULT_INCLIN, DEFAULT_STATE));
    sat.insert(Sat(2, DEFAULT_ALT, DEFAULT_INCLIN, DEFAULT_STATE));
    sat.insert(Sat(7, DEFAULT_ALT, DEFAULT_INCLIN, DEFAULT_STATE));
    sat.insert(Sat(4, DEFAULT_ALT, DEFAULT_INCLIN, DEFAULT_STATE));

    // remove some satellites
    sat.remove(4);
    sat.remove(5);

    // test whether BST property is removed
    bool preserved = propertyBST(sat.m_root);

    return preserved;

  }

  //Function: SatNet::removeDeorbited()
  // Case: Test the removeDeorbited functionality
  // Expected Result: The function should return true if the deorbited satellites are sucessfully removed
  bool testRemoveDeorbited(){

    SatNet sat;

    // insert multiple satellites with different states
    sat.insert(Sat(10001, DEFAULT_ALT, DEFAULT_INCLIN, DEORBITED));
    sat.insert(Sat(10002, DEFAULT_ALT, DEFAULT_INCLIN, ACTIVE));
    sat.insert(Sat(10003, DEFAULT_ALT, DEFAULT_INCLIN, DEORBITED));
    sat.insert(Sat(10004, DEFAULT_ALT, DEFAULT_INCLIN, ACTIVE));

    // call removeDeorbited
    sat.removeDeorbited();


    // determine if the deorbited satellites are removed
    bool removeDeorbit = !sat.findSatellite(10001) && !sat.findSatellite(10003);
    return removeDeorbit;
  }

  //Function: SatNet::countSatellites(INCLIN degree) const
  // Test the countSatellites functionality
  // Expected Result: The function should return true if the count of staellites 
  // with the specific inclination is correct
  bool testCountSatellites(){

      SatNet sat;

      // insert satellites withvarious inclinations
      sat.insert(Sat(10000, DEFAULT_ALT, I48, ACTIVE));
      sat.insert(Sat(10001, DEFAULT_ALT, I53, ACTIVE));
      sat.insert(Sat(10002, DEFAULT_ALT, I70, ACTIVE));
      sat.insert(Sat(10003, DEFAULT_ALT, I53, ACTIVE));

      // count satellites with a specific inclination
      int satelliteCount = sat.countSatellites(I53);

      // test if the count is correct
      bool countCorrect = (satelliteCount == 2);

      return countCorrect;

  }

  // Function: SatNet::findSatellite(int id) const
  // Case: Test the findSatellite functionality for a normal case
  // Expected Result: The fucntion should return true as the satellite with the specific
  // id is found
  bool testNormalFindSatellite(){

    SatNet sat;

    // create three objects and insert them into the SatNet instance
    Sat mySatellite(10000, MI208, I48, ACTIVE);
    Sat mySatellite1(10002, MI208, I53, DEORBITED);
    Sat mySatellite3(10003, MI340, I70, ACTIVE);

    sat.insert(mySatellite);
    sat.insert(mySatellite1);
    sat.insert(mySatellite3);

    // attempt to find a satellite by its ID and determine if it is found
    bool result = sat.findSatellite(10003);

    return result;

  }

  // Function: SatNet::findSatellite(int id) const
  // Case: Test the findSatellite functionality for an error case
  // Expected Result: The function should return true as the satellite with the specified
  // id can't be found
 bool testErrorFindSatellite(){

    SatNet sat;

    // create three objects and insert them into the SatNet instance
    Sat mySatellite(10000, MI208, I48, ACTIVE);
    Sat mySatellite1(10002, MI208, I53, DEORBITED);
    Sat mySatellite3(10003, MI340, I70, ACTIVE);
    sat.insert(mySatellite);
    sat.insert(mySatellite1);
    sat.insert(mySatellite3);

    // attempt to find a satellite by an ID that doesnt exiist in the
    // SatNet instance and determine if the function returns false as expected
    bool result = sat.findSatellite(10005);

    return !result;
  }

  // Function: SatNet::operator=(const SatNet & rhs)
  // Case: Test the assignment operator for a normal case
  // Expected Result: The function shoudl return true as both objects contain the one staellite with
  // the same inclination verifying that the assignment operator correctly copied the 
  // contents of one object to another
 bool testNormalAssignmentOperator(){
    

    SatNet sat1;

    // create two satellite objects and insert them into the first SatNet instance
    Sat mySatellite(10000, MI208, I48, ACTIVE);
    Sat mySatellite1(10002, MI208, I53, DEORBITED);
    sat1.insert(mySatellite);
    sat1.insert(mySatellite1);
    

    SatNet sat2;

    // create two more staellite objects and insert them into the second SatNet instance
    Sat mySatllite2(10003, MI340, I70, ACTIVE);
    Sat mySatellite3(10004, MI350, I97, ACTIVE);
    sat2.insert(mySatllite2);
    sat2.insert(mySatellite3);

        
    // copy the contents of sat1 to sat2
    sat2 = sat1;

    

    // count the number of satellites in each SatNet with a specific inclination
    int satCount1 = sat1.countSatellites(I48);
    int satCount2 = sat2.countSatellites(I48);

    // determine if the assignment operator correctly copied the contents
    // and the satellite counts in both SatNet instances match
    return satCount1 == 1 && satCount2 == 1;
      
  }

  // Function: SatNet::operator=(const SatNet & rhs)
  // Case: Test the assignment operator for an error case
  // Expected Result: The function should true as both objects are empty after assignment
  bool ErrorAssignmentOperator(){

    // create two empty SatNet objects
    SatNet emptySat1;
    SatNet emptySat2;

    // use the assignment operator to copy the objects to each othwr
    emptySat1 = emptySat2;


    // determine if both objects are empty
    return(emptySat1.findSatellite(MINID) == emptySat2.findSatellite(MINID));

  }

  // Function to Prove that the removal operation performs in O(log n
  // Case: Test that the removal operation performs in O(log n)
  // Expected result: The function should return true if the time taken to remove
  // a satellite is less than  0.001 seconds. This demonstrates that the removal
  // operation has acceptable performance( time complexit of O(log n))
 bool performanceRemoval(){

    // create a SatNet object with a large number of staellites
    SatNet sat1;
    int sizeArray = 100000;
    int array[100001];
        
    // insert a large number of staellites into the Satnet object
    for(int i = 0; i < sizeArray; ++i){
      int satID = i + 10001;
      array[i] = satID;
      Sat mySatellite(satID, MI215, I48, ACTIVE);
      sat1.insert(mySatellite);
    }

    // select a satellite ID to remove
    int removeID = array[sizeArray / 2];

    // measure the time taken to remove a satellite by its ID
    clock_t startTime = clock();
    sat1.remove(removeID);
    clock_t endTime = clock();
    double time = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    // The time complexity is O(log n)
    // Test that the time taken is within an acceptable range
    bool timeSuccess = time < 0.001;

      
    return timeSuccess;

  }

  // Function to prove that insertion performs in O(log n)
  // Case: Tests whether the insertion performs in O(log n)
  // Expected Result: The function should return tre if the ratio of the second insertion
  // time to the first insertion time is within an acceptable range. This confirms that the 
  // insertion performance meets the expected ratio.
  bool InsertPerformance(){

    // number of nodes for the first and second sets
    int nodesNum = 1000;
    int nodesNum1 = 2000;

    SatNet sat1;

    // record the start time using clock() for performance measurement
    clock_t startTime = clock();

    // insert the first set of nodes
    for(int i = 1; i <= nodesNum; i++){

      sat1.insert(Sat(i));

    }

    clock_t endTime = clock();

    // calculate the time for the first set of insertions
    double firstTime = double(endTime - startTime) / CLOCKS_PER_SEC; // measure insertion time for 1000 nodes

    SatNet sat2;

    // record the start time for the second set of insertions
    clock_t start = clock();

    // Insert the second set of nodes
    for(int i = 1; i <nodesNum1; i++){
      sat2.insert(Sat(i));

    }

    clock_t end = clock();
    double secondTime = double(end - start) / CLOCKS_PER_SEC; // measure insertion time for 2000 nodes

    // Expected ratio for an AVL tree is log(2n)/log(n) = 1
    // allow an absolute error of 0.4
    double ratio = 1.0;
    double minRatio = ratio - 0.4;
    double maxRatio = ratio + 0.4;

    // calculate the actual ratio of the second set insertion time
    // to twice the first set insertion time 
    double ratioTest = secondTime / (2 * firstTime);
    
    // determine if the actual ratio falls within the acceptable range
    bool passedTest = ratioTest >= minRatio && ratioTest <= maxRatio;

    return passedTest;

  }
};
  int main() {

    SatNet mySatelliteNet;

        
    Tester bstTester;

    cout << "Testing Insert Normal Case: " << endl;
    
    // ouputs the result of the Insert Normal Case
    if(bstTester.testInsertNormal()){

      cout << "Insert Normal Case passed" << endl;

    } else {

      cout << "Insert Normal Case failed" << endl;

    }

    cout << "                  " << endl;

    cout << "Testing Insert Edge Case: " << endl;
    
    // outputs the result of the Insert Edge Case
    if(bstTester.testEdgeInsert()){

      cout << "Insert Edge Case passed" << endl;

    } else {

      cout << "Insert Edge Case failed" << endl;

    }
    
    cout << "                  " << endl;

    cout << "Testing Insert Error Case: " << endl;
    
    // outputs the result of the Insert Error Case
    if(bstTester.testErrorInsert()){

      cout << "Insert Error Case failes" << endl;

    } else {

      cout << "Insert Error Case passed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing Preservation of BST property: " << endl;
    
    // outputs the result of the BST Property Test
    if(bstTester.bstPropertyTest(mySatelliteNet, 300)){

      cout << "The AVL tree preserves the BST property" << endl;
      } else {
      cout << "The AVL tree does not preserve the BST property" << endl;

      }

    
    cout << "                  " << endl;

    cout << "Testing if the AVL tree is balance: " << endl;
    
    // outputs the result of the Balance Test
    if(bstTester.balanceTest()){

      cout << "The AVL tree is balanced" << endl;

    } else {

      cout << "The AVL tree is not balanced" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing Remove Normal Case: " << endl;

    // outputs the result of the Remove Normal Case    
    if(bstTester.testNormalRemove()){

      cout << "Remove Normal Case passed" << endl;
    }
    else {

      cout << "Remove Normal Case failed" << endl;
    }

    
    cout << "                  " << endl;

    cout << "Testing Remove Edge Case: " << endl;
    
    // outputs the result of the Remove Edge Case
    if(bstTester.testEdgeCaseRemove()){

      cout <<"Remove Edge Case passed" << endl;

    } else {

      cout << "Remove Edge Case failed" << endl;

    }

    
    cout << "                  " << endl;

    cout <<"Testing the balance of the tree after multiple removals: " << endl;
    
    // outputs the result of the Balance Test after multiple removals
    if(bstTester.testBalanceTree()){

      cout << "The tree is balanced after multiple removals" << endl;

    } else {

      cout << "The tree is not balanced after multiple removals" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing the BST property after multiple removals: " << endl;
    
    // outputs the result of the BST Property Preserved Test after multiple removals
    if(bstTester.bstPropertyPreservedTest()){

      cout <<"The BST property is preserved after removal" << endl;

    } else {

      cout <<"The BST property is not preserved after removal" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing RemoveDeorbited functionality: " << endl;
    
    // outputs the result of testing the RemoveDeorbited functionality
    if(bstTester.testRemoveDeorbited()){

      cout << "Deorbited satellites are removed" << endl;

    } else {

      cout << "Deorbited satellites are removed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing CountSatellites functionality: " << endl;

    // outputs the result of testing the CountSatellites functionality
    if(bstTester.testCountSatellites()){

      cout << "Count Satellites works correctly " << endl;

    } else {

      cout << "Count Satellites does not work correctly" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing FindSatellites Normal Case: " << endl;
    
    // outputs the result of the FindSatellite Normal Case
    if(bstTester.testNormalFindSatellite()){

      cout << "Find Satellite Normal Case passed" << endl;

    } else {

      cout << "Find Satellite Normal Case failed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing FindSatellites Error Case: " << endl;
    
    // outputs the result of the FindSatellite Error Case
   if(bstTester.testErrorFindSatellite()){

      cout << "Find Satellite Error Case passed" << endl;

    } else {

      cout << "Find Satellite Error Case failed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing Assignment Operator Normal Case: " << endl;
    
    // outputs the result of the Assignment Operator Normal Case
    if(bstTester.testNormalAssignmentOperator()){

      cout << "Assignment Operator Normal Case passed" << endl;

    } else {

      cout << "Assignment Operator Normal Case failed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing Assignment Operator Error Case: " << endl;
    
    // outputs the result of the Assignment Operator Error Case
    if(bstTester.ErrorAssignmentOperator()){

      cout << "Assignment Operator Error Case passed" << endl;

    } else {

      cout << "Assignment Operator Error Case failed" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing the performance of removal operation: " << endl;
    
    // outputs the result of the testing the performance of the removal operation
   if(bstTester.performanceRemoval()){
      cout << "The removal operation performed in O(log n) time " << endl;

    } else {

      cout << "The removal operation did not perform in O(log n) time" << endl;

    }

    
    cout << "                  " << endl;

    cout << "Testing the performance of the insertion operation: " << endl;
    
    //outputs the result of the testing the performance of the insert operation
    if(bstTester.InsertPerformance()){

      cout << "Insertion time test passed. AVL insertion is O(log n)." << endl;

    } else {

      cout << "Insertion time test failed. AVL insertion may not be O(log n)." << endl;

    }

    return 0;

  }

