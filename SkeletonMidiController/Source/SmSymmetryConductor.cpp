//
//  SmSymmetryConductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>

#include "SmSymmetryConductor.h"
#include "SmUserSkeleton.h"
#include "SmJointMotionData.h"

namespace sm {
    
    static const int SYMMETRY_CONDUCTOR_HEIGHT_THRESHOLD = 400;
    
    SymmetryConductor::SymmetryConductor(UserSkeleton* usr, int clckMd, int clckOffst)
    : user(usr)
    , Conductor(clckMd,clckOffst)
    {
        
    }
    
    void SymmetryConductor::clockCallback(int clock)
    {
        if ((clock % clockMod) == 0)
        {
            JointMotionData* leftData;
            JointMotionData* rightData;
            if((leftData = user->getJointMotionlDataObject(leftJointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                printf("error: jointtype not registered for user"); return;
            }
            if((rightData = user->getJointMotionlDataObject(rightJointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                printf("error: jointtype not registered for user"); return;
            }
            
            //float velocity = data->velocity[data->index].Y;

            
        }
    }
    
    
}