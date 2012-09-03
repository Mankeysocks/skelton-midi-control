//
//  GenerativePitchPlayer.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/31/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_GenerativePitchModel_h
#define SkeletonModuleApplication_GenerativePitchModel_h

#include <vector>

#include "SmCallback.h"
#include "SmPlayer.h"
#include "RtMidiEnumerations.h"
#include "SmUserSkeleton.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/bimap.hpp>

namespace sm {
    
    class GenerativePitchPlayer : public Player
    {
    public:
        
        GenerativePitchPlayer(UserSkeleton* skeleton, char* file, JointType jntTyp = HandRight, rt::Channel::Channel ch = rt::Channel::Channel1);
        
        ~GenerativePitchPlayer();

        virtual void play();
        
        JointType jointType;
        
    private:
        // This is our handle to user joint data that we use to decide model bounds
        UserSkeleton* user;
        
        // Normally this is the last note that was played (the only exception is the very first note)
        Note lastNote;
        
        // Index that keeps track of what state the model is playing
        int state_index_;
        
        // The matrix that represents a markov model for pitch states
        boost::numeric::ublas::matrix<int>* state_matrix_;
        
        boost::bimap<int/*state#*/,int/*index*/>* state_indices_; // .left = state index, .right = state number
        // keeps track of the state identifier (key) and its corresponding row/col number (value) (i.e. state C = 65, D = 67, F = 70, might have corresponding indices 0,1,2 respecitvely)
        
        float lastHeight;
    };
    
}

#endif
