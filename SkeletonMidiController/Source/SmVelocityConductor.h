//
//  SmVelocityConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmVelocityConductor_h
#define SkeletonModuleApplication_vEarth_SmVelocityConductor_h


#include "SmCallback.h"
#include "SmUserSkeleton.h"
#include "SmConductor.h"
#include "SmPlayer.h"

namespace sm {
    
    class VelocityConductor : public Conductor
    {
    public:
        
        VelocityConductor(UserSkeleton* usr, JointType jointType = HandRight,  int clkMd = 1, int clkOffSt = 0);
        
        virtual void clockCallback(int clock);

        UserSkeleton* user;
        
        JointType jointType;
        
    protected:
        
        bool playing;
    };
    
}


#endif
