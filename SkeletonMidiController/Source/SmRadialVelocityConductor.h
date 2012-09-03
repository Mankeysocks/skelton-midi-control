//
//  SmRadialVelocityConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/11/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmRadialVelocityConductor_h
#define SkeletonModuleApplication_vEarth_SmRadialVelocityConductor_h

#include "SmCallback.h"
#include "SmUserSkeleton.h"
#include "SmVelocityConductor.h"
#include "SmPlayer.h"

namespace sm {
    
    class RadialVelocityConductor : public VelocityConductor
    {
    public:
        
        RadialVelocityConductor(UserSkeleton* usr, JointType jointType = HandRight, JointType anchrJntTyp = ShoulderRight,int clkMd = 1, int clkOffst = 0);
        
        virtual void clockCallback(int clock);
        
    protected:
        
        virtual void tellPlay();
        
        virtual void tellStop();
//        
//        float last_z_position; // keeps track of the last z position so it can know if it should stop playing
//        float last_y_position; // keeps track of the last y position so it can know if it should play other notes
//        float last_x_position; // keeps track of the last x position so it can know if it should play other notes
//        
        JointType anchorJointType;
    };
    
}


#endif
