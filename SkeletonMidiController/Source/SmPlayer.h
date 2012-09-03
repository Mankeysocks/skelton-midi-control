//
//  SmNoteChooser.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/1/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_SmNoteChooser_h
#define SkeletonModuleApplication_SmNoteChooser_h

#include <queue>

#include "RtMidiEnumerations.h"

namespace sm {
    
    // This struct defines the attributes that a NoteSelector will choose
    struct Note
    {
        Note(rt::Pitch::Pitch ptch = rt::Pitch::C5, char vl = 100) : pitch(ptch) , vel(vl) {}
        Note(unsigned char ptch = rt::Pitch::C5, char vl = 100) : pitch(rt::Pitch::Pitch(ptch)) , vel(vl) {}
        rt::Pitch::Pitch pitch;
        char vel;
    };
    
    // A player is analogous to an orchestra member that has no sense of time. They must follow their conductor who tells them when to play and when to stop.
    class Player
    {
    public:
        
        Player(rt::Channel::Channel ch = rt::Channel::Channel1);
        
        // The destructor of Player should stop all the notes in its playedNotes queue
        ~Player();
        
        // The play method (must be implemented) chooses a note to play, sends a corresponding noteon midi message, and pushes the note to the top of playedNotes queue
        virtual void play() = 0;
        
        // Pops a note from the back of playedNotes queue and sends a corresponding noteoff midi message
        virtual void stop();
        
        // Pops all notes from the playedNotes queue and sends a corresponding noteoff midi message for each note
        virtual void stopAll();
        
    protected:
        // This queue stores the notes that the player plays so they can stop them later
        std::queue<Note> playedNotes;
        
        // Each Player has a channel they play on. It can be sent in the constructor. The channels of a Player, Conductor, Controller trio should be the same
        rt::Channel::Channel channel;
    };
    
}

#endif
