//
//  SmVelocityConductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <math.h>
#include "SmVelocityConductor.h"
#include "SmUserSkeleton.h"
#include "SmJointMotionData.h"

namespace sm {
    
    static const int VELOCITY_CONDUCTOR_SPEED_THRESHOLD = 500;
    static const int VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD = 110;
    
    VelocityConductor::VelocityConductor(UserSkeleton* usr, JointType jntTyp, int clkMd, int clkOffSt)
    : user(usr)
    , jointType(jntTyp)
    , Conductor(clkMd,clkOffSt)
    , playing(false)
    {
        
    }

    void VelocityConductor::clockCallback(int clock)
    {
        if ((clock % clockMod) == 0)
        {
            JointMotionData* data;
            if((data = user->getJointMotionlDataObject(jointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                return;
            }
            
            float vel_x = data->velocity[data->index].X;
            float vel_y = data->velocity[data->index].Y;
            float vel_z = data->velocity[data->index].Z;
            
            float speed = sqrt(vel_x*vel_x + vel_y* vel_y + vel_z * vel_z);
            
            if (speed > VELOCITY_CONDUCTOR_SPEED_THRESHOLD && !playing) {
                
                std::vector<Player*>::iterator it = players.begin();
                while (it != players.end()) {
                    (*it)->stop();
                    (*it)->play();
                    ++it;
                }
                playing = true;
            }
            else if (speed < VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD && playing) {
                
                std::vector<Player*>::iterator it = players.begin();
                while (it != players.end()) {
                    (*it)->stop();
                    ++it;
                }
                playing = false;
            }

        }
    }


}