//
//  SmHeightMappedPitchPlayer.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/13/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmHeightMappedPitchPlayer_h
#define SkeletonModuleApplication_vEarth_SmHeightMappedPitchPlayer_h


#include <vector>

#include "SmCallback.h"
#include "SmPlayer.h"
#include "RtMidiEnumerations.h"
#include "SmUserSkeleton.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/bimap.hpp>

namespace sm {
    
    class HeightMappedPitchPlayer : public Player
    {
    public:
        
        HeightMappedPitchPlayer(UserSkeleton* skeleton, JointType jntTyp = HandRight, JointType anchorJointType = ShoulderRight, rt::Channel::Channel ch = rt::Channel::Channel1);
        
        virtual void play();
        
        JointType jointType;
        
        JointType anchorJointType;
        
    private:
        // This is our handle to user joint data that we use to decide model bounds
        UserSkeleton* user;
        
        // Normally this is the last note that was played (the only exception is the very first note)
        Note lastNote;
        
        // The last height of var joint when played
        float lastHeight;
        
        // The last distance from the anchor to var joint
        float lastDist;
    };
    
}

#endif
