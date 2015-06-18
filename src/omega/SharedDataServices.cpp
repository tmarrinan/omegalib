/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/SharedDataServices.h"

using namespace omega;

ISharedData* SharedDataServices::mysSharedData = NULL;
Dictionary<String, SharedObject*> SharedDataServices::mysRegistrationQueue;

///////////////////////////////////////////////////////////////////////////////////////////////////
void SharedDataServices::setSharedData(ISharedData* data)
{
	mysSharedData = data;
	typedef Dictionary<String, SharedObject*>::Item SharedObjectEntry;
	foreach(SharedObjectEntry item, mysRegistrationQueue)
	{
		registerObject(item.getValue(), item.getKey());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SharedDataServices::registerObject(SharedObject* module, const String& sharedId)
{
	if(mysSharedData != NULL) 
	{
		mysSharedData->registerObject(module, sharedId);
	}
	else
	{
		//ofmsg("SharedDataServices::registerObject: queuing %1% for registration", %sharedId);
		// QUEUE
		mysRegistrationQueue[sharedId] = module;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SharedDataServices::unregisterObject(const String& id)
{
	if(mysSharedData != NULL) 
	{
		mysSharedData->unregisterObject(id);
	}
	else
	{
        // if no shared data stream is available, silently do nothing, since we will get
        // here when running non-cluster display systems like GLFW.
		//oferror("SharedDataServices::unregisterObject: shared data stream unavailable while unregistering %1%", %id);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void SharedDataServices::cleanup()
{
	// Shared data should take care of cleanup internally, here we just clean up the queue.
	mysRegistrationQueue.clear();
}
