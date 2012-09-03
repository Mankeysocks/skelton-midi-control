//
//  OpenNiControl.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/18/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleTest_Header_h
#define SkeletonModuleTest_Header_h

#include <vector>

#include "SmUmbrellaHeader.h"
#include "SmUserSkeleton.h"

namespace sm{
    
    // this initializes and starts skeletal tracking
    int initializeOpenNiSkeletalTracking();
    
    // safe release of openni resources
    void CleanupExit();
    
}

#endif
