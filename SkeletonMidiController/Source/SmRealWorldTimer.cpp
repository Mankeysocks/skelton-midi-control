//
//  RealWorldTimer.cpp
//  SkeletonMidiController
//
//  Created by Sterling Lutes and Chase Parks 7/31/12
//  Distributed under GNU Lesser Public License (version 3 or later)
//

#include "SmRealWorldTimer.h"
#include "SmUmbrellaHeader.h"
#include <iostream>
#if defined(__MACOSX_CORE__)
#include <sys/time.h>
#endif
#if defined(__WINDOWS_MM__)
#include "windows.h"
#endif




namespace sm {
    
    RealWorldTimer::RealWorldTimer(){
        // get the time at instantiation
#if defined(__MACOSX_CORE__)
        timeval time;
        gettimeofday(&time,NULL);
        lastTime_ = (time.tv_sec * 1000) + (time.tv_usec);
        //printf("Got Time");
#endif
#if defined(__WINDOWS_MM__)
        SYSTEMTIME time;
        GetSystemTime(&time);
        startTime = (time.wSeconds * 1000) + time.wMilliseconds;
#endif
    }
    
    
    //Gets the elapsed time in seconds
    float RealWorldTimer::getElapsedTime()
    {
#if defined(__MACOSX_CORE__)
        long tempTime;
        timeval time;
        gettimeofday(&time,NULL);
        tempTime = (time.tv_sec * 1000) + (time.tv_usec);
        float returnTime = (float)(tempTime - lastTime_)/1000000;
        //printf("Got Elapsed Time: %f",returnTime);
        lastTime_ = tempTime;
        return returnTime;

#endif
#if defined(__WINDOWS_MM__)
       WORD tempTime;
        SYSTEMTIME time;
        GetSystemTime(&time);
		tempTime = (time.wSecond * 1000) + time.wMilliseconds;
		float returnTime = (float)(tempTime - lastTime)/1000;
		//printf("Time:  %f" , returnTime);
		lastTime = tempTime;
        return returnTime;
#endif
        return 0;
    }
    
}