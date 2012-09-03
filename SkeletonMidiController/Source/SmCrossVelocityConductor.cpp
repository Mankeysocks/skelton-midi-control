//
//  SmCrossVelocityConductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/12/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <iostream>
#include <math.h>

#include "SmCrossVelocityConductor.h"
#include "SmUserSkeleton.h"
#include "SmJointMotionData.h"

#include "SmUmbrellaHeader.h"

namespace sm {
    
    static const int CRS_VELOCITY_CONDUCTOR_SPEED_THRESHOLD = 1500;
    static const int CRS_VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD = 110;
    
    CrossVelocityConductor::CrossVelocityConductor(UserSkeleton* usr, JointType jntTyp, JointType anchrJntTyp,int clkMd, int clkOffst)
    : RadialVelocityConductor(usr, jntTyp,anchrJntTyp,clkMd,clkOffst)
    {
        
    }

    void CrossVelocityConductor::clockCallback(int clock)
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
            float anchor_pos_x = anchor_data->position[anchor_data->index].X;
            float anchor_pos_y = anchor_data->position[anchor_data->index].Y;
            float anchor_pos_z = anchor_data->position[anchor_data->index].Z;
            //            printf("anchor pos : %f \n", anchor_pos_x);
            float diff_pos_x = pos_x - anchor_pos_x;
            float diff_pos_y = pos_y - anchor_pos_y;
            float diff_pos_z = pos_z - anchor_pos_z;
            //            printf("diff pos : %f \n", diff_pos_x);
            float anchor_var_distance = sqrt(diff_pos_x*diff_pos_x + diff_pos_y* diff_pos_y + diff_pos_z * diff_pos_z);
            //            printf("radial distance: %f \n", anchor_var_distance);
            float radial_comp_x = diff_pos_x / anchor_var_distance;
            float radial_comp_y = diff_pos_y / anchor_var_distance;
            float radial_comp_z = diff_pos_z / anchor_var_distance;
            
            // Obtain cross product
            float crs_comp_x = vel_y*radial_comp_z - vel_z*radial_comp_y; // c_1 = a_2 b_3 - a_3 b_2 
            float crs_comp_y = vel_z*radial_comp_x - vel_x*radial_comp_z; // c_2 = a_3 b_1 - a_1 b_3
            float crs_comp_z = vel_x*radial_comp_y - vel_y*radial_comp_x; // c_3 = a_1 b_2 - a_2 b_1 
            //            printf("cross product x: %f \n", crs_comp_x);
            
            float magn_crs_prod = sqrtf(crs_comp_x*crs_comp_x + crs_comp_y*crs_comp_y + crs_comp_z*crs_comp_z);
            printf("|cross product| : %f \n", magn_crs_prod);
            // We check for exceeding threshold, playing status, and that the x component is positive (or that the joint is moving downwards)
            if (magn_crs_prod > CRS_VELOCITY_CONDUCTOR_SPEED_THRESHOLD && crs_comp_x > 0 && !playing) {
                
                std::vector<Player*>::iterator it = players.begin();
                while (it != players.end()) {
                    (*it)->stop();
                    (*it)->play();
                    ++it;
                }
                playing = true;
            }
            else if (magn_crs_prod < CRS_VELOCITY_CONDUCTOR_MIN_SPEED_THRESHOLD && playing) {
                
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