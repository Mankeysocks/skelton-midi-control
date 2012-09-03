//
//  JointMotionData.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/12/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef MotionModuleFramework_JointMotionData_h
#define MotionModuleFramework_JointMotionData_h

namespace sm {
    
    struct Vector3
    {
        Vector3(float x = 0, float y = 0, float z = 0)
        : X(x)
        , Y(y)
        , Z(z) {}
        float X;
        float Y;
        float Z;
    };
    
    enum JointType{
        
        Null,
        AnkleLeft,      //XN_SKEL_LEFT_ANKLE
        AnkleRight,     //XN_SKEL_RIGHT_ANKLE
        ElbowLeft,      //XN_SKEL_LEFT_ELBOW
        ElbowRight,     //XN_SKEL_RIGHT_ELBOW
        FootLeft,       //XN_SKEL_LEFT_FOOT
        FootRight,      //XN_SKEL_RIGHT_FOOT
        HandLeft,       //XN_SKEL_LEFT_HAND       XN_SKEL_LEFT_FINGERTIP
        HandRight,      //XN_SKEL_RIGHT_HAND      XN_SKEL_RIGHT_FINGERTIP
        Head,           //XN_SKEL_HEAD
        HipCenter,      //XN_SKEL_WAIST
        HipLeft,        //XN_SKEL_LEFT_HIP
        HipRight,       //XN_SKEL_RIGHT_HIP
        KneeLeft,       //XN_SKEL_LEFT_KNEE
        KneeRight,      //XN_SKEL_RIGHT_KNEE
        ShoulderCenter, //XN_SKEL_NECK            XN_SKEL_TORSO
        ShoulderLeft,   //XN_SKEL_LEFT_SHOULDER   XN_SKEL_LEFT_COLLAR
        ShoulderRight,  //XN_SKEL_RIGHT_SHOULDER  XN_SKEL_RIGHT_COLLAR
        Spine,          //XN_SKEL_TORSO
        WristLeft,      //XN_SKEL_LEFT_WRIST
        WristRight,     //XN_SKEL_RIGHT_WRIST
    };
    
    
    //TODO perhaps this should all be a struct and  clear/reset is unnecessary
    
    // This class stores motion information about a particular joint in a tracked skeleton.
    // In particular, it stores the position, velocity and acceleration data.
    // This data is stored in 9 float arrays of size 256 (BUFFER_SIZE):
    // One each for X, Y, and Z components of Position, Velocity, and Acceleration
    class JointMotionData
    {
    public:
        
        // Constructor for JointMotionData object
        JointMotionData();
        
        // Destructor for JointMotionData object
        // Must take care of the 9 data buffers
        ~JointMotionData();
        
        // The size for the buffers storing the motion data (position, velocity, acceleration)
        static const int BUFFER_SIZE = 256;
        
        // The following structures allow us to access our motion data in a convenient way,
        // first declaring the degree of the data (i.e. position, velocity, or acceleration)
        // and then declaring the component (X, Y, or Z)
        Vector3* position;
        Vector3* velocity;
        Vector3* acceleration;
        
        // This method allows us to send in the position data to store in the buffers
        // It takes this new data, along with the change in time since the last update, 
        // in order to calculate the velocity and acceleration information
        void addData(float posX, float posY, float posZ, float dt);
        
        // Sets the data in the buffer arrays to zero and puts the index back to 0
        void clearAndReset();
        
        
        // The index that keeps track of where to place data in the buffers
        int index;
        
    private:
        
        // The information of previously calculated motion data used to calculate derivative values
        Vector3 lastPosition, lastVelocity;
        
    };
    
}

#endif
