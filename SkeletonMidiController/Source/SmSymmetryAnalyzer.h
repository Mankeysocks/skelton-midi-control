//
//  SymmetryDetector.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SymmetryDetector_h
#define SkeletonModuleApplication_vEarth_SymmetryDetector_h


#include "SmCallback.h"
#include "SmJointMotionData.h"
#include "SmUserSkeleton.h"

namespace sm {
    
    class SymmetryAnalyzer : public ClockCallback
    {
    public:
        
        SymmetryAnalyzer(UserSkeleton* usr, JointType lftJntTyp = HandLeft, JointType rghtJntTyp = HandRight);
        
        virtual void clockCallback(int clock);
        
        UserSkeleton* user;
        
        JointType leftJointType;
        
        JointType rightJointType;

    };
}

#endif
