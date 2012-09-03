//
//  JointMotionData.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/12/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include "SmJointMotionData.h"

namespace sm {

    
    JointMotionData::JointMotionData()
    {
        // create our buffer arrays for this joint data, each of size BUFFER_SIZE defined in JointMotionData.h
        position = new Vector3[BUFFER_SIZE];
        velocity = new Vector3[BUFFER_SIZE];
        acceleration = new Vector3[BUFFER_SIZE];
        // set the buffer values and index to 0
        clearAndReset();
    }
    
    // Destructor for JointMotionData that deletes our buffer arrays
    JointMotionData::~JointMotionData()
    {
        delete position;
        delete velocity;
        delete acceleration;
    }
    
    
    void JointMotionData::addData(float posX, float posY, float posZ, float dt)
    {
        // increment the index so it is ready for the next addData call
        ++index;
        index = index%BUFFER_SIZE; // loop back to 0 if necessary
        
        // set the position data to the buffers in the index position (index is maintained so that it is always in the next open slot for data)
        position[index].X = posX;
        position[index].Y = posY;
        position[index].Z = posZ;
        // set velocity using current position and last position (V = dx/dt; dx = current_position - last_position)
        // use temp variable for efficiency
        float tempVx = (posX - lastPosition.X)/dt;
        float tempVy = (posY - lastPosition.Y)/dt;
        float tempVz = (posZ - lastPosition.Z)/dt;
        velocity[index].X = tempVx;
        velocity[index].Y = tempVy;
        velocity[index].Z = tempVz;
        // set acceleration using current velocity and last velocity (A = dv/dt; dv = current_velocity - last_velocity)
        acceleration[index].X = (tempVx - lastVelocity.X)/dt;
        acceleration[index].Y = (tempVy - lastVelocity.Y)/dt;
        acceleration[index].Z = (tempVz - lastVelocity.Z)/dt;
        // update last position for next addData call
        lastPosition.X = posX;
        lastPosition.Y = posY;
        lastPosition.Z = posZ;
        // update last velocity for next addData call
        lastVelocity.X = tempVx;
        lastVelocity.Y = tempVy;
        lastVelocity.Z = tempVz;
    }
    
    // Sets the data in the buffer arrays to zero and puts the index back to 0
    void JointMotionData::clearAndReset()
    {
        for (int i = 0; i < BUFFER_SIZE; i++) // TODO: DO MEMSET
        {
            position[i].X = 0;
            position[i].Y = 0;
            position[i].Z = 0;
            velocity[i].X = 0;
            velocity[i].Y = 0;
            velocity[i].Z = 0;
            acceleration[i].X = 0;
            acceleration[i].Y = 0;
            acceleration[i].Z = 0;
        }
        index = -1;
    }
    
}