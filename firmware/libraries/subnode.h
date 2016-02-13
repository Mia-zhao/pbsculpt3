/* Represents a generic submodule for a Teensy Unit in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __SUBMODULE_H
#define __SUBMODULE_H

class SubNode {
    
    public:
        
        // Constructor
        SubNode();
        
        void init();
        void loop();
        
    protected:
        
        /* Print a list of available commands and their formats over the
         * defined communications channel
         */
        void cmdGetCommands();
        
        
}

#endif
