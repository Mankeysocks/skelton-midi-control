//
//  SmControl.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/8/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_vEarth_SmControl_h
#define SkeletonModuleApplication_vEarth_SmControl_h

#include <list>

#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include "RtMidi.h"

#include "SmCallback.h"


namespace sm {
    

    
    ////////////////////////////////
    // Midi
    ////////////////////////////////
    
    // Adds the parameter to the message queue of Midi messages to send this frame
    void sendMidiMessage(rt::CommandName::CommandName command, rt::Channel::Channel channel, unsigned char paramA, unsigned char paramB);
    
    // Adds the parameter to the message queue of Midi messages to send this frame
    void sendMidiSystemExclusiveMessage(rt::SystemMessageType::SystemMessageType type, unsigned char paramA, unsigned char paramB);
    
    // This gives us a function that we can register to RtMidi's midi-in callback. When we recieve a midi message, we parse it upfront (here) and send it to the appropriate registered callbacks
    void midiCallbackFunction(double d, std::vector< unsigned char >* message, void * usr);
    
    // Returns true if the ports are configured, false if it could not configure the ports
    bool chooseMidiPortConfig();

    
    
    ////////////////////////////////
    // Random Number Generator
    ////////////////////////////////
    
    // returns a random number [0,1)
    float getRandomNumber();

    
    
    ////////////////////////////////////
    // Callback Registration and Lists
    ////////////////////////////////////
    
    // This registers the given function pointer for the midi system message event
    void registerSystemMessageCallback(sm::SystemCallback* obj);
    
    // This registers the given function pointer for the midi clock event
    void registerClockCallback(sm::ClockCallback* obj);
    
    // This registers the given function pointer for the midi control change event
    void registerControlChangeCallback(sm::ControlChangeCallback* obj);
    
    // This registers the given function pointer for the midi patch change event
    void registerPatchChangeCallback(sm::PatchChangeCallback* obj);
    
    // This registers the given function pointer for the midi note on event
    void registerNoteOnCallback(sm::NoteOnCallback* obj);
    
    // This registers the given function pointer for the midi note off event
    void registerNoteOffCallback(sm::NoteOffCallback* obj);

    
}


#endif
