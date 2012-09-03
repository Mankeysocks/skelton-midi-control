//
//  smControl.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/15/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "SmUserSkeleton.h"

#include <iostream>
#include <cstdlib>
#include <map>

#include "SmUmbrellaHeader.h"
#include "SmJointMotionData.h"

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------
namespace sm {
    
    using namespace std;
    
    // 
    // 
    // Constructor/Destructor
    // 
    // 
    UserSkeleton::UserSkeleton()
    : skeleton_number(0)
    {
        
    }
    
    UserSkeleton::~UserSkeleton()
    {
        // Start at the beginning of our jointdata list...
        map<JointType,JointMotionData*>::iterator jnt_it;
        for(jnt_it = motionJoints_.begin(); jnt_it != motionJoints_.end(); jnt_it++)
        {
            delete (*jnt_it).second; // *index.second = JointMotionData*
        }
    }
    
    //
    //
    // JointMotionData interactions
    //
    //
    JointMotionData* UserSkeleton::getJointMotionlDataObject(JointType type)
    {
        // If it exists, we want to find the JointMotionData corresponding to JointType type
        map<JointType,JointMotionData*>::iterator it = motionJoints_.find(type);
        
        if(it != motionJoints_.end())
        {
            return (*it).second;
        }
        return NULL;
    }
    
    bool UserSkeleton::registerJoint(JointType type)
    {
        JointMotionData* temp;
        // A map forbids duplicates, and will return a pair that contains an iterator (pointing to
        // where the object was inserted, or to where an equivalent object already is in the list)
        // and a bool (true if joint type was just added, false if already added)
        pair<std::map<JointType,JointMotionData*>::iterator,bool> result = motionJoints_.insert(std::pair<JointType,JointMotionData*>(type,temp));
        // If the type was not previously in the list, then we will instantiate a new JointMotionData object
        if(result.second)
        {
            (*result.first).second = new JointMotionData(); // *result.first = pair<JointType,JointMotionData*>
        }
        
        // return bool value representing whether or not the joint type was already in active joints
        return result.second;
    }
    
    bool UserSkeleton::unregisterJoint(JointType type)
    {
        // If it exists, we want to find the JointMotionData corresponding to JointType type
        map<JointType,JointMotionData*>::iterator it = motionJoints_.find(type);
        // If the type is in the map (or the iterator never reaches map.end()), we want to destruct the JointMotionData
        // object and remove its mapping (pair<JointType,JointMotionData*>)
        if(it != motionJoints_.end())
        {
            delete (*it).second; // *index.second = JointMotionData*
            motionJoints_.erase(it);
            return true;
        }
        return false;
    }
}
