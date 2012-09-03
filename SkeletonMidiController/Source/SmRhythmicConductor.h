//
//  SmRhythmicTrigger.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/30/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_SmRhythmicTrigger_h
#define SkeletonModuleApplication_SmRhythmicTrigger_h

#include "SmCallback.h"
#include "SmConductor.h"
#include "SmPlayer.h"

namespace sm {
    
    class RhythmicConductor : public Conductor
    {
    public:
        
        RhythmicConductor(std::vector<int> rhythm_vec, int clkMd = 1, int clkOffst = 0);
        
        virtual void clockCallback(int clock);
        
        std::vector<int> rhythm;
        
        std::vector<int>::iterator rhythm_it;
        
    private:
        
        void updateClockMod();
        
    };
    
}
#endif
