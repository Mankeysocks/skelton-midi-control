//
//  SubtleVelocityConductor.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/10/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <math.h>

#include "SmSubtleVelocityConductor.h"
#include "SmUserSkeleton.h"
#include "SmJointMotionData.h"

#include "SmUmbrellaHeader.h"

namespace sm {
    
    static const int Z_HIGH_VELOCITY_CONDUCTOR_THRESHOLD = -1700;
    static const int Z_LOW_VELOCITY_CONDUCTOR_THRESHOLD = 300;
    static const int Z_HIGH_FRAME_THRESHOLD = 3;
    static const int STOPPLAYING_DISTANCE_THRESHOLD = 90;
    
    SubtleVelocityConductor::SubtleVelocityConductor(UserSkeleton* usr, JointType jntTyp)
    : VelocityConductor(usr, jntTyp,1,0)
    , armed(false)
    , playing(false)
    , legato(false)
    , high_frames(0)
    {
        
    }
    
    void SubtleVelocityConductor::tellPlay()
    {
        std::vector<Player*>::iterator it = players.begin();
        while (it != players.end()) {
            (*it)->play();
            ++it;
        }
    }
    void SubtleVelocityConductor::tellStop()
    {
        std::vector<Player*>::iterator it = players.begin();
        while (it != players.end()) {
            (*it)->stop();
            ++it;
        }
    }
    
    void SubtleVelocityConductor::clockCallback(int clock)
    {
        if ((clock % clockMod) == 0)
        {
            JointMotionData* data;
            if((data = user->getJointMotionlDataObject(jointType)) == NULL)
            {   // The data for the joint type was never registered with the user
                return;
            }
            
            float velocity = data->velocity[data->index].Z;
            float position = data->position[data->index].Z;
            
#ifdef __SM_DEBUG_SUBTLE__
            printf("Z Vel Pos: %f   %f\n", velocity, position);  
#endif
            if (velocity < Z_HIGH_VELOCITY_CONDUCTOR_THRESHOLD) // if they are moving fast enough, then increment high frame count
            {
                ++high_frames;
                if(high_frames > Z_HIGH_FRAME_THRESHOLD)
                {
                    armed = true; // if it moves forward fast enough long enough, arm it
                }
                return;
            }
            else if(armed)
            {
                if (fabs(velocity) < Z_LOW_VELOCITY_CONDUCTOR_THRESHOLD) // it is essentially still
                {
                    high_frames = 0;
                    tellPlay();
                    last_x_position = data->position[data->index].X;
                    last_y_position = data->position[data->index].Y;
                    last_z_position = data->position[data->index].Z;
                    playing = true;
                    armed = false;
                    return;
                }
            }
            else if(playing)
            {
                // needs to check if sufficient movement occurred, if so play next
                // needs to check if backward movement occurred, if sufficient stop all
                float temp_x_position = data->position[data->index].X;
                float temp_y_position = data->position[data->index].Y;
                float temp_z_position = data->position[data->index].Z;
                float diff_x = temp_x_position - last_x_position;
                float diff_y = temp_y_position - last_y_position;
                float diff_z = temp_z_position - last_z_position;
                //if (diff_z < Z_BACKWARD_DIFFERENCE_THRESHOLD) { // if we have NOT moved back
                
                float distance = sqrt(diff_x*diff_x + diff_y* diff_y + diff_z * diff_z);
                if (distance > STOPPLAYING_DISTANCE_THRESHOLD) {
                    
                    tellStop();
                    playing = false;
                    
                }
            }
            
        }
    }
}


/*
 
 if(playing)
 {
 // needs to check if sufficient movement occurred, if so play next
 // needs to check if backward movement occurred, if sufficient stop all
 float temp_x_position = data->position[data->index].X;
 float temp_y_position = data->position[data->index].Y;
 float temp_z_position = data->position[data->index].Z;
 float diff_x = temp_x_position - last_x_position;
 float diff_y = temp_y_position - last_y_position;
 float diff_z = temp_z_position - last_z_position;
 //if (diff_z < Z_BACKWARD_DIFFERENCE_THRESHOLD) { // if we have NOT moved back
 
 float distance = sqrt(diff_x*diff_x + diff_y* diff_y + diff_z * diff_z);
 if (distance > PLAYNEXT_DISTANCE_THRESHOLD) {
 //tellPlay();
 tellStop();
 //                        last_x_position = temp_x_position;
 //                        last_y_position = temp_y_position;
 //                        last_z_position = temp_z_position;
 //                    }
 //                }
 //                else {
 //                    tellStop();
 //                    playing = false;
 }
 
 
 }
 
 */