//
//  SmChordSelector.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/9/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmChordSelector_h
#define SkeletonModuleApplication_vEarth_SmChordSelector_h

#include <vector>

#include "RtMidiEnumerations.h"
#include "SmPlayer.h"
#include "SmCallback.h"

namespace sm {
    
    class ChordPlayer : public Player, public NoteOnCallback
    {
    public:
        
        ChordPlayer(rt::Pitch::Pitch rtKy = rt::Pitch::Pitch(65), rt::Channel::Channel chnnl = rt::Channel::Channel1, rt::Channel::Channel ntOnChnnl = rt::Channel::Channel15);
        
        virtual void play();
        
        virtual void stop();
        
        virtual void stopAll();
        
        virtual void noteOnCallback(rt::Channel::Channel ch, rt::Pitch::Pitch pt, unsigned char val);
        
        // This vector defines the attributes that a ChordSelector will choose
        std::vector<Note> pitches; // pitch, velocity
        
        rt::Pitch::Pitch rootKey;
        
    };
    
}

#endif
