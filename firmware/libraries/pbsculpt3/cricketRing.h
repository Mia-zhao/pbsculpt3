/* Represents a Cricket Ring subnode in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __CRICKETRING_H
#define __CRICKETRING_H

class CricketRing: public SubNode {
    
    public:
        
        // Constructor
        CricketRing(int port);
        
        void init();
        void loop();
        
    protected:
        
        
        
}

#endif
