  //
//  SmRhythmicTrigger.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/30/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//


#include <iostream>
#include <vector>

#include "SmUmbrellaHeader.h"
#include "SmRhythmicConductor.h"

namespace sm {
    
    RhythmicConductor::RhythmicConductor(std::vector<int> rhythm_vec, int clkMod, int clkOffst)
    : rhythm(rhythm_vec)
    , Conductor(clkMod,clkOffst)
    {
        rhythm_it = rhythm.begin();
    }
    
    void RhythmicConductor::clockCallback(int clock)
    {
        if ((clock % clockMod) == 0)
        {
            std::vector<Player*>::iterator it = players.begin();
            while (it != players.end()) {
                (*it)->stop();
                (*it)->play();
                ++it;
            }
            updateClockMod();
        }
    }
    
    void RhythmicConductor::updateClockMod(){
        clockMod = *rhythm_it;
        ++rhythm_it;
        if(rhythm_it == rhythm.end())
        {
            rhythm_it = rhythm.begin();
        }
    }
}

/*
 #ifdef __SM_DEBUG_CALLBACKS__
 printf("Reached Rhythmic Trigger callback...\n");
 #endif
 
 
 
 
 
 
 */