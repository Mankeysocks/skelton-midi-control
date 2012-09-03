//
//  PlayerRouter.cpp
//  SkeletonMidiController
//
//  Created by Brian Breitsch on 8/1/12.
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include <iostream>
#include <map>

#include "SmPlayerRouter.h"
#include "RtMidiEnumerations.h"

namespace sm {
    
    
    PlayerRouter::PlayerRouter(rt::Channel::Channel ch)
    : in_channel(ch)
    {
        currentPlayer = NULL;
    }
    
    void PlayerRouter::noteOnCallback(rt::Pitch::Pitch pt, unsigned char val, rt::Channel::Channel ch)
    {
        if(ch == channel)
        {
            // change selector accordingly
            player_router_it it = selectors.find(pt);
            if(it != selectors.end())
            { // send all off
                currentPlayer->stopAll();
                currentPlayer = it->second;
            }
        }
    }
    
    void PlayerRouter::play()
    {
        if(currentPlayer != NULL)
        {
            currentPlayer->play();
        }
    }
    
    void PlayerRouter::stop()
    {
        if(currentPlayer != NULL)
        {
            currentPlayer->stop();
        }
    }
    
    void PlayerRouter::stopAll()
    {
        if(currentPlayer != NULL)
        {
            currentPlayer->stopAll();
        }
    }
    
    bool PlayerRouter::addPlayer(Player* player, rt::Pitch::Pitch pitch)
    {
        std::pair<player_router_it, bool> ret_val = selectors.insert(player_router_pair(pitch,player));
        return ret_val.second;
    }
    
    
}