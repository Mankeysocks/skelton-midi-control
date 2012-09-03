//
//  Player.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>

#include <queue>
#include "SmPlayer.h"
#include "RtMidiEnumerations.h"
#include "SmControl.h"

namespace sm {
    
    Player::Player(rt::Channel::Channel ch)
    : channel(ch)
    {
        
    }
    
    Player::~Player()
    {
        stopAll();
    }
    
    // Pops a note from the back of playedNotes queue and sends a corresponding noteoff midi message
    void Player::stop()
    {
        if (playedNotes.empty()) {
            return;
        }
        Note note = playedNotes.front();
        sendMidiMessage(rt::CommandName::NoteOff, channel, note.pitch, note.vel);
        playedNotes.pop();
    }
    
    // Pops all notes from the playedNotes queue and sends a corresponding noteoff midi message for each note
    void Player::stopAll()
    {
        while (!playedNotes.empty()) {
            stop();
        }
    }

}