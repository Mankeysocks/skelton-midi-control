//
//  SmCrossVelocityConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/12/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmCrossVelocityConductor_h
#define SkeletonModuleApplication_vEarth_SmCrossVelocityConductor_h

#include "SmCallback.h"
#include "SmUserSkeleton.h"
#include "SmRadialVelocityConductor.h"
#include "SmPlayer.h"

namespace sm {
    
    class CrossVelocityConductor : public RadialVelocityConductor
    {
    public:
        
        CrossVelocityConductor(UserSkeleton* usr, JointType jointType = HandRight, JointType anchrJntTyp = ShoulderRight,int clkMd = 1, int clkOffst = 0);
        
        virtual void clockCallback(int clock);
        
    };
    
}


#endif
