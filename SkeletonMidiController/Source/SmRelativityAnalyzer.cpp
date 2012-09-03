//
//  SmRelativityAnalyzer.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/10/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//


//#include <iostream>

#include "SmRelativityAnalyzer.h"
#include "SmUserSkeleton.h"
//#include "SmJointMotionData.h"

namespace sm {
    
    static const int SYMMETRY_DETECTOR_HEIGHT_THRESHOLD = 400;
    
    RelativityAnalyzer::RelativityAnalyzer(UserSkeleton* usr)
    : user(usr)
    {
        
    }
    
    void RelativityAnalyzer::clockCallback(int clock)
    {        
        Vector3 axis_point_1; // TODO: These will exist somehow
        Vector3 axis_point_2;
        
        Vector3 relative_axis(axis_point_2.X - axis_point_1.X, axis_point_2.Y - axis_point_1.Y, axis_point_2.Z - axis_point_1.Z);
        float distance = relative_axis.X*relative_axis.X+relative_axis.Y*relative_axis.Y+relative_axis.Z*relative_axis.Z;
        relative_axis.X = relative_axis.X/distance;
        relative_axis.Y = relative_axis.Y/distance;
        relative_axis.Z = relative_axis.Z/distance;
        
        JointMotionData* leftData;
        JointMotionData* rightData;
        /*
        if((leftData = user->getJointMotionlDataObject(leftJointType)) == NULL)
        {   // The data for the joint type was never registered with the user
            printf("error: jointtype not registered for user"); return;
        }
        if((rightData = user->getJointMotionlDataObject(rightJointType)) == NULL)
        {   // The data for the joint type was never registered with the user
            printf("error: jointtype not registered for user"); return;
        }
        
        float  left_x_velocity   =  leftData->velocity[ leftData->index].X;
        float right_x_velocity   = rightData->velocity[rightData->index].X;
        float  left_y_velocity   =  leftData->velocity[ leftData->index].Y;
        float right_y_velocity   = rightData->velocity[rightData->index].Y;
        float  left_z_velocity   =  leftData->velocity[ leftData->index].Z;
        float right_z_velocity   = rightData->velocity[rightData->index].Z;
        
        float left_x_relative_axis_velocity = left_x_velocity
        */
    }
}