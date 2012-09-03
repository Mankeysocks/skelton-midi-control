//
// This file was modified from its original form (OpenNi sample project NiUserTracker; file: SceneDrawer.cpp)
// by Brian Breitsch on 30 July 2012
//
// In accordance with the original license, this code is released under the GNU Lesser General Public License
//

// Includes
//---------------------------------------------------------------------------
#include <math.h>
#include "SmRenderer.h"
#include "SmOpenNiControl.h"
#include "SmControl.h"
#include "SmRealWorldTimer.h"
#include "SmUmbrellaHeader.h"
#include <boost/lexical_cast.hpp>

#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace sm {
    
    // A basic utility struct for representing window coordinates
    struct SmPoint{
        int X;
        int Y;
    };

    extern std::map<XnUInt32, std::pair<XnCalibrationStatus, XnPoseDetectionStatus> > m_Errors;
    
#define SM_USER_ID_HORIZONTAL_PLACEMENT 400
#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480
    float  offset = GL_WIN_SIZE_Y/2;
    int GLUT_WIN_X = GL_WIN_SIZE_X;
    int GLUT_WIN_Y = GL_WIN_SIZE_Y;
    
    extern xn::UserGenerator g_UserGenerator;
    extern xn::DepthGenerator g_DepthGenerator;
    extern xn::Context g_Context;
    
    XnBool g_bPause;
    XnBool g_bQuit;
    XnBool g_bDrawBackground;
    XnBool g_bDrawPixels;
    XnBool g_bDrawSkeleton;
    XnBool g_bPrintID;
    XnBool g_bPrintState;
    XnBool g_bPrintFrameID;
    XnBool g_bMarkJoints;
    XnBool g_bPrintUserIDs;
    
    static const float DEG2RAD = 3.14159/180;
    
    static GLubyte red[]    = { 255,   0,   0, 255 };
    static GLubyte green[]  = {   0, 255,   0, 255 };
    static GLubyte blue[]   = {   0,   0, 255, 255 };
    static GLubyte white[]  = { 255, 255, 255, 255 };
    static GLubyte teal[]   = {   0, 255, 255, 255 };
    static GLubyte black[]  = {   0,   0,   0, 255 };
    static GLubyte yellow[] = { 255, 255,   0, 255 };
    static GLubyte purple[] = { 255,   0, 255,   0 };
    
    bool playing_midi_test_note;
    int selectedUserNumber;
    
    float resolution;
    float scale;
    int user1_id;
    int user2_id;
    sm::JointType graph_joint_type;

    std::vector<sm::UserSkeleton*> users; // based on the user1 and user2 ids, we set this currentUser pointer
    UserSkeleton* currentUser;

    // Gets the SmJointType that corresponds to the given XnSkeletonJoint
    JointType getSmTypeEquivalent(XnSkeletonJoint joint)
    {
        switch (joint) {
            case XN_SKEL_HEAD:
                return sm::Head;
                break;
            case XN_SKEL_LEFT_SHOULDER:
                return sm::ShoulderLeft;
                break;
            case XN_SKEL_LEFT_ELBOW:
                return sm::ElbowLeft;
                break;
            case XN_SKEL_LEFT_WRIST:
                return sm::WristLeft;
                break;
            case XN_SKEL_LEFT_HAND:
                return sm::HandLeft;
                break;
            case XN_SKEL_LEFT_FINGERTIP:
                return sm::Null;
                break;
            case XN_SKEL_LEFT_HIP:
                return sm::HipLeft;
                break;
            case XN_SKEL_LEFT_KNEE:
                return sm::KneeLeft;
                break;
            case XN_SKEL_LEFT_ANKLE:
                return sm::AnkleLeft;
                break;
            case XN_SKEL_LEFT_FOOT:
                return sm::FootLeft;
                break;
            case XN_SKEL_RIGHT_SHOULDER:
                return sm::ShoulderRight;
                break;
            case XN_SKEL_RIGHT_ELBOW:
                return sm::ElbowRight;
                break;
            case XN_SKEL_RIGHT_WRIST:
                return sm::WristRight;
                break;
            case XN_SKEL_RIGHT_HAND:
                return sm::HandRight;
                break;
            case XN_SKEL_RIGHT_FINGERTIP:
                return sm::Null;
                break;
            case XN_SKEL_RIGHT_HIP:
                return sm::HipRight;
                break;
            case XN_SKEL_RIGHT_KNEE:
                return sm::KneeRight;
                break;
            case XN_SKEL_RIGHT_ANKLE:
                return sm::AnkleRight;
                break;
            case XN_SKEL_RIGHT_FOOT:
                return sm::FootRight;
                break;
            case XN_SKEL_LEFT_COLLAR:
                return sm::ShoulderLeft;
                break;
            case XN_SKEL_RIGHT_COLLAR:
                return sm::ShoulderRight;
                break;
            case XN_SKEL_NECK:
                return sm::ShoulderCenter;
                break;
            case XN_SKEL_TORSO:
                return sm::Spine;
                break;
            case XN_SKEL_WAIST:
                return sm::HipCenter;
                break;
        }
    }
    
    unsigned int getClosestPowerOfTwo(unsigned int n)
    {
        unsigned int m = 2;
        while(m < n) m<<=1;
        
        return m;
    }
    GLuint initTexture(void** buf, int& width, int& height)
    {
        GLuint texID = 0;
        glGenTextures(1,&texID);
        
        width = getClosestPowerOfTwo(width);
        height = getClosestPowerOfTwo(height);
        *buf = new unsigned char[width*height*4];
        glBindTexture(GL_TEXTURE_2D,texID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        return texID;
    }
    
    GLfloat texcoords[8];
    void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
    {
        GLfloat verts[8] = {	topLeftX, topLeftY,
            topLeftX, bottomRightY,
            bottomRightX, bottomRightY,
            bottomRightX, topLeftY
        };
        glVertexPointer(2, GL_FLOAT, 0, verts);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        //TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
        glFlush();
    }
    void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        
        DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);
        
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    XnFloat Colors[][3] =
    {
        {0,1,1},
        {0,0,1},
        {0,1,0},
        {1,1,0},
        {1,0,0},
        {1,.5,0},
        {.5,1,0},
        {0,.5,1},
        {.5,0,1},
        {1,1,.5},
        {1,1,1}
    };
    XnUInt32 nColors = 10;
    
    void glPrintString(void *font, char *str)
    {
        int i,l = (int)strlen(str);
        
        for(i=0; i<l; i++)
        {
            glutBitmapCharacter(font,*str++);
        }
    }
    
    void glPrintString(void *font, std::string str, float startX, float startY, int text_width, int text_height)
    {
        int x = startX+text_width/2;
        int y = startY+text_height/2;
        
        for(int i=0; i<str.length(); i++)
        {
            glRasterPos2i(x,y);
            glutBitmapCharacter(font,str.at(i));
            x+=text_width;
        }
    }
    
    bool DrawLimb(XnUserID player, XnSkeletonJoint eJoint1, XnSkeletonJoint eJoint2)
    {
        if (!g_UserGenerator.GetSkeletonCap().IsTracking(player))
        {
            printf("not tracked!\n");
            return true;
        }
        
        if (!g_UserGenerator.GetSkeletonCap().IsJointActive(eJoint1) ||
            !g_UserGenerator.GetSkeletonCap().IsJointActive(eJoint2))
        {
            return false;
        }
        
        XnSkeletonJointPosition joint1, joint2;
        g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint1, joint1);
        g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint2, joint2);
        
        if (joint1.fConfidence < 0.5 || joint2.fConfidence < 0.5)
        {
            return true;
        }
        
        XnPoint3D pt[2];
        pt[0] = joint1.position;
        pt[1] = joint2.position;
        
        g_DepthGenerator.ConvertRealWorldToProjective(2, pt, pt);
        
        glVertex3i(pt[0].X, pt[0].Y, 0);
        glVertex3i(pt[1].X, pt[1].Y, 0);
        
        return true;
    }
    
    void DrawCircle(float x, float y, float radius)
    {
        glBegin(GL_TRIANGLE_FAN);

        for (int i=0; i < 360; i++)
        {
            float degInRad = i*DEG2RAD;
            glVertex2f(x + cos(degInRad)*radius, y + sin(degInRad)*radius);
        }
        
        glEnd();
    }
    
    
    void DrawJoint(XnUserID player, XnSkeletonJoint eJoint, float dt)
    {
        if (!g_UserGenerator.GetSkeletonCap().IsTracking(player))
        {
            printf("not tracked!\n");
            return;
        }
        
        if (!g_UserGenerator.GetSkeletonCap().IsJointActive(eJoint))
        {
            return;
        }
        
        XnSkeletonJointPosition joint;
        g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, eJoint, joint);
        
        if (joint.fConfidence < 0.5)
        {
            return;
        }
        
        XnPoint3D pt;
        pt = joint.position;
        
        // Add the data from the joint to our structures
        JointMotionData* data;
        if((data = currentUser->getJointMotionlDataObject(getSmTypeEquivalent(eJoint))) != NULL){
            data->addData(pt.X, pt.Y, pt.Z, dt);
        }
        
        g_DepthGenerator.ConvertRealWorldToProjective(1, &pt, &pt);
        if(getSmTypeEquivalent(eJoint) == graph_joint_type){
            glColor4ub(0, 255, 0, 1);
            DrawCircle(pt.X, pt.Y, 2);
            glColor4ub(255, 0, 0, 1);

        } else {
        glColor4f(1, 0, 0, 1);
        DrawCircle(pt.X, pt.Y, 2);
        }
    }
    
    const XnChar* GetCalibrationErrorString(XnCalibrationStatus error)
    {
        switch (error)
        {
            case XN_CALIBRATION_STATUS_OK:
                return "OK";
            case XN_CALIBRATION_STATUS_NO_USER:
                return "NoUser";
            case XN_CALIBRATION_STATUS_ARM:
                return "Arm";
            case XN_CALIBRATION_STATUS_LEG:
                return "Leg";
            case XN_CALIBRATION_STATUS_HEAD:
                return "Head";
            case XN_CALIBRATION_STATUS_TORSO:
                return "Torso";
            case XN_CALIBRATION_STATUS_TOP_FOV:
                return "Top FOV";
            case XN_CALIBRATION_STATUS_SIDE_FOV:
                return "Side FOV";
            case XN_CALIBRATION_STATUS_POSE:
                return "Pose";
            default:
                return "Unknown";
        }
    }
    const XnChar* GetPoseErrorString(XnPoseDetectionStatus error)
    {
        switch (error)
        {
            case XN_POSE_DETECTION_STATUS_OK:
                return "OK";
            case XN_POSE_DETECTION_STATUS_NO_USER:
                return "NoUser";
            case XN_POSE_DETECTION_STATUS_TOP_FOV:
                return "Top FOV";
            case XN_POSE_DETECTION_STATUS_SIDE_FOV:
                return "Side FOV";
            case XN_POSE_DETECTION_STATUS_ERROR:
                return "General error";
            default:
                return "Unknown";
        }
    }
    
    void DrawGraph()
    {
        SmPoint point1;
        SmPoint point2;
        glColor4b(100, 200, 100, 1);
        sm::JointMotionData* data;
        if((data = currentUser->getJointMotionlDataObject(graph_joint_type)) != NULL){
            
            //keeps track of the index of the buffer to draw based on the resolution we are trying to draw at
            if (resolution >= 1) // each pixel represents multiple points, so decrement dIndex by the res points per pixel
            {
                float dIndex = data->index; // starts at x (the point in the buffer we most recently wote to)
                
                point2.X = GLUT_WIN_X;
                point2.Y = scale * data->position[(int)dIndex].X + offset;//OPTIMIZED DRAWING CHEKC FOR WORKING
                for (int i = GLUT_WIN_X; i > 0; i--) // TODO Must change if image width changed in xaml
                {
                    SmPoint temp = point1;
                    point1 = point2;
                    point2 = temp;
                    dIndex -= resolution;
                    if (dIndex < 0)
                        dIndex += SM_BUFFER_SIZE;
                    point2.X = i - 1; // TODO optimize by setting point1 to point2
                    point2.Y = scale * data->position[(int)dIndex].X + offset;
                    glVertex2d(point1.X, point1.Y);
                    glVertex2d(point2.X, point2.Y);
                }
                
            }
            else if (resolution > 0) // one point spans some pixels, so decrement dIndex by 1
            {
                float pIndex = (float)GLUT_WIN_X;                  //index of the pixel X to draw next, 0 < pINdex < image.Width
                int numToDraw = (int)(GLUT_WIN_X * resolution);    // the number of points we'll draw
                float widthToMove = 1 / resolution;
                
                point2.X = (int)pIndex;
                point2.Y = scale * data->position[data->index].X + offset; //OPTIMIZED CHECK FOR WORKING
                for (int i = 0; i < numToDraw; i++)
                {
                    SmPoint temp = point1;
                    point1 = point2;
                    point2 = temp;
                    int index = (data->index - i + SM_BUFFER_SIZE) % SM_BUFFER_SIZE;
                    pIndex -= widthToMove;
                    if (pIndex < 0)
                        pIndex += SM_BUFFER_SIZE;
                    point2.X = (int)pIndex;
                    point2.Y = scale * data->position[index].X + offset;
                    glVertex2d(point1.X, point1.Y);
                    glVertex2d(point2.X, point2.Y);
                }
            }
            
        }
    }
    
    void DrawScene(const xn::DepthMetaData& dmd, const xn::SceneMetaData& smd)
    {
        static bool bInitialized = false;
        static GLuint depthTexID;
        static unsigned char* pDepthTexBuf;
        static int texWidth, texHeight;
        
        float topLeftX;
        float topLeftY;
        float bottomRightY;
        float bottomRightX;
        float texXpos;
        float texYpos;
        
        if(!bInitialized)
        {
            texWidth =  getClosestPowerOfTwo(dmd.XRes());
            texHeight = getClosestPowerOfTwo(dmd.YRes());
            
            //		printf("Initializing depth texture: width = %d, height = %d\n", texWidth, texHeight);
            depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;
            
            //		printf("Initialized depth texture: width = %d, height = %d\n", texWidth, texHeight);
            bInitialized = true;
            
            topLeftX = dmd.XRes();
            topLeftY = 0;
            bottomRightY = dmd.YRes();
            bottomRightX = 0;
            texXpos =(float)dmd.XRes()/texWidth;
            texYpos  =(float)dmd.YRes()/texHeight;
            
            memset(texcoords, 0, 8*sizeof(float));
            texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;
        }
        
        unsigned int nValue = 0;
        unsigned int nHistValue = 0;
        unsigned int nIndex = 0;
        unsigned int nX = 0;
        unsigned int nY = 0;
        unsigned int nNumberOfPoints = 0;
        XnUInt16 g_nXRes = dmd.XRes();
        XnUInt16 g_nYRes = dmd.YRes();
        
        unsigned char* pDestImage = pDepthTexBuf;
        
        const XnDepthPixel* pDepth = dmd.Data();
        const XnLabel* pLabels = smd.Data();
        
        static unsigned int nZRes = dmd.ZRes();
        static float* pDepthHist = (float*)malloc(nZRes* sizeof(float));
        
        // Calculate the accumulative histogram
        memset(pDepthHist, 0, nZRes*sizeof(float));
        for (nY=0; nY<g_nYRes; nY++)
        {
            for (nX=0; nX<g_nXRes; nX++)
            {
                nValue = *pDepth;
                
                if (nValue != 0)
                {
                    pDepthHist[nValue]++;
                    nNumberOfPoints++;
                }
                
                pDepth++;
            }
        }
        
        for (nIndex=1; nIndex<nZRes; nIndex++)
        {
            pDepthHist[nIndex] += pDepthHist[nIndex-1];
        }
        if (nNumberOfPoints)
        {
            for (nIndex=1; nIndex<nZRes; nIndex++)
            {
                pDepthHist[nIndex] = (unsigned int)(256 * (1.0f - (pDepthHist[nIndex] / nNumberOfPoints)));
            }
        }
        
        pDepth = dmd.Data();
        if (g_bDrawPixels)
        {
            XnUInt32 nIndex = 0;
            // Prepare the texture map
            for (nY=0; nY<g_nYRes; nY++)
            {
                for (nX=0; nX < g_nXRes; nX++, nIndex++)
                {
                    
                    pDestImage[0] = 0;
                    pDestImage[1] = 0;
                    pDestImage[2] = 0;
                    if (g_bDrawBackground || *pLabels != 0)
                    {
                        nValue = *pDepth;
                        XnLabel label = *pLabels;
                        XnUInt32 nColorID = label % nColors;
                        if (label == 0)
                        {
                            nColorID = nColors;
                        }
                        
                        if (nValue != 0)
                        {
                            nHistValue = pDepthHist[nValue];
                            
                            pDestImage[0] = nHistValue * Colors[nColorID][0];
                            pDestImage[1] = nHistValue * Colors[nColorID][1];
                            pDestImage[2] = nHistValue * Colors[nColorID][2];
                        }
                    }
                    
                    pDepth++;
                    pLabels++;
                    pDestImage+=3;
                }
                
                pDestImage += (texWidth - g_nXRes) *3;
            }
        }
        else
        {
            xnOSMemSet(pDepthTexBuf, 0, 3*2*g_nXRes*g_nYRes);
        }
        
        glBindTexture(GL_TEXTURE_2D, depthTexID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pDepthTexBuf);
        
        // Display the OpenGL texture map
        glColor4f(0.75,0.75,0.75,1);
        
        glEnable(GL_TEXTURE_2D);
        DrawTexture(dmd.XRes(),dmd.YRes(),0,0);
        glDisable(GL_TEXTURE_2D);
        
        char strLabel[50] = "";
        XnUserID aUsers[SM_NUM_USERS_TO_TRACK];
        XnUInt16 nUsers = SM_NUM_USERS_TO_TRACK;
        g_UserGenerator.GetUsers(aUsers, nUsers);

        for (int i = 0; i < nUsers; ++i)
        {
            if (i > 0) {
                printf("break?\n"); // TESTING CODE!!!
            }
            if (g_bPrintID)
            {
                XnPoint3D com; // Get the user's center of mass
                g_UserGenerator.GetCoM(aUsers[i], com);
                g_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);
                
                XnUInt32 nDummy = 0;
                
                xnOSMemSet(strLabel, 0, sizeof(strLabel));
                if (!g_bPrintState)
                {
                    // Tracking
                    xnOSStrFormat(strLabel, sizeof(strLabel), &nDummy, "%d", aUsers[i]);
                }
                else if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
                {
                    // Tracking
                    xnOSStrFormat(strLabel, sizeof(strLabel), &nDummy, "%d - Tracking", aUsers[i]);
                }
                else if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUsers[i]))
                {
                    // Calibrating
                    xnOSStrFormat(strLabel, sizeof(strLabel), &nDummy, "%d - Calibrating [%s]", aUsers[i], GetCalibrationErrorString(m_Errors[aUsers[i]].first));
                }
                else
                {
                    // Nothing
                    xnOSStrFormat(strLabel, sizeof(strLabel), &nDummy, "%d - Looking for pose [%s]", aUsers[i], GetPoseErrorString(m_Errors[aUsers[i]].second));
                }
                
                glColor4ub(255, 0, 0, 1);
                
                glRasterPos2i(com.X, com.Y);
                glPrintString(GLUT_BITMAP_HELVETICA_18, strLabel);
            }
            // if it exists, find the user with the skeleton number corresponding to the current openni skeleton
            currentUser = NULL;
            std::vector<UserSkeleton*>::iterator it;
            for (it = users.begin(); it != users.end(); ++it) {
                if ((*it)->skeleton_number == i) {
                    currentUser = *it;
                    break;
                }
            }
            
            if (g_bDrawSkeleton && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i]) && currentUser != NULL)
            {
               glColor4ub(255, 0, 0, 1);
                
                // Draw Joints
                if (g_bMarkJoints)
                {
                    float dt = currentUser->timer.getElapsedTime();
                    
                    // Try to draw all joints
                    DrawJoint(aUsers[i], XN_SKEL_HEAD, dt);
                    DrawJoint(aUsers[i], XN_SKEL_NECK, dt);
                    DrawJoint(aUsers[i], XN_SKEL_TORSO, dt);
                    DrawJoint(aUsers[i], XN_SKEL_WAIST, dt);
                    
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_COLLAR, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_SHOULDER, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_ELBOW, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_WRIST, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_HAND, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_FINGERTIP, dt);
                    
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_COLLAR, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_SHOULDER, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_ELBOW, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_WRIST, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_HAND, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_FINGERTIP, dt);
                    
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_HIP, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_KNEE, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_ANKLE, dt);
                    DrawJoint(aUsers[i], XN_SKEL_LEFT_FOOT, dt);
                    
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_HIP, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_KNEE, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_ANKLE, dt);
                    DrawJoint(aUsers[i], XN_SKEL_RIGHT_FOOT, dt);
                }
                
                glBegin(GL_LINES);
                
                // Draw Limbs
                DrawLimb(aUsers[i], XN_SKEL_HEAD, XN_SKEL_NECK);
                
                DrawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER);
                DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW);
                if (!DrawLimb(aUsers[i], XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_WRIST))
                {
                    DrawLimb(aUsers[i], XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND);
                }
                else
                {
                    DrawLimb(aUsers[i], XN_SKEL_LEFT_WRIST, XN_SKEL_LEFT_HAND);
                    DrawLimb(aUsers[i], XN_SKEL_LEFT_HAND, XN_SKEL_LEFT_FINGERTIP);
                }
                
                
                DrawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER);
                DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
                if (!DrawLimb(aUsers[i], XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_WRIST))
                {
                    DrawLimb(aUsers[i], XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);
                }
                else
                {
                    DrawLimb(aUsers[i], XN_SKEL_RIGHT_WRIST, XN_SKEL_RIGHT_HAND);
                    DrawLimb(aUsers[i], XN_SKEL_RIGHT_HAND, XN_SKEL_RIGHT_FINGERTIP);
                }
                
                DrawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
                DrawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO);
                
                DrawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
                DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
                DrawLimb(aUsers[i], XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);
                
                DrawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);
                DrawLimb(aUsers[i], XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
                DrawLimb(aUsers[i], XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);
                
                DrawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);
                DrawGraph();
                glEnd();
            }
        }
        
        if (g_bPrintFrameID)
        {
            static XnChar strFrameID[80];
            xnOSMemSet(strFrameID, 0, 80);
            XnUInt32 nDummy = 0;
            xnOSStrFormat(strFrameID, sizeof(strFrameID), &nDummy, "%d", dmd.FrameID());
            
            glColor4f(1, 0, 0, 1);
            
            glRasterPos2i(10, 20);
            
            glPrintString(GLUT_BITMAP_HELVETICA_18, strFrameID);
            
        }
        
        if (g_bPrintUserIDs) {
            std::vector<UserSkeleton*>::iterator it;
            int offset_counter = 0;
            for (it = users.begin(); it != users.end(); ++it) {
                glColor4f(.5, .8, 1, 1);
                if (offset_counter == selectedUserNumber) {
                    glColor4f(.2, 1, 1, 1);
                }
                ++offset_counter;
                glPrintString(GLUT_BITMAP_TIMES_ROMAN_10, "User " + boost::lexical_cast<std::string>(offset_counter) + " skel#: " + boost::lexical_cast<std::string>((*it)->skeleton_number), SM_USER_ID_HORIZONTAL_PLACEMENT, 10 + offset_counter*10,10,10);
            }
            
        }
        
    }
    
    void DrawMenu()
    {
        //glColor4ub(200, 240, 190, 1);
        glColor3ubv(teal);
        std::string options("options");
        glPrintString(GLUT_BITMAP_TIMES_ROMAN_24,options,100,100,24,24);
    }
    
    void glutKeyboard (unsigned char key, int /*x*/, int /*y*/)
    {
        // If we are paused, we want to define different actions than if we are not paused
        // 'p' should always be pause/unpause
        if (g_bPause) {
            switch (key)
            {
                case 27:
                    CleanupExit();
                case'p':
                    g_bPause = !g_bPause; // If paused, unpause
                    break;
            }
        }
        else {
            switch (key)
            {
                case 27:
                    CleanupExit();
                case  'b':
                    // Draw background?
                    g_bDrawBackground = !g_bDrawBackground;
                    break;
                case 'P':
                    // Draw pixels at all?
                    g_bDrawPixels = !g_bDrawPixels;
                    break;
                case 's':
                    // Draw Skeleton?
                    g_bDrawSkeleton = !g_bDrawSkeleton;
                    break;
                case 'i':
                    // Print label?
                    g_bPrintID = !g_bPrintID;
                    break;
                case 'l':
                    // Print ID & state as label, or only ID?
                    g_bPrintState = !g_bPrintState;
                    break;
                case 'f':
                    // Print FrameID
                    g_bPrintFrameID = !g_bPrintFrameID;
                    break;
                case 'j':
                    // Mark joints
                    g_bMarkJoints = !g_bMarkJoints;
                    break;
                case'p':
                    g_bPause = !g_bPause;
                    break;
                case 'm':
                    if (!playing_midi_test_note) {
                        sm::sendMidiMessage(rt::CommandName::NoteOn, rt::Channel::Channel1, rt::Pitch::CSharp5, 100);
                        playing_midi_test_note = true;
                    }
                    break;
                case 'u':{
                    int num_users = users.size();
                    selectedUserNumber = (selectedUserNumber + 1)%num_users;
                }
                    break;
                case 'U':
                    users[selectedUserNumber]->skeleton_number = (users[selectedUserNumber]->skeleton_number + 1) % SM_NUM_USERS_TO_TRACK;
                    break;
                case 'o':
                    sm::sendMidiMessage(rt::CommandName::ControlChange, rt::Channel::Channel1, rt::Control::AllNotesOff, 0);
                    sm::sendMidiMessage(rt::CommandName::ControlChange, rt::Channel::Channel2, rt::Control::AllNotesOff, 0);
                    sm::sendMidiMessage(rt::CommandName::ControlChange, rt::Channel::Channel3, rt::Control::AllNotesOff, 0);
                    break;
            }
        }
    }
    
    void glutKeyboardUp (unsigned char key, int /*x*/, int /*y*/)
    {
        // If we are paused, we want to define different actions than if we are not paused
        // 'p' should always be pause/unpause
        if (g_bPause) {
            
        }
        else {
            switch (key)
            {
                case 'm':
                    sm::sendMidiMessage(rt::CommandName::NoteOff, rt::Channel::Channel1, rt::Pitch::CSharp5, 100);
                    playing_midi_test_note = false;
                    break;
                case 'u':
                    //user1_id = (user1_id + 1)%SM_NUM_USERS_TO_TRACK;
                    break;
                case 'U':
                    //user2_id = (user2_id + 1)%SM_NUM_USERS_TO_TRACK;
                    break;
            }
        }
    }

    
    // this function is called each frame
    void glutDisplay (void)
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Setup the OpenGL viewpoint
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        xn::SceneMetaData sceneMD;
        xn::DepthMetaData depthMD;
        g_DepthGenerator.GetMetaData(depthMD);
        glOrtho(0, depthMD.XRes(), depthMD.YRes(), 0, -1.0, 1.0);
        
        glDisable(GL_TEXTURE_2D);
        
        if (!g_bPause)
        {
            // Read next available data
            g_Context.WaitOneUpdateAll(g_UserGenerator);
        }
        
		// Process the data
		g_DepthGenerator.GetMetaData(depthMD);
		g_UserGenerator.GetUserPixels(0, sceneMD);
		
        DrawScene(depthMD, sceneMD);
        
        if (g_bPause)
        {
            DrawMenu();
        }
        
        glutSwapBuffers();
    }
    
    void glutIdle (void)
    {
        if (g_bQuit) {
            CleanupExit();
        }
        // Display the frame
        glutPostRedisplay();
    }
    
    
    void glInit (int * pargc, char ** argv)
    {
        glutInit(pargc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
        glutCreateWindow ("User Tracker Viewer");
        glutKeyboardFunc(glutKeyboard); // Keyboard events handled in MenuDrawer
        glutKeyboardUpFunc(glutKeyboardUp); // Keyboard events handled in MenuDrawer
        glutDisplayFunc(glutDisplay);   // Display function handled (here) in OpenNiControl
        glutIdleFunc(glutIdle);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        
        glutAttachMenu(GLUT_RIGHT_BUTTON);;
    }
    
    int smBeginRendering(int argc, char* argv[],std::vector<UserSkeleton*> usrs)
    {
        users = usrs;
        currentUser = NULL;
        g_bPause = FALSE;
        g_bQuit = FALSE;
        g_bDrawBackground = TRUE;
        g_bDrawPixels = TRUE;
        g_bDrawSkeleton = TRUE;
        g_bPrintID = TRUE;
        g_bPrintState = TRUE;
        g_bPrintFrameID = TRUE;
        g_bMarkJoints = TRUE;
        g_bPrintUserIDs = TRUE;
        glInit(&argc, argv);
        glutMainLoop();
        printf("Should never print...");
        return 1;
    }

}