//
//  Conductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>

#include "SmConductor.h"

namespace sm {
    
    Conductor::Conductor(int clckMd, int clckOffst)
    : clockMod(clckMd)
    , clockOffset(clckOffst) {}
    
    void Conductor::registerPlayer(Player* player)
    {
        players.push_back(player);
    }

}