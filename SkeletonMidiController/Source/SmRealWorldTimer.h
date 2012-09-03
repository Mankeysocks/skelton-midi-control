//
//  RealWorldTimer.h
//  SkeletonMidiController
//
//  Created by Sterling Lutes and Chase Parks 7/31/12
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#ifndef __REALWORLDTIMER_H__
#define __REALWORLDTIMER_H__

#include "SmUmbrellaHeader.h"

#define SM_TIME_TYPE long
#if defined(__WINDOWS_MM__)
#include <Windows.h>
#define SM_TIME_TYPE WORD
#endif
namespace sm {
    

    
    // A cross-platform utility for measuring elapsed real-world time
    class RealWorldTimer
    {
    public:
        RealWorldTimer();
        

        
        // returns the time in seconds (with system-dependent micro/millisecond accuracy)
        float getElapsedTime();
        
    private:
        
        SM_TIME_TYPE lastTime_;
    };
}
#endif