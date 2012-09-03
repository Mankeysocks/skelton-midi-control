//
//  SmSymmetryConductor.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmSymmetryConductor_h
#define SkeletonModuleApplication_vEarth_SmSymmetryConductor_h

#include "SmConductor.h"
#include "SmUserSkeleton.h"

namespace sm {
    
    class SymmetryConductor : public Conductor
    {
    public:
        
        SymmetryConductor(UserSkeleton* usr, int clckMd = 1, int clckOffst = 0);
        
        virtual void clockCallback(int clock);
        
        UserSkeleton* user;
        
        JointType leftJointType;
        
        JointType rightJointType;

    };
    
}

#endif
