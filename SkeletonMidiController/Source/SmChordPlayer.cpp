//
//  SmChordPlayer.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/10/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <vector>

#include "SmChordPlayer.h"
#include "SmControl.h"
#include "SmPlayer.h"

namespace sm {
    
    ChordPlayer::ChordPlayer(rt::Pitch::Pitch rtKy, rt::Channel::Channel chnnl, rt::Channel::Channel ntOnChnnl)
    : rootKey(rtKy)
    , Player(chnnl)
    , NoteOnCallback(ntOnChnnl)
    {
        pitches.push_back(Note(rtKy,100));
        pitches.push_back(Note(rtKy+4,100));
        pitches.push_back(Note(rtKy+7,100));
        pitches.push_back(Note(rtKy+12,100));
    }
    
    void ChordPlayer::play()
    {
        std::vector<Note>::iterator it;
        for (it = pitches.begin(); it != pitches.end(); ++it)
        {
            sendMidiMessage(rt::CommandName::NoteOn, channel, it->pitch, it->vel);
        }
    }
    
    void ChordPlayer::stop()
    {
        std::vector<Note>::iterator it;
        for (it = pitches.begin(); it != pitches.end(); ++it)
        {
            sendMidiMessage(rt::CommandName::NoteOff, channel, it->pitch, it->vel);
        }
    }

    void ChordPlayer::stopAll()
    {
        stop();
    }
    
    // The note on callback signals the root of chord that the chord player should play. Based on the root (major) key defined for the player, the chord player will adjust its note vector (the notes in the chord that it plays) to play the chord that corresponds to that root in the key of rootKey. For example, a calculated chordRoot of 0 in the rootKey of 65 (C) would construct a C chord in the vector, whereas a 1 would construct a C#dim chord
    void ChordPlayer::noteOnCallback(rt::Channel::Channel ch, rt::Pitch::Pitch pt, unsigned char val)
    {
        stop();
        char chordRoot; // This is the 
        pitches[0] = Note(pt);
        
        // First we want to get the difference between the 
        if (pt < rootKey)
        {
            chordRoot = rootKey - pt;
        }
        else
        {
            chordRoot = pt - rootKey;
        }
        
        chordRoot = chordRoot % 12; // We get the tonal relationship of the signal chord to the root key
        
        if(chordRoot == 0 || chordRoot == 5 || chordRoot == 7 )
        {
            pitches[1] = Note(pt+4);
            pitches[2] = Note(pt+7);
            pitches[3] = Note(pt+12);
        }
        else if (chordRoot == 2 || chordRoot == 4 || chordRoot == 9 ) {
            pitches[1] = Note(pt+3);
            pitches[2] = Note(pt+7);
            pitches[3] = Note(pt+12);
        }
        else if (chordRoot == 1 || chordRoot == 6 || chordRoot == 8 || chordRoot == 11) {
            pitches[1] = Note(pt+3);
            pitches[2] = Note(pt+6);
            pitches[3] = Note(pt+9);
        }
        else if (chordRoot == 3 || chordRoot == 10) {
            pitches[1] = Note(pt+4);
            pitches[2] = Note(pt+8);
            pitches[3] = Note(pt+12);
        }
    }
}