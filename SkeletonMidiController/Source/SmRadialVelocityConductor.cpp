//
//  SmRadialVelocityConductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/11/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <math.h>

#include "SmRadialVelocityConductor.h"
#include "SmUserSkeleton.h"
#include "SmJointMotionData.h"

#include "SmUmbrellaHeader.h"

namespace sm {
    
    static const int RAD_VELOCITY_CONDUCTOR_SPEED_THRESHOLD = 1000;
    static const int RAD_VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD = 110;
    
    RadialVelocityConductor::RadialVelocityConductor(UserSkeleton* usr, JointType jntTyp, JointType anchrJntTyp,int clkMd, int clkOffst)
    : VelocityConductor(usr, jntTyp,clkMd,clkOffst)
    , anchorJointType(anchrJntTyp)
    {
        
    }
    
    void RadialVelocityConductor::tellPlay()
    {
        std::vector<Player*>::iterator it = players.begin();
        while (it != players.end()) {
            (*it)->play();
            ++it;
        }
    }
    void RadialVelocityConductor::tellStop()
    {
        std::vector<Player*>::iterator it = players.begin();
        while (it != players.end()) {
            (*it)->stop();
            ++it;
        }
    }
    
    void RadialVelocityConductor::clockCallback(int clock)
    {
        if ((clock % clockMod) == 0)
        {
            JointMotionData *var_data, *anchor_data;
            if((var_data = user->getJointMotionlDataObject(jointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                return;
            }
            if((anchor_data = user->getJointMotionlDataObject(anchorJointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                return;
            }
            
            float vel_x = var_data->velocity[var_data->index].X;
            float vel_y = var_data->velocity[var_data->index].Y;
            float vel_z = var_data->velocity[var_data->index].Z;
            
            float pos_x = var_data->position[var_data->index].X;
            float pos_y = var_data->position[var_data->index].Y;
            float pos_z = var_data->position[var_data->index].Z;
//            printf("pos : %f \n", pos_x);
            float anchor_pos_x = anchor_data->position[var_data->index].X;
            float anchor_pos_y = anchor_data->position[var_data->index].Y;
            float anchor_pos_z = anchor_data->position[var_data->index].Z;
//            printf("anchor pos : %f \n", anchor_pos_x);
            float diff_pos_x = pos_x - anchor_pos_x;
            float diff_pos_y = pos_y - anchor_pos_y;
            float diff_pos_z = pos_z - anchor_pos_z;
//            printf("diff pos : %f \n", diff_pos_x);
            float anchor_var_distance = sqrt(diff_pos_x*diff_pos_x + diff_pos_y* diff_pos_y + diff_pos_z * diff_pos_z);
//            printf("radial distance: %f \n", anchor_var_distance);

            float dot_radial_vel = vel_x*diff_pos_x + vel_y*diff_pos_y + vel_z*diff_pos_z;
            float radial_vel_comp = dot_radial_vel / anchor_var_distance;
            
//            printf("radial velocity: %f \n", radial_vel_comp);
            if (radial_vel_comp > RAD_VELOCITY_CONDUCTOR_SPEED_THRESHOLD && !playing) {
                
                std::vector<Player*>::iterator it = players.begin();
                while (it != players.end()) {
                    (*it)->stop();
                    (*it)->play();
                    ++it;
                }
                playing = true;
            }
            else if (radial_vel_comp < RAD_VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD && playing) {
                
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







