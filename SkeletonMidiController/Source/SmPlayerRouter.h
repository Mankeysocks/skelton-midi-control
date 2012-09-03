//
//  SmModelRouter.h
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/1/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef SkeletonModuleApplication_SmModelRouter_h
#define SkeletonModuleApplication_SmModelRouter_h

#include "SmCallback.h"
#include "SmPlayer.h"
#include "RtMidiEnumerations.h"

namespace sm {
    
    typedef std::map<rt::Pitch::Pitch, Player*> player_router_map;
    typedef std::pair<rt::Pitch::Pitch ,Player*> player_router_pair;
    typedef player_router_map::iterator player_router_it;
    
    // This NoteSelector class routes requests for a note selection to another note selector
    class PlayerRouter : public NoteOnCallback , public Player
    {
        
    public:
        
        PlayerRouter(rt::Channel::Channel ch = rt::Channel::Channel15);
        
        void noteOnCallback(rt::Pitch::Pitch pt, unsigned char val, rt::Channel::Channel ch);
        
        virtual void play(); // TODO problem with stopping routed notes
        
        virtual void stop();
        
        virtual void stopAll();
        
        // adds a model to the router that will become active when the parameter pitch is recieved in noteOnCallback
        bool addPlayer(Player* player, rt::Pitch::Pitch pitch);
        
        // this is a map of pitches to models that we should use
        player_router_map selectors;
        
        Player* currentPlayer;
        
        rt::Channel::Channel in_channel;
    };
    
}

#endif
