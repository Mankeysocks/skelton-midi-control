//
// This file was modified from its original form (OpenNi sample project NiUserTracker; file: SceneDrawer.h)
// by Brian Breitsch on 30 July 2012
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
#ifndef XNV_POINT_DRAWER_H_
#define XNV_POINT_DRAWER_H_

#include <XnCppWrapper.h>
#include "SmUserSkeleton.h"

namespace sm {
    
    int smBeginRendering(int argc, char* argv[], std::vector<UserSkeleton*> usrs);

}

#endif
