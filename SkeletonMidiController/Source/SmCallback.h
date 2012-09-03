//
//  SmMidiCallback.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/23/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleGUI_SmMidiCallback_h
#define SkeletonModuleGUI_SmMidiCallback_h

#include <vector>
#include "RtMidiEnumerations.h"

namespace sm {
    
    // Interface class for objects that implement the systemCallback function for system exclusive events
    class SystemCallback
    {
    public:
        // This callback function is useul to generators/routers dependent upon midi sync or system message info
        virtual void systemCallback(rt::SystemMessageType::SystemMessageType type) = 0;
    };
    
    // Interface class for objects that implement the clockCallback function
    class ClockCallback
    {
    public:
        // This callback function is mainly used by note generators so they know when to trigger the next sound
        virtual void clockCallback(int clock) = 0;
    };
    
    // Interface class for objects that implement the controlChangeCallback function
    class ControlChangeCallback
    {
    public:
        //
        ControlChangeCallback(rt::Channel::Channel chnnl = rt::Channel::Channel15) : controlChannel(chnnl) {}
        
        virtual void controlChangeCallback(rt::Channel::Channel channel, rt::Control::Control controller, unsigned char val) = 0;
        
        rt::Channel::Channel controlChannel;
    };
    
    // Interface class for objects that implement the patchChangeCallback function
    class PatchChangeCallback
    {
    public:
        //
        PatchChangeCallback(rt::Channel::Channel chnnl = rt::Channel::Channel15) : patchChannel(chnnl) {}
        
        virtual void patchChangeCallback(rt::Channel::Channel channel, unsigned char patch) = 0;
        
        rt::Channel::Channel patchChannel;
    };
    
    // Interface class for objects that implement the noteOnCallback function
    class NoteOnCallback
    {
    public:
        NoteOnCallback(rt::Channel::Channel chnnl = rt::Channel::Channel15) : noteOnChannel(chnnl) {}
        
        // This callback function is useul to generators/routers dependent upon song note data
        virtual void noteOnCallback(rt::Channel::Channel ch, rt::Pitch::Pitch pt, unsigned char val) = 0;
        
        rt::Channel::Channel noteOnChannel;
    };
    
    // Interface class for objects that implement the noteOffCallback function
    class NoteOffCallback
    {
    public:
        NoteOffCallback(rt::Channel::Channel chnnl = rt::Channel::Channel15) : noteOffChannel(chnnl) {}
        
        // This callback function is useful to generators/routers dependent upon song note data
        virtual void noteOffCallback(rt::Channel::Channel ch, rt::Pitch::Pitch pt, unsigned char val) = 0;
        
        rt::Channel::Channel noteOffChannel;
    };
    
    
}

#endif





