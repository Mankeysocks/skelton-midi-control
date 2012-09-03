//
//  SmConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmPlayer_h
#define SkeletonModuleApplication_vEarth_SmPlayer_h

#include <vector>
#include "RtMidiEnumerations.h"
#include "SmPlayer.h"
#include "SmCallback.h"

namespace sm {
    
    // All player classes recieve a clock callback to drive their behavior
    // Player classes can decide how 
    class Conductor : public ClockCallback
    {
    public:
        Conductor(int clckMd = 1, int clckOffst = 0);
        
        // Conductors are driven through their clock callback
        virtual void clockCallback(int clock) = 0;
        
        void registerPlayer(Player* player);
        
    protected:
        
        // The clock mod is applied to the incoming clock counter to determine whether or not the signal should be ignored
        int clockMod;
        
        // The clock mod is subtracted from the result of the clock mod operation to determine whether or not the signal should be ignored 
        int clockOffset;
        
        // An array of the conductors players whose play, stop, and stopAll methods will be called when the conductor deems fit
        std::vector<Player*> players;
        
    };
    
}

#endif
