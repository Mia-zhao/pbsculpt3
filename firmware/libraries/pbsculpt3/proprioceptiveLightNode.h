/* Represents a Proprioceptive Light Node in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __PROPRIOCEPTIVELIGHTNODE_H
#define __PROPRIOCEPTIVELIGHTNODE_H

class ProprioceptiveLightNode {
    
    public:
        
        // Constructor
        ProprioceptiveLightNode(int serialNumber);
        
        void init();
        void loop();
        
    protected:
        int _serial;
        SubNode subnodes[6];
}

#endif
