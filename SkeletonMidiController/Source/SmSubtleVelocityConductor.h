//
//  SmSubtleVelocityConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/10/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmSubtleVelocityConductor_h
#define SkeletonModuleApplication_vEarth_SmSubtleVelocityConductor_h

#include "SmCallback.h"
#include "SmUserSkeleton.h"
#include "SmVelocityConductor.h"
#include "SmPlayer.h"

namespace sm {
    
    class SubtleVelocityConductor : public VelocityConductor
    {
    public:
        
        SubtleVelocityConductor(UserSkeleton* usr, JointType jointType = HandRight);
        
        virtual void clockCallback(int clock);
        
    private:
        
        virtual void tellPlay();
        
        virtual void tellStop();
        
        int high_frames; // The number of frames that the Z velocity exceeds the threshold
        bool armed; // If the conductor is armed to play
        bool playing; // If the conductor is telling its players to play -- a note or notes have been triggered
        bool legato; // If the distance between the hand and shoulder remain at similar or greater distance then notes continue to play (must update last distance) -- otherwise, the note is stacatto and players turn off. Also in this condition, stop should be called after play
        float last_z_position; // keeps track of the last z position so it can know if it should stop playing
        float last_y_position; // keeps track of the last y position so it can know if it should play other notes
        float last_x_position; // keeps track of the last x position so it can know if it should play other notes
        
    };
    
}


#endif
