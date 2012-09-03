
//
//  smControl.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/14/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef MotionModuleLibrary_smControl_h
#define MotionModuleLibrary_smControl_h

#include "SmJointMotionData.h"
#include "RtMidi.h"
#include "RtMidiEnumerations.h"
#include "SmRealWorldTimer.h"
#include "SmCallback.h"

#include <cstdlib>
#include <map>
#include <list>
#include <vector>

namespace sm{
    
    class UserSkeleton {
        
    public:
        
        // Skeleton constructor - 
        UserSkeleton();
        
        // Skeleton deconstructor; must deconstruct the JointMotionData
        ~UserSkeleton();
        
        // Adds the JointType type to the list of active joints (activeTypes)
        // Returns true if the joint is added, false if the joint is already active and was not added
        bool registerJoint(JointType type);
        
        // Removes the JoinType type from the list of active joints
        // Returns true if the JointType was removed, false if it was not active and therefor not removed
        bool unregisterJoint(JointType type);
        
        // bool isJointRegistered(JointType type)
        // use getJointMotionDataObject(JointType type) and check for null return
        
        // If a joint motion data object of JointType type is registered, then returns a pointer to that object
        JointMotionData* getJointMotionlDataObject(JointType type);
        
        // A designeted realworld timer tool should be used to get the time between updates to this object's data
        RealWorldTimer timer;
        
        // this id is used to determine which openni skeletal data should be assigned to this user
        int skeleton_number;
        
    private:
        // The map in which we store our value Motion Data corresponding to its key JointType
        // (Note: we store the pointers to the JointMotionData objects to avoid instantiating
        // the objects before we know if we should add them to the map; see registerJoint implementation)
        std::map<JointType, JointMotionData*> motionJoints_;
        
        
    };
    
}

#endif
