//
//  SmGenerativePitchModel.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/31/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include "SmGenerativePitchPlayer.h"
#include "SmControl.h"
#include "SmGenerativeModelUtility.h"
#include "SmJointMotionData.h"

namespace sm {
    
    static const float SM_GENERATIVE_PITCH_PLAYER_HEIGHT_CHANGE_THRESHOLD = 50;
    
    using namespace boost::numeric::ublas;
    using namespace std;
    
    //////////////////////////////////////////
    //                                      //
    //      Class: GenerativePitchModel     //
    //                                      //
    //////////////////////////////////////////
    
    GenerativePitchPlayer::GenerativePitchPlayer(sm::UserSkeleton* usr, char* file, JointType jntTyp, rt::Channel::Channel ch)
    : Player(ch)
    , user(usr)
    , jointType(jntTyp)
    , state_index_(0)
    , lastNote(64)
    , lastHeight(0)
    {
        state_indices_ = NULL;
        state_matrix_ = NULL;
        if(!generatePitchModelFromFile(file, &state_matrix_, &state_indices_))
        {
            state_indices_ = NULL;
            state_matrix_ = NULL; // need public status to reference in case of failure
        }
        lastNote.pitch = rt::Pitch::Pitch(64);
        lastNote.vel = 100;
    }
    
    GenerativePitchPlayer::~GenerativePitchPlayer()
    {
        if(state_matrix_ != NULL)  
        {
            delete state_matrix_;
        }
        if(state_indices_ != NULL)
        {
            delete state_indices_;
        }
    }
    
    void GenerativePitchPlayer::play()
    {
        state_index_ = state_indices_->right.at(lastNote.pitch);
        float rand_val = sm::getRandomNumber(); // get a random value [0,1)
        // the row index is the current state's state index
        // index, #
        int low_index = 0;
        int high_index = state_matrix_->size1() - 1;
        // get user joint data to decide the model bounds
        JointMotionData* data;
        if((data = user->getJointMotionlDataObject(jointType)) != NULL)
        {
            float y_pos = data->position[data->index].Y;
            if(y_pos < lastHeight - SM_GENERATIVE_PITCH_PLAYER_HEIGHT_CHANGE_THRESHOLD)
            {
                high_index = state_index_;
            }
            else if(y_pos > lastHeight + SM_GENERATIVE_PITCH_PLAYER_HEIGHT_CHANGE_THRESHOLD)
            {
                low_index = state_index_;
            }
            lastHeight = y_pos;
        }
        state_index_ = binarySearchMatrixRow(state_matrix_,state_index_,rand_val,low_index,high_index); // the next state is the column index that has a p.f. just >= rand_val
        lastNote.pitch = rt::Pitch::Pitch(state_indices_->left.at(state_index_));
        sendMidiMessage(rt::CommandName::NoteOn, channel, lastNote.pitch, lastNote.vel);
        playedNotes.push(lastNote);
    }
    
}