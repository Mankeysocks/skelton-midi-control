//
//  SmControl.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 7/30/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

//////////////////////////////////
// Includes
//////////////////////////////////

#include <iostream>
#include <string>
#include <vector>

#include "SmControl.h"
#include "SmUmbrellaHeader.h"
#include "RtMidiEnumerations.h"


namespace sm {
    
    using namespace std;
    
    //////////////////////////////////
    // Globals
    //////////////////////////////////
    
    // This is a holder for user response to certain menu commands
    char user_res = 0;
    
    // This is our clock variable that we use to keep track of where we are from the start of the song, and keep the generators in sync
    int sm_clock = 0;
    
    // The preset names for our midi input and midi output ports
    const string midi_input_portname = SM_MIDI_INPUT_PORT_NAME;
    const string midi_output_portname = SM_MIDI_OUTPUT_PORT_NAME;
    
    
    // This recieves the sound to use from the midiout object.
    rt::RtMidiIn midiin;
    
    // This is our midi out object for all the modules that send sound to use
    rt::RtMidiOut midiout;
    
    
    // Random number generator with reproducible seed
    boost::minstd_rand generator(42u);
    
    // Define a uniform random number distribution which produces "float"
    boost::uniform_real<> uni_dist(0,1);
    
    // The object that uses the random number generator and yields a uniform distribution
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni(generator,uni_dist);
    
    
    // A list of objects registered to recieve callback functions for midi system events
    std::list<sm::SystemCallback*> systemCallbacks;
    
    // A list of objects registered to recieve callback functions for note on events
    std::list<sm::ClockCallback*> clockCallbacks;
    
    // A list of objects registered to recieve callback functions for controlChange events
    std::list<sm::ControlChangeCallback*> controlChangeCallbacks;
    
    // A list of objects registered to recieve callback functions for controlChange events
    std::list<sm::PatchChangeCallback*> patchChangeCallbacks;
    
    // A list of objects registered to recieve callback functions for note on events
    std::list<sm::NoteOnCallback*> noteOnCallbacks;
    
    // A list of objects registered to recieve callback functions for note off events
    std::list<sm::NoteOffCallback*> noteOffCallbacks;
    
    
    //////////////////////////////////
    // Midi Callback
    //////////////////////////////////
    
    // This gives us a function that we can register to RtMidi's midi-in callback. When we recieve a midi message, we parse it upfront (here) and send it to the appropriate registered callbacks
    void midiCallbackFunction(double d, std::vector< unsigned char >* message, void * usr)
    {
        std::vector<unsigned char>::iterator msg_it = message->begin();
        unsigned char temp_command_type = (*msg_it) >> 4;   // command type is MSB nibble 
        temp_command_type = temp_command_type << 4;         // our enumeration still uses 0x8X - 0xFX
        rt::CommandName::CommandName command_type = static_cast<rt::CommandName::CommandName>(temp_command_type);
        if (command_type == rt::CommandName::SystemMessage)
        {
            rt::SystemMessageType::SystemMessageType type = static_cast<rt::SystemMessageType::SystemMessageType>((*msg_it) % 16);
#ifdef __SM_DEBUG_CALLBACKS__
            printf("Reached system callback; type %i\n",rt::SystemMessageType::SystemMessageType(type));
#endif
            if (type ==rt::SystemMessageType::Clock) {
                list<sm::ClockCallback*>::iterator it = clockCallbacks.begin();
                for (; it != clockCallbacks.end(); ++it)
                {
                    (*it)->clockCallback(sm_clock);
                }
                ++sm_clock;
            }
            else
            {
                list<sm::SystemCallback*>::iterator it = systemCallbacks.begin();
                for (; it != systemCallbacks.end(); ++it)
                {
                    (*it)->systemCallback(type);
                }
            }
        }
        else    // all other callback types use channels
        {
            rt::Channel::Channel channel = static_cast<rt::Channel::Channel>((*msg_it) % 16); // channel is LSB nibble of first byte
            ++msg_it;
            if (command_type == rt::CommandName::ControlChange)
            {
                rt::Control::Control controller = static_cast<rt::Control::Control>((*msg_it) % 8); // control type is 0-127
                ++msg_it;
                unsigned char value = static_cast<unsigned char>((*msg_it) % 8); // control value is 0-127
                // notify all control change participants
                for (list<sm::ControlChangeCallback*>::iterator it = controlChangeCallbacks.begin(); it != controlChangeCallbacks.end(); ++it)
                {
                    (*it)->controlChangeCallback(channel, controller, value);
                }
            }
            else if(command_type == rt::CommandName::PatchChange)
            {
                unsigned char patch = static_cast<unsigned char>((*msg_it) % 8); // control type is 0-127
                // notify all patch change participants
                for (list<sm::PatchChangeCallback*>::iterator it = patchChangeCallbacks.begin(); it != patchChangeCallbacks.end(); ++it)
                {
                    (*it)->patchChangeCallback(channel, patch);
                }
            }
            else    // note on/note off callbacks use pitch
            {
                rt::Pitch::Pitch pitch = static_cast<rt::Pitch::Pitch>(*msg_it);
                msg_it++;
                char val = *msg_it;
                if (command_type == rt::CommandName::NoteOn)
                {
                    // notify all note on participants
                    for (list<sm::NoteOnCallback*>::iterator it = noteOnCallbacks.begin(); it != noteOnCallbacks.end(); ++it)
                    {
                        (*it)->noteOnCallback(channel, pitch, val);
                    }
                }
                else if (command_type == rt::CommandName::NoteOff)
                {
                    // notify all note off participants
                    for (list<sm::NoteOffCallback*>::iterator it = noteOffCallbacks.begin(); it != noteOffCallbacks.end(); ++it)
                    {
                        (*it)->noteOffCallback(channel, pitch, val);
                    }
                }
            }
        }
    }
    
    
    
    //////////////////////////////////
    // Midi Configuration
    //////////////////////////////////

#define EXIT_IF_Q(ur,what)		\
if (ur == 'q')				    \
{								\
printf("Quit at %s\n", what);   \
return false;					\
}                               \

    // Returns true if the ports are configured, false if it could not configure the ports
    bool chooseMidiPortConfig()
    {
        //////////////////////////
        // Open Midi Input Port //
        //////////////////////////
        
        // Check inputs.
        unsigned int num_ports = midiin.getPortCount();
        for ( unsigned i=0; i<num_ports; i++ ) {
            string portName = midiin.getPortName(i);
            //cout << "  Input Port #" << i+1 << ": " << portName << '\n';
            if(portName.compare(midi_input_portname) == 0)
            {
                midiin.openPort(i);
                midiin.setCallback(&midiCallbackFunction);
                midiin.ignoreTypes(false,false,false);
                cout << "Dedicated input port found and will be used:\n   " << midi_input_portname << endl;
                goto sm_midi_output_select;
            }
        }
        cout << "\nThere are " << num_ports << " MIDI input sources available.\n";
        while (true) {
            cout << "Please choose midi input port #: ";
            cin >> user_res;
            EXIT_IF_Q(user_res,"midi-in initialization")
            try {
                int i = user_res - 49;
                midiin.openPort(i);
                midiin.setCallback(&midiCallbackFunction);
                midiin.ignoreTypes(false,false,false);
                string portName = midiout.getPortName(i);
                cout << "  Input Port #" << i+1 << ": " << portName << endl;
                break;
            } catch (rt::RtError e) {
                cout << "\nPlease select another port (port doesn't exist or failed to register callback)" << endl;
            }
        }
        
        ///////////////////////////
        // Open Midi Output Port //
        ///////////////////////////
        
    sm_midi_output_select:
        // Check outputs.
        num_ports = midiout.getPortCount();
        for ( unsigned i=0; i<num_ports; i++ ) {
            string portName = midiout.getPortName(i);
            //cout << "  Output Port #" << i+1 << ": " << portName << endl;
            if(portName.compare(midi_output_portname) == 0)
            {
                midiout.openPort(i);
                cout << "Dedicated output port found and will be used:\n   " << midi_output_portname << endl;
                goto sm_done;
            }
        }
        cout << endl;
        cout << "\nThere are " << num_ports << " MIDI output ports available.\n";
        while (true) {
            cout << "Please choose midi output port #:\n";
            cin >> user_res;
            EXIT_IF_Q(user_res,"midi-out initialization")
            try {
                int i = user_res - 49;
                midiout.openPort(i);
                string portName = midiout.getPortName(i);
                cout << "  Output Port #" << i+1 << ": " << portName << endl;
                break;
            } catch (rt::RtError e) {
                cout << "\nPlease select another port" << endl;
            }
        }
    sm_done:
        return true;
    }
    
    
    
    //////////////////////////////////
    // Midi Messaging
    //////////////////////////////////
    
    // Adds the parameter to the message queue of Midi messages to send this frame
    void sendMidiMessage(rt::CommandName::CommandName command, rt::Channel::Channel channel, unsigned char paramA, unsigned char paramB)
    {
        vector<unsigned char> queue;
        queue.push_back(command + channel);
        queue.push_back(paramA);
        queue.push_back(paramB);
        midiout.sendMessage(&queue);
#ifdef __SM_DEBUG_MODE__
        printf("Sent midi message\n");
#endif
    }
    
    void sendMidiSystemExclusiveMessage(rt::SystemMessageType::SystemMessageType type, unsigned char paramA, unsigned char paramB)
    {
        vector<unsigned char> queue;
        queue.push_back(rt::CommandName::SystemMessage + type);
        queue.push_back(paramA);
        queue.push_back(paramB);
        midiout.sendMessage(&queue);
#ifdef __SM_DEBUG_MODE__
        printf("sent system exclusive midi message\n");
#endif
    }
    
    

    
    //////////////////////////////////
    // Midi Callback Registration
    //////////////////////////////////
    
    // This registers the given function pointer for the midi system message event
    void registerSystemMessageCallback(sm::SystemCallback* obj)
    {
        systemCallbacks.push_back(obj);
    }
    
    // This registers the given function pointer for the midi clock event
    void registerClockCallback(sm::ClockCallback* obj)
    {
        clockCallbacks.push_back(obj);
    }
    
    // This registers the given function pointer for the midi control change event
    void registerControlChangeCallback(sm::ControlChangeCallback* obj)
    {
        controlChangeCallbacks.push_back(obj);
    }
    
    // This registers the given function pointer for the midi patch change event
    void registerPatchChangeCallback(sm::PatchChangeCallback* obj)
    {
        patchChangeCallbacks.push_back(obj);
    }
    
    // This registers the given function pointer for the midi note on event
    void registerNoteOnCallback(sm::NoteOnCallback* obj)
    {
        noteOnCallbacks.push_back(obj);
    }
    
    // This registers the given function pointer for the midi note off event
    void registerNoteOffCallback(sm::NoteOffCallback* obj)
    {
        noteOffCallbacks.push_back(obj);
    }
    
    
    
    //////////////////////////////////
    // Random Numbers
    //////////////////////////////////
    
    float getRandomNumber()
    {
        return uni();
    }
    
    
}
/*
 
 control.registerJoint(sm::AnkleLeft);
 control.registerJoint(sm::AnkleRight);
 control.registerJoint(sm::ElbowLeft);
 control.registerJoint(sm::ElbowRight);
 control.registerJoint(sm::FootLeft);
 control.registerJoint(sm::FootRight);
 control.registerJoint(sm::HandLeft);
 control.registerJoint(sm::HandRight);
 control.registerJoint(sm::Head);
 //control.registerJoint(sm::HipCenter);
 control.registerJoint(sm::HipLeft);
 control.registerJoint(sm::HipRight);
 control.registerJoint(sm::KneeLeft);
 control.registerJoint(sm::KneeRight);
 //control.registerJoint(sm::ShoulderCenter);
 control.registerJoint(sm::ShoulderLeft);
 control.registerJoint(sm::ShoulderRight);
 control.registerJoint(sm::Spine);
 //control.registerJoint(sm::WristLeft);
 //control.registerJoint(sm::WristRight);
 
 
 sm::GenerativePitchModel c_model(&control,PITCH_GRAPH);
 sm::GenerativePitchModel d_model(&control,PITCH_GRAPH_A);
 sm::GenerativePitchModel e_model(&control,PITCH_GRAPH_C);
 //    sm::GenerativePitchModel g_model(&control,PITCH_GRAPH_C);
 //    sm::GenerativePitchModel a_model(&control,PITCH_GRAPH_G);
 //    sm::GenerativePitchModel f_model(&control,PITCH_GRAPH_F);
 
 //    sm::NoteSelectorRouter router;
 //    router.addSelector(&c_model,rt::Pitch::C5);
 //    router.addSelector(&g_model,rt::Pitch::G5);
 //    router.addSelector(&a_model,rt::Pitch::A5);
 //    router.addSelector(&f_model,rt::Pitch::F5);
 //
 
 
 //    sm::LinearScaleNoteSelector lin2_selector(&control,sm::HandLeft);
 //    lin2_selector.current_note.channel = rt::Channel::Channel2;
 ////    sm::DistanceBasedNoteTrigger dist2_note_trigger(&control,&lin2_trigger,sm::ShoulderLeft, sm::HandLeft); // change to linear select model
 ////    control.registerClockCallback(&dist2_note_trigger);
 ////    control.addModule(&dist2_note_trigger);
 ////    
 ////    
 ////     
 ////    sm::LinearScaleNoteSelector lin_trigger(&control,sm::HandRight);
 ////    sm::DistanceBasedNoteTrigger dist_note_trigger(&control,&lin_trigger,sm::ShoulderRight, sm::HandRight); // change to linear select model
 ////    control.registerClockCallback(&dist_note_trigger);
 ////    control.addModule(&dist_note_trigger);
 //    
 ////    sm::LinearScaleNoteSelector lin_trigger(&control,sm::HandRight);
 ////        sm::DistanceBasedNoteTrigger dist_note_trigger(&control,&lin_trigger,sm::ShoulderRight, sm::HandRight); // change to linear select model
 ////        control.registerClockCallback(&dist_note_trigger);
 ////        control.addModule(&dist_note_trigger);
 //// 
 ////        sm::DistanceBasedNoteTrigger dist2_note_trigger(&control,&e_model,sm::ShoulderLeft, sm::HandLeft); // change to linear select model
 ////        control.registerClockCallback(&dist2_note_trigger);
 ////        control.addModule(&dist2_note_trigger);
 //
 ////     
 //     
 //    sm::DistanceBasedNoteTrigger lin2_note_trigger(&control,&lin2_selector,sm::ShoulderLeft, sm::HandLeft); // change to linear select model
 //    control.registerClockCallback(&lin2_note_trigger);
 //    control.addModule(&lin2_note_trigger);
 //  
 sm::LinearScaleNoteSelector lin_selector(&control,sm::HandLeft);
 sm::VelocityNoteTrigger vel_note_trigger(&control, &lin_selector, sm::HandRight); // change to linear select model
 control.registerClockCallback(&vel_note_trigger);
 control.addModule(&vel_note_trigger);
 //hand_note_generator.registerCallback(&p_model);
 */


