//
//
// This file was modified from its original form (OpenNi sample project NiUserTracker; file: NiUserTracker.cpp)
// by Brian Breitsch on 30 July 2012
//
//

/****************************************************************************
 *                                                                           *
 *  OpenNI 1.x Alpha                                                         *
 *  Copyright (C) 2011 PrimeSense Ltd.                                       *
 *                                                                           *
 *  This file is part of OpenNI.                                             *
 *                                                                           *
 *  OpenNI is free software: you can redistribute it and/or modify           *
 *  it under the terms of the GNU Lesser General Public License as published *
 *  by the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  OpenNI is distributed in the hope that it will be useful,                *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public License *
 *  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                           *
 ****************************************************************************/

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>

#include "SmRenderer.h"
#include "SmUmbrellaHeader.h"
#include "SmOpenNiControl.h"
#include "SmControl.h"


namespace sm {
    
    
    //---------------------------------------------------------------------------
    // Globals
    //---------------------------------------------------------------------------
    xn::Context g_Context;
    xn::ScriptNode g_scriptNode;
    xn::DepthGenerator g_DepthGenerator;
    xn::UserGenerator g_UserGenerator;
    xn::Player g_Player;
    
    std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > m_Errors;
    
    bool g_bNeedPose = FALSE;
    char g_strPose[20] = "";

    //---------------------------------------------------------------------------
    // Code
    //---------------------------------------------------------------------------
    
    void CleanupExit()
    {
        g_scriptNode.Release();
        g_DepthGenerator.Release();
        g_UserGenerator.Release();
        g_Player.Release();
        g_Context.Release();
    }
    
    // Callback: New user was detected
    void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
    {
        XnUInt32 epochTime = 0;
        xnOSGetEpochTime(&epochTime);
        printf("%d New User %d\n", epochTime, nId);
        // New user found
        if (g_bNeedPose)
        {
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else
        {
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
    // Callback: An existing user was lost
    void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
    {
        XnUInt32 epochTime = 0;
        xnOSGetEpochTime(&epochTime);
        printf("%d Lost user %d\n", epochTime, nId);
    }
    // Callback: Detected a pose
    void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
    {
        XnUInt32 epochTime = 0;
        xnOSGetEpochTime(&epochTime);
        printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
        g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
    // Callback: Started calibration
    void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
    {
        XnUInt32 epochTime = 0;
        xnOSGetEpochTime(&epochTime);
        printf("%d Calibration started for user %d\n", epochTime, nId);
    }
    // Callback: Finished calibration
    void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
    {
        XnUInt32 epochTime = 0;
        xnOSGetEpochTime(&epochTime);
        if (eStatus == XN_CALIBRATION_STATUS_OK)
        {
            // Calibration succeeded
            printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
            g_UserGenerator.GetSkeletonCap().StartTracking(nId);
        }
        else
        {
            // Calibration failed
            printf("%d Calibration failed for user %d\n", epochTime, nId);
            if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
            {
                printf("Manual abort occured, stop attempting to calibrate!");
                return;
            }
            if (g_bNeedPose)
            {
                g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
            }
            else
            {
                g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
            }
        }
    }
    
#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"
    
    // Save calibration to file
    void SaveCalibration()
    {
        XnUserID aUserIDs[20] = {0};
        XnUInt16 nUsers = 20;
        g_UserGenerator.GetUsers(aUserIDs, nUsers);
        for (int i = 0; i < nUsers; ++i)
        {
            // Find a user who is already calibrated
            if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i]))
            {
                // Save user's calibration to file
                g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
                break;
            }
        }
    }
    // Load calibration from file
    void LoadCalibration()
    {
        XnUserID aUserIDs[20] = {0};
        XnUInt16 nUsers = 20;
        g_UserGenerator.GetUsers(aUserIDs, nUsers);
        for (int i = 0; i < nUsers; ++i)
        {
            // Find a user who isn't calibrated or currently in pose
            if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
            if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;
            
            // Load user's calibration from file
            XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
            if (rc == XN_STATUS_OK)
            {
                // Make sure state is coherent
                g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
                g_UserGenerator.GetSkeletonCap().StartTracking(aUserIDs[i]);
            }
            break;
        }
    }    

    void XN_CALLBACK_TYPE MyCalibrationInProgress(xn::SkeletonCapability& /*capability*/, XnUserID id, XnCalibrationStatus calibrationError, void* /*pCookie*/)
    {
        m_Errors[id].first = calibrationError;
    }
    void XN_CALLBACK_TYPE MyPoseInProgress(xn::PoseDetectionCapability& /*capability*/, const XnChar* /*strPose*/, XnUserID id, XnPoseDetectionStatus poseError, void* /*pCookie*/)
    {
        m_Errors[id].second = poseError;
    }
           
#define CHECK_RC(nRetVal, what)									\
if (nRetVal != XN_STATUS_OK)									\
{																\
printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
return nRetVal;                                                 \
}
    
    int initializeOpenNiSkeletalTracking()
    {
        
        XnStatus nRetVal = XN_STATUS_OK;
        
        xn::EnumerationErrors errors;
        nRetVal = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
        if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
        {
            XnChar strError[1024];
            errors.ToString(strError, 1024);
            printf("%s\n", strError);
            return (nRetVal);
        }
        else if (nRetVal != XN_STATUS_OK)
        {
            printf("Open failed: %s\n", xnGetStatusString(nRetVal));
            return (nRetVal);
        }
        
        nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
        if (nRetVal != XN_STATUS_OK)
        {
            printf("No depth generator found.");
            return 1;
        }
        
        nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
        if (nRetVal != XN_STATUS_OK)
        {
            nRetVal = g_UserGenerator.Create(g_Context);
            CHECK_RC(nRetVal, "Find user generator");
        }
        
        XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected, hCalibrationInProgress, hPoseInProgress;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
        {
            printf("Supplied user generator doesn't support skeleton\n");
            return 1;
        }
        nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
        CHECK_RC(nRetVal, "Register to user callbacks");
        nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
        CHECK_RC(nRetVal, "Register to calibration start");
        nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
        CHECK_RC(nRetVal, "Register to calibration complete");
        
        if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
        {
            g_bNeedPose = TRUE;
            if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            {
                printf("Pose required, but not supported\n");
                return 1;
            }
            nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
            CHECK_RC(nRetVal, "Register to Pose Detected");
            g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
            
            nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseInProgress(MyPoseInProgress, NULL, hPoseInProgress);
            CHECK_RC(nRetVal, "Register to pose in progress");
        }

        g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
        
        nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationInProgress(MyCalibrationInProgress, NULL, hCalibrationInProgress);
        CHECK_RC(nRetVal, "Register to calibration in progress");
        
        nRetVal = g_Context.StartGeneratingAll();
        CHECK_RC(nRetVal, "StartGenerating");

        return 0;
    }
}
