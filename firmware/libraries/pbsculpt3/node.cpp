/*
 *
 *
 */

#include "node.h"
#include "subnode.h"

Node::Node(int serialNumber)/*: 
    _serialNumber(serialNumber), subnodes(new SubNode[6])*/
{
    //this->_serialNumber = serialNumber;
}

void Node::init(){
    // Call init for all of the subnodes
    for(int i=0; i < N_SUBNODES; i++){
        subnodes[i]->init();
    }
}

void Node::loop(){
    // Call loop for all of the subnodes
    for(int i=0; i < N_SUBNODES; i++){
        subnodes[i]->loop();
    }
}
