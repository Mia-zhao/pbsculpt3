/*
 *
 *
 */

#include "emptySubNode.h"

EmptySubNode::EmptySubNode(char port):
    SubNode(port)
{
    
}

void EmptySubNode::init(){}

void EmptySubNode::loop(){}

void EmptySubNode::getDeviceList(char* devList){}
