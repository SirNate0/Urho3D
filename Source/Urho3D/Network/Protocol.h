//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <RakNet/MessageIdentifiers.h>

namespace Urho3D
{

#define URHO3D_NETWORK_USER_MSGID(ID) ID_USER_PACKET_ENUM + ID

/// Client->server: send VariantMap of identity and authentication data.
static const int MSG_IDENTITY = URHO3D_NETWORK_USER_MSGID(0);
/// Client->server: send controls (buttons and mouse movement.)
static const int MSG_CONTROLS = URHO3D_NETWORK_USER_MSGID(1);
/// Client->server: scene has been loaded and client is ready to proceed.
static const int MSG_SCENELOADED = URHO3D_NETWORK_USER_MSGID(2);
/// Client->server: request a package file.
static const int MSG_REQUESTPACKAGE = URHO3D_NETWORK_USER_MSGID(3);

/// Server->client: package file data fragment.
static const int MSG_PACKAGEDATA = URHO3D_NETWORK_USER_MSGID(4);
/// Server->client: load new scene. In case of empty filename the client should just empty the scene.
static const int MSG_LOADSCENE = URHO3D_NETWORK_USER_MSGID(5);
/// Server->client: wrong scene checksum, can not participate.
static const int MSG_SCENECHECKSUMERROR = URHO3D_NETWORK_USER_MSGID(6);
/// Server->client: create new node.
static const int MSG_CREATENODE = URHO3D_NETWORK_USER_MSGID(7);
/// Server->client: node delta update.
static const int MSG_NODEDELTAUPDATE = URHO3D_NETWORK_USER_MSGID(8);
/// Server->client: node latest data update.
static const int MSG_NODELATESTDATA = URHO3D_NETWORK_USER_MSGID(9);
/// Server->client: remove node.
static const int MSG_REMOVENODE = URHO3D_NETWORK_USER_MSGID(10);
/// Server->client: create new component.
static const int MSG_CREATECOMPONENT = URHO3D_NETWORK_USER_MSGID(11);
/// Server->client: component delta update.
static const int MSG_COMPONENTDELTAUPDATE = URHO3D_NETWORK_USER_MSGID(12);
/// Server->client: component latest data update.
static const int MSG_COMPONENTLATESTDATA = URHO3D_NETWORK_USER_MSGID(13);
/// Server->client: remove component.
static const int MSG_REMOVECOMPONENT = URHO3D_NETWORK_USER_MSGID(14);

/// Client->server and server->client: remote event.
static const int MSG_REMOTEEVENT = URHO3D_NETWORK_USER_MSGID(15);
/// Client->server and server->client: remote node event.
static const int MSG_REMOTENODEEVENT = URHO3D_NETWORK_USER_MSGID(16);
/// Server->client: info about package.
static const int MSG_PACKAGEINFO = URHO3D_NETWORK_USER_MSGID(17);

/// Fixed content ID for client controls update.
static const unsigned CONTROLS_CONTENT_ID = 1;
/// Package file fragment size.
static const unsigned PACKAGE_FRAGMENT_SIZE = 1024;

}
