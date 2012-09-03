//
//  main.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/8/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <vector>

#include "SmControl.h"
#include "SmUserSkeleton.h"
#include "SmOpenNiControl.h"
#include "SmRenderer.h"

#include "SmConductor.h"
#include "SmControl.h"
#include "SmPlayer.h"
#include "SmGenerativePitchPlayer.h"
#include "SmRhythmicConductor.h"
#include "SmVelocityConductor.h"
#include "SmSubtleVelocityConductor.h"
#include "SmRadialVelocityConductor.h"
#include "SmCrossVelocityConductor.h"
#include "SmHeightMappedPitchPlayer.h"
#include "SmChordPlayer.h"

using namespace std;
using namespace sm;

int main(int argc, char* argv[])
{
    cout << "Welcome to Motion Music Application\n['q' to quit]\n" << endl;
    // initialize midi configurations
    if(!sm::chooseMidiPortConfig())
    {
        exit(1);
    }
    cout << "\n\n";
    UserSkeleton user1;
    UserSkeleton user2;
    UserSkeleton user3;
    //UserSkeleton user4;
    std::vector<UserSkeleton*> users;
    users.push_back(&user1);
    users.push_back(&user2);
    users.push_back(&user3);
    //users.push_back(&user4);
    
    user1.registerJoint(Head);
    user1.registerJoint(HandLeft);
    user1.registerJoint(HandRight);
    user1.registerJoint(FootLeft);
    user1.registerJoint(FootRight);
    user1.registerJoint(ShoulderRight);
    user1.registerJoint(ShoulderLeft);
    
    user2.registerJoint(Head);
    user2.registerJoint(HandLeft);
    user2.registerJoint(HandRight);
    user2.registerJoint(ShoulderRight);
    user2.registerJoint(ShoulderLeft);
    user2.registerJoint(FootLeft);
    user2.registerJoint(FootRight);
    
    user3.registerJoint(Head);
    user3.registerJoint(HandLeft);
    user3.registerJoint(HandRight);
    user3.registerJoint(ShoulderRight);
    user3.registerJoint(ShoulderLeft);
    user3.registerJoint(FootLeft);
    user3.registerJoint(FootRight); 
    
    GenerativePitchPlayer gen_player1(&user1,PITCH_GRAPH_C,HandRight,rt::Channel::Channel1);
    GenerativePitchPlayer gen_player2(&user2,PITCH_GRAPH_G,HandRight,rt::Channel::Channel2);
    GenerativePitchPlayer gen_player3(&user3,PITCH_GRAPH_F,HandRight,rt::Channel::Channel3);
    GenerativePitchPlayer gen_player4(&user1,PITCH_GRAPH_A,HandLeft,rt::Channel::Channel1);
    GenerativePitchPlayer gen_player5(&user2,PITCH_GRAPH_C,HandLeft,rt::Channel::Channel2);
    GenerativePitchPlayer gen_player6(&user3,PITCH_GRAPH_G,HandLeft,rt::Channel::Channel3);
    //RhythmicConductor rhy_conductor1(rhythem1); registerClockCallback(&rhy_conductor1);
    
    //HeightMappedPitchPlayer pitch_player1();
    ChordPlayer chrd_player1; registerNoteOnCallback(&chrd_player1);
    //ChordPlayer chrd_player2; registerNoteOnCallback(&chrd_player2);
    
    //CrossVelocityConductor vel_conductor1(&user1);registerClockCallback(&vel_conductor1);
    //CrossVelocityConductor vel_conductor2(&user1,HandLeft,ShoulderLeft);registerClockCallback(&vel_conductor2);
    RadialVelocityConductor vel_conductor1r(&user1,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor1r);
    RadialVelocityConductor vel_conductor1l(&user1,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor1l);
    
    RadialVelocityConductor vel_conductor2r(&user2,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor2r);
    RadialVelocityConductor vel_conductor2l(&user2,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor2l);
    
    RadialVelocityConductor vel_conductor3r(&user3,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor3r);
    RadialVelocityConductor vel_conductor3l(&user3,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor3l);
    
    vel_conductor1r.registerPlayer(&gen_player1);
    vel_conductor1l.registerPlayer(&gen_player4);
    vel_conductor2r.registerPlayer(&gen_player2);
    vel_conductor2l.registerPlayer(&gen_player5);
    vel_conductor3r.registerPlayer(&gen_player3);
    vel_conductor3l.registerPlayer(&gen_player6);
    
    //vel_conductor2.registerPlayer(&chrd_player2);
    if(!initializeOpenNiSkeletalTracking()) // if initialization succeeds
        smBeginRendering(argc,argv,users);

    int i;
    cin >> i;
    
    return 0;
}


 
 
 /*
 UserSkeleton user1;
 UserSkeleton user2;
 UserSkeleton user3;
 //UserSkeleton user4;
 std::vector<UserSkeleton*> users;
 users.push_back(&user1);
 users.push_back(&user2);
 users.push_back(&user3);
 //users.push_back(&user4);
 
 user1.registerJoint(Head);
 user1.registerJoint(HandLeft);
 user1.registerJoint(HandRight);
 user1.registerJoint(FootLeft);
 user1.registerJoint(FootRight);
 user1.registerJoint(ShoulderRight);
 user1.registerJoint(ShoulderLeft);
 
 user2.registerJoint(Head);
 user2.registerJoint(HandLeft);
 user2.registerJoint(HandRight);
 user2.registerJoint(ShoulderRight);
 user2.registerJoint(ShoulderLeft);
 user2.registerJoint(FootLeft);
 user2.registerJoint(FootRight);
 
 user3.registerJoint(Head);
 user3.registerJoint(HandLeft);
 user3.registerJoint(HandRight);
 user3.registerJoint(ShoulderRight);
 user3.registerJoint(ShoulderLeft);
 user3.registerJoint(FootLeft);
 user3.registerJoint(FootRight); 
 
 GenerativePitchPlayer gen_player1(&user1,PITCH_GRAPH_C,HandRight,rt::Channel::Channel1);
 GenerativePitchPlayer gen_player2(&user2,PITCH_GRAPH_G,HandRight,rt::Channel::Channel2);
 GenerativePitchPlayer gen_player3(&user3,PITCH_GRAPH_F,HandRight,rt::Channel::Channel3);
 GenerativePitchPlayer gen_player4(&user1,PITCH_GRAPH_A,HandLeft,rt::Channel::Channel1);
 GenerativePitchPlayer gen_player5(&user2,PITCH_GRAPH_C,HandLeft,rt::Channel::Channel2);
 GenerativePitchPlayer gen_player6(&user3,PITCH_GRAPH_G,HandLeft,rt::Channel::Channel3);
 //RhythmicConductor rhy_conductor1(rhythem1); registerClockCallback(&rhy_conductor1);
 
 //HeightMappedPitchPlayer pitch_player1();
 ChordPlayer chrd_player1; registerNoteOnCallback(&chrd_player1);
 //ChordPlayer chrd_player2; registerNoteOnCallback(&chrd_player2);
 
 //CrossVelocityConductor vel_conductor1(&user1);registerClockCallback(&vel_conductor1);
 //CrossVelocityConductor vel_conductor2(&user1,HandLeft,ShoulderLeft);registerClockCallback(&vel_conductor2);
 RadialVelocityConductor vel_conductor1r(&user1,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor1r);
 RadialVelocityConductor vel_conductor1l(&user1,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor1l);
 
 RadialVelocityConductor vel_conductor2r(&user2,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor2r);
 RadialVelocityConductor vel_conductor2l(&user2,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor2l);
 
 RadialVelocityConductor vel_conductor3r(&user3,HandRight,ShoulderRight,4,1);registerClockCallback(&vel_conductor3r);
 RadialVelocityConductor vel_conductor3l(&user3,HandLeft,ShoulderLeft,4,1);registerClockCallback(&vel_conductor3l);
 
 vel_conductor1r.registerPlayer(&gen_player1);
 vel_conductor1l.registerPlayer(&gen_player4);
 vel_conductor2r.registerPlayer(&gen_player2);
 vel_conductor2l.registerPlayer(&gen_player5);
 vel_conductor3r.registerPlayer(&gen_player3);
 vel_conductor3l.registerPlayer(&gen_player6);
 
 //vel_conductor2.registerPlayer(&chrd_player2);
 if(!initializeOpenNiSkeletalTracking()) // if initialization succeeds
 smBeginRendering(argc,argv,users);

 
 
 
 
 
 
 
 
 
 
 
 
 
 #include <stdio.h>
 #include <sys/types.h>
 
 #include <libusb/libusb.h>
 
 static void print_devs(libusb_device **devs)
 {
         libusb_device *dev;
         int i = 0;
 
         while ((dev = devs[i++]) != NULL) {
                 struct libusb_device_descriptor desc;
                 int r = libusb_get_device_descriptor(dev, &desc);
                 if (r < 0) {
                         fprintf(stderr, "failed to get device descriptor");
                         return;
                 }
 
                 printf("%04x:%04x (bus %d, device %d)\n",
                         desc.idVendor, desc.idProduct,
                         libusb_get_bus_number(dev), libusb_get_device_address(dev));
         }
 }
 
 
 
 */
/*
 
 vector<int> rhythem1;
 rhythem1.push_back(12);    rhythem1.push_back(24);    rhythem1.push_back(36);    rhythem1.push_back(48);
 
     sm::GenerativePitchModel c_model(&control,PITCH_GRAPH_C);
     sm::GenerativePitchModel g_model(&control,PITCH_GRAPH_G);
     sm::GenerativePitchModel a_model(&control,PITCH_GRAPH_A);
     sm::GenerativePitchModel f_model(&control,PITCH_GRAPH_F);
     
     sm::NoteSelectorRouter router_1;
     if(!router_1.addSelector(&c_model,rt::Pitch::C5)) printf("router failed to add c selector\n");
     if(!router_1.addSelector(&g_model,rt::Pitch::G5)) printf("router failed to add g selector\n");
     if(!router_1.addSelector(&a_model,rt::Pitch::A5)) printf("router failed to add a selector\n");
     if(!router_1.addSelector(&f_model,rt::Pitch::F5)) printf("router failed to add f selector\n");
     
     control.registerNoteOnCallback(&router_1);
     
     sm::VelocityNoteTrigger vel_trigger_1(&control,&c_model,sm::HandLeft);
     //sm::DistanceBasedNoteTrigger dist_trigger_1(&control,&router_1,sm::HandRight, sm::HandLeft);
     sm::RhythmicTrigger rhy_trigger;
     control.registerClockCallback(&vel_trigger_1);
     
     control.registerClockCallback(&rhy_trigger);
     
     control.registerJoint(sm::HandRight);
     control.registerJoint(sm::HandLeft);
     
     control.unregisterJoint(sm::HandRight);
     
     // initialize the skeletal tracking //boost::thread openni_thread(
     sm::initializeOpenNiSkeletalTracking( argc, argv, &control);
     */