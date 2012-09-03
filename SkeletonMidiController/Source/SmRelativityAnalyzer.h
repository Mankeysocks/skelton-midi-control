//
//  SmRelativityAnalyzer.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/10/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmRelativityAnalyzer_h
#define SkeletonModuleApplication_vEarth_SmRelativityAnalyzer_h

#include "SmCallback.h"
#include "SmJointMotionData.h"
#include "SmUserSkeleton.h"

namespace sm {
    
    class RelativityAnalyzer : public ClockCallback
    {
    public:
        
        RelativityAnalyzer(UserSkeleton* usr);
        
        virtual void clockCallback(int clock);
        
        UserSkeleton* user;
        
    };
    
}

#endif
