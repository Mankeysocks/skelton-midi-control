//
//  SmHeightMappedPitchPlayer.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/13/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include "SmHeightMappedPitchPlayer.h"
#include "SmControl.h"
#include "SmGenerativeModelUtility.h"
#include "SmJointMotionData.h"

namespace sm {
    
    static const float SM_GENERATIVE_PITCH_PLAYER_HEIGHT_CHANGE_THRESHOLD = 50;
    
    using namespace boost::numeric::ublas;
    using namespace std;
    
    //////////////////////////////////////////
    //                                      //
    //      Class: HeightMappedPitchPlayer  //
    //                                      //
    //////////////////////////////////////////
    
    HeightMappedPitchPlayer::HeightMappedPitchPlayer(sm::UserSkeleton* usr, JointType jntTyp, JointType anchrJntTyp, rt::Channel::Channel ch)
    : Player(ch)
    , user(usr)
    , jointType(jntTyp)
    , anchorJointType(anchrJntTyp)
    , lastHeight(0)
    , lastNote(rt::Pitch::Pitch(64))
    {
        lastNote.pitch = rt::Pitch::Pitch(64);
        lastNote.vel = 100;
    }
    
    Note getNextNote(Note lastNote, float height, float last_height, float dist, float last_dist)
    {
        Note newNote(60,100);
        newNote.pitch = rt::Pitch::Pitch(0);
        return newNote;
    }
    
    
    void HeightMappedPitchPlayer::play()
    {
        // get user joint data to decide the model bounds
        JointMotionData *var_data, *anchor_data;
        if((var_data = user->getJointMotionlDataObject(jointType)) != NULL && (anchor_data = user->getJointMotionlDataObject(jointType)) != NULL)
        {
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
            
            lastNote = getNextNote(lastNote, pos_x, lastHeight, anchor_var_distance, lastDist);
            lastHeight = pos_x;
            lastDist = anchor_var_distance;
            
        }
        sendMidiMessage(rt::CommandName::NoteOn, channel, lastNote.pitch, lastNote.vel);
        playedNotes.push(lastNote);
    }
    
}