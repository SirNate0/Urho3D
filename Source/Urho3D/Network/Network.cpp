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

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Core/CoreEvents.h"
#include "../Core/Profiler.h"
#include "../Engine/EngineEvents.h"
#include "../IO/FileSystem.h"
#include "../Input/InputEvents.h"
#include "../IO/IOEvents.h"
#include "../IO/Log.h"
#include "../IO/MemoryBuffer.h"
#include "../Network/HttpRequest.h"
#include "../Network/Network.h"
#include "../Network/NetworkEvents.h"
#include "../Network/NetworkPriority.h"
#include "../Network/Protocol.h"
#include "../Scene/Scene.h"

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetTypes.h>

#include "../DebugNew.h"

namespace Urho3D
{

static const char* RAKNET_MESSAGEID_STRINGS[] = {
    "ID_CONNECTED_PING",
    "ID_UNCONNECTED_PING",
    "ID_UNCONNECTED_PING_OPEN_CONNECTIONS",
    "ID_CONNECTED_PONG",
    "ID_DETECT_LOST_CONNECTIONS",
    "ID_OPEN_CONNECTION_REQUEST_1",
    "ID_OPEN_CONNECTION_REPLY_1",
    "ID_OPEN_CONNECTION_REQUEST_2",
    "ID_OPEN_CONNECTION_REPLY_2",
    "ID_CONNECTION_REQUEST",
    "ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY",
    "ID_OUR_SYSTEM_REQUIRES_SECURITY",
    "ID_PUBLIC_KEY_MISMATCH",
    "ID_OUT_OF_BAND_INTERNAL",
    "ID_SND_RECEIPT_ACKED",
    "ID_SND_RECEIPT_LOSS",
    "ID_CONNECTION_REQUEST_ACCEPTED",
    "ID_CONNECTION_ATTEMPT_FAILED",
    "ID_ALREADY_CONNECTED",
    "ID_NEW_INCOMING_CONNECTION",
    "ID_NO_FREE_INCOMING_CONNECTIONS",
    "ID_DISCONNECTION_NOTIFICATION",
    "ID_CONNECTION_LOST",
    "ID_CONNECTION_BANNED",
    "ID_INVALID_PASSWORD",
    "ID_INCOMPATIBLE_PROTOCOL_VERSION",
    "ID_IP_RECENTLY_CONNECTED",
    "ID_TIMESTAMP",
    "ID_UNCONNECTED_PONG",
    "ID_ADVERTISE_SYSTEM",
    "ID_DOWNLOAD_PROGRESS",
    "ID_REMOTE_DISCONNECTION_NOTIFICATION",
    "ID_REMOTE_CONNECTION_LOST",
    "ID_REMOTE_NEW_INCOMING_CONNECTION",
    "ID_FILE_LIST_TRANSFER_HEADER",
    "ID_FILE_LIST_TRANSFER_FILE",
    "ID_FILE_LIST_REFERENCE_PUSH_ACK",
    "ID_DDT_DOWNLOAD_REQUEST",
    "ID_TRANSPORT_STRING",
    "ID_REPLICA_MANAGER_CONSTRUCTION",
    "ID_REPLICA_MANAGER_SCOPE_CHANGE",
    "ID_REPLICA_MANAGER_SERIALIZE",
    "ID_REPLICA_MANAGER_DOWNLOAD_STARTED",
    "ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE",
    "ID_RAKVOICE_OPEN_CHANNEL_REQUEST",
    "ID_RAKVOICE_OPEN_CHANNEL_REPLY",
    "ID_RAKVOICE_CLOSE_CHANNEL",
    "ID_RAKVOICE_DATA",
    "ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE",
    "ID_AUTOPATCHER_CREATION_LIST",
    "ID_AUTOPATCHER_DELETION_LIST",
    "ID_AUTOPATCHER_GET_PATCH",
    "ID_AUTOPATCHER_PATCH_LIST",
    "ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR",
    "ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES",
    "ID_AUTOPATCHER_FINISHED_INTERNAL",
    "ID_AUTOPATCHER_FINISHED",
    "ID_AUTOPATCHER_RESTART_APPLICATION",
    "ID_NAT_PUNCHTHROUGH_REQUEST",
    "ID_NAT_CONNECT_AT_TIME",
    "ID_NAT_GET_MOST_RECENT_PORT",
    "ID_NAT_CLIENT_READY",
    "ID_NAT_TARGET_NOT_CONNECTED",
    "ID_NAT_TARGET_UNRESPONSIVE",
    "ID_NAT_CONNECTION_TO_TARGET_LOST",
    "ID_NAT_ALREADY_IN_PROGRESS",
    "ID_NAT_PUNCHTHROUGH_FAILED",
    "ID_NAT_PUNCHTHROUGH_SUCCEEDED",
    "ID_READY_EVENT_SET",
    "ID_READY_EVENT_UNSET",
    "ID_READY_EVENT_ALL_SET",
    "ID_READY_EVENT_QUERY",
    "ID_LOBBY_GENERAL",
    "ID_RPC_REMOTE_ERROR",
    "ID_RPC_PLUGIN",
    "ID_FILE_LIST_REFERENCE_PUSH",
    "ID_READY_EVENT_FORCE_ALL_SET",
    "ID_ROOMS_EXECUTE_FUNC",
    "ID_ROOMS_LOGON_STATUS",
    "ID_ROOMS_HANDLE_CHANGE",
    "ID_LOBBY2_SEND_MESSAGE",
    "ID_LOBBY2_SERVER_ERROR",
    "ID_FCM2_NEW_HOST",
    "ID_FCM2_REQUEST_FCMGUID",
    "ID_FCM2_RESPOND_CONNECTION_COUNT",
    "ID_FCM2_INFORM_FCMGUID",
    "ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT",
    "ID_FCM2_VERIFIED_JOIN_START",
    "ID_FCM2_VERIFIED_JOIN_CAPABLE",
    "ID_FCM2_VERIFIED_JOIN_FAILED",
    "ID_FCM2_VERIFIED_JOIN_ACCEPTED",
    "ID_FCM2_VERIFIED_JOIN_REJECTED",
    "ID_UDP_PROXY_GENERAL",
    "ID_SQLite3_EXEC",
    "ID_SQLite3_UNKNOWN_DB",
    "ID_SQLLITE_LOGGER",
    "ID_NAT_TYPE_DETECTION_REQUEST",
    "ID_NAT_TYPE_DETECTION_RESULT",
    "ID_ROUTER_2_INTERNAL",
    "ID_ROUTER_2_FORWARDING_NO_PATH",
    "ID_ROUTER_2_FORWARDING_ESTABLISHED",
    "ID_ROUTER_2_REROUTED",
    "ID_TEAM_BALANCER_INTERNAL",
    "ID_TEAM_BALANCER_REQUESTED_TEAM_FULL",
    "ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED",
    "ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED",
    "ID_TEAM_BALANCER_TEAM_ASSIGNED",
    "ID_LIGHTSPEED_INTEGRATION",
    "ID_XBOX_LOBBY",
    "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS",
    "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT",
    "ID_TWO_WAY_AUTHENTICATION_NEGOTIATION",
    "ID_CLOUD_POST_REQUEST",
    "ID_CLOUD_RELEASE_REQUEST",
    "ID_CLOUD_GET_REQUEST",
    "ID_CLOUD_GET_RESPONSE",
    "ID_CLOUD_UNSUBSCRIBE_REQUEST",
    "ID_CLOUD_SERVER_TO_SERVER_COMMAND",
    "ID_CLOUD_SUBSCRIPTION_NOTIFICATION",
    "ID_LIB_VOICE",
    "ID_RELAY_PLUGIN",
    "ID_NAT_REQUEST_BOUND_ADDRESSES",
    "ID_NAT_RESPOND_BOUND_ADDRESSES",
    "ID_FCM2_UPDATE_USER_CONTEXT",
    "ID_RESERVED_3",
    "ID_RESERVED_4",
    "ID_RESERVED_5",
    "ID_RESERVED_6",
    "ID_RESERVED_7",
    "ID_RESERVED_8",
    "ID_RESERVED_9",
    //"ID_USER_PACKET_ENUM"
    "MSG_IDENTITY",
    "MSG_CONTROLS",
    "MSG_SCENELOADED",
    "MSG_REQUESTPACKAGE",
    "MSG_PACKAGEDATA",
    "MSG_LOADSCENE",
    "MSG_SCENECHECKSUMERROR",
    "MSG_CREATENODE",
    "MSG_NODEDELTAUPDATE",
    "MSG_NODELATESTDATA",
    "MSG_REMOVENODE",
    "MSG_CREATECOMPONENT",
    "MSG_COMPONENTDELTAUPDATE",
    "MSG_COMPONENTLATESTDATA",
    "MSG_REMOVECOMPONENT",
    "MSG_REMOTEEVENT",
    "MSG_REMOTENODEEVENT",
    "MSG_PACKAGEINFO"
};

static const int DEFAULT_UPDATE_FPS = 30;

Network::Network(Context* context) :
    Object(context),
    updateFps_(DEFAULT_UPDATE_FPS),
    simulatedLatency_(0),
    simulatedPacketLoss_(0.0f),
    updateInterval_(1.0f / (float)DEFAULT_UPDATE_FPS),
    updateAcc_(0.0f)
{
    rakPeer_ = RakNet::RakPeerInterface::GetInstance();

    // Register Network library object factories
    RegisterNetworkLibrary(context_);

    SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(Network, HandleBeginFrame));
    SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(Network, HandleRenderUpdate));

    // Blacklist remote events which are not to be allowed to be registered in any case
    blacklistedRemoteEvents_.Insert(E_CONSOLECOMMAND);
    blacklistedRemoteEvents_.Insert(E_LOGMESSAGE);
    blacklistedRemoteEvents_.Insert(E_BEGINFRAME);
    blacklistedRemoteEvents_.Insert(E_UPDATE);
    blacklistedRemoteEvents_.Insert(E_POSTUPDATE);
    blacklistedRemoteEvents_.Insert(E_RENDERUPDATE);
    blacklistedRemoteEvents_.Insert(E_ENDFRAME);
    blacklistedRemoteEvents_.Insert(E_MOUSEBUTTONDOWN);
    blacklistedRemoteEvents_.Insert(E_MOUSEBUTTONUP);
    blacklistedRemoteEvents_.Insert(E_MOUSEMOVE);
    blacklistedRemoteEvents_.Insert(E_MOUSEWHEEL);
    blacklistedRemoteEvents_.Insert(E_KEYDOWN);
    blacklistedRemoteEvents_.Insert(E_KEYUP);
    blacklistedRemoteEvents_.Insert(E_TEXTINPUT);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKCONNECTED);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKDISCONNECTED);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKBUTTONDOWN);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKBUTTONUP);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKAXISMOVE);
    blacklistedRemoteEvents_.Insert(E_JOYSTICKHATMOVE);
    blacklistedRemoteEvents_.Insert(E_TOUCHBEGIN);
    blacklistedRemoteEvents_.Insert(E_TOUCHEND);
    blacklistedRemoteEvents_.Insert(E_TOUCHMOVE);
    blacklistedRemoteEvents_.Insert(E_GESTURERECORDED);
    blacklistedRemoteEvents_.Insert(E_GESTUREINPUT);
    blacklistedRemoteEvents_.Insert(E_MULTIGESTURE);
    blacklistedRemoteEvents_.Insert(E_DROPFILE);
    blacklistedRemoteEvents_.Insert(E_INPUTFOCUS);
    blacklistedRemoteEvents_.Insert(E_MOUSEVISIBLECHANGED);
    blacklistedRemoteEvents_.Insert(E_EXITREQUESTED);
    blacklistedRemoteEvents_.Insert(E_SERVERCONNECTED);
    blacklistedRemoteEvents_.Insert(E_SERVERDISCONNECTED);
    blacklistedRemoteEvents_.Insert(E_CONNECTFAILED);
    blacklistedRemoteEvents_.Insert(E_CLIENTCONNECTED);
    blacklistedRemoteEvents_.Insert(E_CLIENTDISCONNECTED);
    blacklistedRemoteEvents_.Insert(E_CLIENTIDENTITY);
    blacklistedRemoteEvents_.Insert(E_CLIENTSCENELOADED);
    blacklistedRemoteEvents_.Insert(E_NETWORKMESSAGE);
    blacklistedRemoteEvents_.Insert(E_NETWORKUPDATE);
    blacklistedRemoteEvents_.Insert(E_NETWORKUPDATESENT);
    blacklistedRemoteEvents_.Insert(E_NETWORKSCENELOADFAILED);
}

Network::~Network()
{
    // If server connection exists, disconnect, but do not send an event because we are shutting down
    Disconnect(100);
    serverConnection_.Reset();

    clientConnections_.Clear();

    RakNet::RakPeerInterface::DestroyInstance(rakPeer_);
    rakPeer_ = 0;
}

void Network::HandleMessage(const RakNet::AddressOrGUID& source, int packetId, int msgId, const char* data, size_t numBytes)
{
    // Only process messages from known sources
    Connection* connection = GetConnection(source);
    if (connection)
    {
        MemoryBuffer msg(data, (unsigned)numBytes);
        if (connection->ProcessMessage((int)msgId, msg))
            return;

        // If message was not handled internally, forward as an event
        using namespace NetworkMessage;

        VariantMap& eventData = GetEventDataMap();
        eventData[P_CONNECTION] = connection;
        eventData[P_MESSAGEID] = (int)msgId;
        eventData[P_DATA].SetBuffer(msg.GetData(), msg.GetSize());
        connection->SendEvent(E_NETWORKMESSAGE, eventData);
    }
    else
        URHO3D_LOGWARNING("Discarding message from unknown MessageConnection " + String(source.ToString()));
}

void Network::NewConnectionEstablished(const RakNet::AddressOrGUID& connection)
{
    // Create a new client connection corresponding to this MessageConnection
    SharedPtr<Connection> newConnection(new Connection(context_, true, connection, rakPeer_));
    newConnection->ConfigureNetworkSimulator(simulatedLatency_, simulatedPacketLoss_);
    clientConnections_[connection] = newConnection;
    URHO3D_LOGINFO("Client " + newConnection->ToString() + " connected");

    using namespace ClientConnected;

    VariantMap& eventData = GetEventDataMap();
    eventData[P_CONNECTION] = newConnection;
    newConnection->SendEvent(E_CLIENTCONNECTED, eventData);
}

void Network::ClientDisconnected(const RakNet::AddressOrGUID& connection)
{
    // Remove the client connection that corresponds to this MessageConnection
    HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Find(connection);
    if (i != clientConnections_.End())
    {
        Connection* connection = i->second_;
        URHO3D_LOGINFO("Client " + connection->ToString() + " disconnected");

        using namespace ClientDisconnected;

        VariantMap& eventData = GetEventDataMap();
        eventData[P_CONNECTION] = connection;
        connection->SendEvent(E_CLIENTDISCONNECTED, eventData);

        clientConnections_.Erase(i);
    }
}

void Network::SetDiscoveryBeacon(const VariantMap& data)
{
    VectorBuffer buffer;
    buffer.WriteVariantMap(data);
    if (buffer.GetSize() > 400)
        URHO3D_LOGERROR("Discovery beacon of size: " + String(buffer.GetSize()) + " bytes is too large, modify MAX_OFFLINE_DATA_LENGTH in RakNet or reduce size");
    rakPeer_->SetOfflinePingResponse((const char*)buffer.GetData(), buffer.GetSize());
}

void Network::DiscoverHosts(unsigned port)
{
    // JSandusky: Contrary to the manual, we actually do have to perform Startup first before we can Ping
    if (rakPeer_ && !rakPeer_->IsActive())
    {
        RakNet::SocketDescriptor socket;
        rakPeer_->Startup(1, &socket, 1);
    }
    rakPeer_->Ping("255.255.255.255", port, false);
}

bool Network::Connect(const String& address, unsigned short port, Scene* scene, const VariantMap& identity)
{
    URHO3D_PROFILE(Connect);

    if (rakPeer_->IsActive())
    {
        /// Allow 300 ms to notify
        rakPeer_->Shutdown(300);
    }

    isServer_ = false;
    RakNet::SocketDescriptor socket;
    socket.socketFamily=AF_INET6; // Test out IPV6
    rakPeer_->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult connectResult = rakPeer_->Connect(address.CString(), port, 0, 0);
    if (connectResult != RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        URHO3D_LOGERROR("Failed to connect to server " + address + ":" + String(port) + ", error code: " + String((int)connectResult));
        SendEvent(E_CONNECTFAILED);
        return false;
    }
    else
    {
        serverConnection_ = new Connection(context_, false, rakPeer_->GetMyBoundAddress(), rakPeer_);
        serverConnection_->SetScene(scene);
        serverConnection_->SetIdentity(identity);
        serverConnection_->SetConnectPending(true);
        serverConnection_->ConfigureNetworkSimulator(simulatedLatency_, simulatedPacketLoss_);

        URHO3D_LOGINFO("Connecting to server " + serverConnection_->ToString());
        return true;
    }
}

void Network::Disconnect(int waitMSec)
{
    if (!serverConnection_)
        return;

    URHO3D_PROFILE(Disconnect);
    serverConnection_->Disconnect(waitMSec);
}

bool Network::StartServer(unsigned short port)
{
    if (IsServerRunning())
        return true;

    URHO3D_PROFILE(StartServer);

//    RakNet::SocketDescriptor socket(port, 0);
//    RakNet::StartupResult startResult = rakPeer_->Startup(128, &socket, 1);
//    if (startResult == RakNet::RAKNET_STARTED)
//    {
//        URHO3D_LOGINFO("Started server on port " + String(port));
//        rakPeer_->SetMaximumIncomingConnections(128);
//        isServer_ = true;
//        return true;
//    }
//    else
//    {
//        URHO3D_LOGINFO("Failed to start server on port " + String(port) + ", error code: " + String((int)startResult));
//        return false;
//    }
    
    RakNet::SocketDescriptor socketDescriptors[2] = {RakNet::SocketDescriptor(port,0), RakNet::SocketDescriptor(port+1,0)};
//    socketDescriptors[0].port=port;
    socketDescriptors[0].socketFamily=AF_INET6; // Test out IPV6
//    socketDescriptors[1].port=port;
    socketDescriptors[1].socketFamily=AF_INET; // Test out IPV4
    RakNet::StartupResult startResult = rakPeer_->Startup(128, socketDescriptors, 2 );
    if (startResult != RakNet::RAKNET_STARTED)
    {
        URHO3D_LOGINFO("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.");
        // Try again, but leave out IPV6
        startResult = rakPeer_->Startup(128, socketDescriptors, 1 );
    }
    if (startResult != RakNet::RAKNET_STARTED)
    {
        URHO3D_LOGINFO("Failed to start server on port " + String(port) + ", error code: " + String((int)startResult));
        return false;
    }
    else
    {
        URHO3D_LOGINFO("Started server on port " + String(port));
        rakPeer_->SetMaximumIncomingConnections(128);
        isServer_ = true;
        return true;
    }
}

void Network::StopServer()
{
    if (!IsServerRunning())
        return;

    URHO3D_PROFILE(StopServer);

    clientConnections_.Clear();
    // Provide 300 ms to notify
    rakPeer_->Shutdown(300);

    URHO3D_LOGINFO("Stopped server");
}

void Network::BroadcastMessage(int msgID, bool reliable, bool inOrder, const VectorBuffer& msg, unsigned contentID)
{
    BroadcastMessage(msgID, reliable, inOrder, msg.GetData(), msg.GetSize(), contentID);
}

void Network::BroadcastMessage(int msgID, bool reliable, bool inOrder, const unsigned char* data, unsigned numBytes,
    unsigned contentID)
{
    // Make sure not to use kNet internal message ID's
    if (msgID < ID_USER_PACKET_ENUM || msgID >= 255)
    {
        URHO3D_LOGERROR("Can not send message with reserved ID");
        return;
    }

    VectorBuffer msgData;
    msgData.WriteUByte((unsigned char)msgID);
    msgData.Write(data, numBytes);

    if (isServer_)
        rakPeer_->Send((const char*)msgData.GetData(), (int)msgData.GetSize(), HIGH_PRIORITY, RELIABLE, (char)0, RakNet::UNASSIGNED_RAKNET_GUID, true);
    else
        URHO3D_LOGERROR("Server not running, can not broadcast messages");
}

void Network::BroadcastRemoteEvent(StringHash eventType, bool inOrder, const VariantMap& eventData)
{
    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin(); i != clientConnections_.End(); ++i)
        i->second_->SendRemoteEvent(eventType, inOrder, eventData);
}

void Network::BroadcastRemoteEvent(Scene* scene, StringHash eventType, bool inOrder, const VariantMap& eventData)
{
    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
         i != clientConnections_.End(); ++i)
    {
        if (i->second_->GetScene() == scene)
            i->second_->SendRemoteEvent(eventType, inOrder, eventData);
    }
}

void Network::BroadcastRemoteEvent(Node* node, StringHash eventType, bool inOrder, const VariantMap& eventData)
{
    if (!node)
    {
        URHO3D_LOGERROR("Null sender node for remote node event");
        return;
    }
    if (node->GetID() >= FIRST_LOCAL_ID)
    {
        URHO3D_LOGERROR("Sender node has a local ID, can not send remote node event");
        return;
    }

    Scene* scene = node->GetScene();
    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
         i != clientConnections_.End(); ++i)
    {
        if (i->second_->GetScene() == scene)
            i->second_->SendRemoteEvent(node, eventType, inOrder, eventData);
    }
}

void Network::SetUpdateFps(int fps)
{
    updateFps_ = Max(fps, 1);
    updateInterval_ = 1.0f / (float)updateFps_;
    updateAcc_ = 0.0f;
}

void Network::SetSimulatedLatency(int ms)
{
    simulatedLatency_ = Max(ms, 0);
    ConfigureNetworkSimulator();
}

void Network::SetSimulatedPacketLoss(float loss)
{
    simulatedPacketLoss_ = Clamp(loss, 0.0f, 1.0f);
    ConfigureNetworkSimulator();
}

void Network::RegisterRemoteEvent(StringHash eventType)
{
    if (blacklistedRemoteEvents_.Find(eventType) != blacklistedRemoteEvents_.End())
    {
        URHO3D_LOGERROR("Attempted to register blacklisted remote event type " + String(eventType));
        return;
    }

    allowedRemoteEvents_.Insert(eventType);
}

void Network::UnregisterRemoteEvent(StringHash eventType)
{
    allowedRemoteEvents_.Erase(eventType);
}

void Network::UnregisterAllRemoteEvents()
{
    allowedRemoteEvents_.Clear();
}

void Network::SetPackageCacheDir(const String& path)
{
    packageCacheDir_ = AddTrailingSlash(path);
}

void Network::SendPackageToClients(Scene* scene, PackageFile* package)
{
    if (!scene)
    {
        URHO3D_LOGERROR("Null scene specified for SendPackageToClients");
        return;
    }
    if (!package)
    {
        URHO3D_LOGERROR("Null package specified for SendPackageToClients");
        return;
    }

    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
         i != clientConnections_.End(); ++i)
    {
        if (i->second_->GetScene() == scene)
            i->second_->SendPackageToClient(package);
    }
}

SharedPtr<HttpRequest> Network::MakeHttpRequest(const String& url, const String& verb, const Vector<String>& headers,
    const String& postData)
{
    URHO3D_PROFILE(MakeHttpRequest);

    // The initialization of the request will take time, can not know at this point if it has an error or not
    SharedPtr<HttpRequest> request(new HttpRequest(url, verb, headers, postData));
    return request;
}

Connection* Network::GetConnection(const RakNet::AddressOrGUID& connection) const
{
    if (serverConnection_ && serverConnection_->GetAddressOrGUID() == connection)
        return serverConnection_;
    else
    {
        HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::ConstIterator i = clientConnections_.Find(connection);
        if (i != clientConnections_.End())
            return i->second_;
        else
            return 0;
    }
}

Connection* Network::GetServerConnection() const
{
    return serverConnection_;
}

Vector<SharedPtr<Connection> > Network::GetClientConnections() const
{
    Vector<SharedPtr<Connection> > ret;
    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::ConstIterator i = clientConnections_.Begin();
         i != clientConnections_.End(); ++i)
        ret.Push(i->second_);

    return ret;
}

bool Network::IsServerRunning() const
{
    return rakPeer_->IsActive() && isServer_;
}

bool Network::CheckRemoteEvent(StringHash eventType) const
{
    return allowedRemoteEvents_.Contains(eventType);
}

void Network::Update(float timeStep)
{
    URHO3D_PROFILE(UpdateNetwork);

    // Process the network server if started
    if (rakPeer_->IsActive())
    {
        while (RakNet::Packet* packet = rakPeer_->Receive())
        {
            unsigned char packetID = packet->data[0];
            bool packetHandled = false;

            // Deal with timestamped backents
            unsigned dataStart = sizeof(char);
            if (packetID == ID_TIMESTAMP)
            {
                dataStart += sizeof(RakNet::Time);
                packetID = packet->data[dataStart];
                dataStart += sizeof(char);
            }

        // Connectivity messages
            if (packetID == ID_NEW_INCOMING_CONNECTION) // We're a server/peer, we've got a new connection
            {
                NewConnectionEstablished(packet->systemAddress);
                packetHandled = true;
            }
            else if (packetID == ID_CONNECTION_REQUEST_ACCEPTED) // We're a client, our connection as been accepted
            {
                OnServerConnected(packet->systemAddress);
                packetHandled = true;
            }
            else if (packetID == ID_CONNECTION_LOST) // We've lost connectivity with the packet source
            {
                if (isServer_)
                    ClientDisconnected(packet->systemAddress);
                else
                    OnServerDisconnected();
                packetHandled = true;
            }
            else if (packetID == ID_DISCONNECTION_NOTIFICATION) // We've lost connection with the other side
            {
                if (isServer_) // If a server then we've lost the client
                    ClientDisconnected(packet->systemAddress);
                else // If a client then we've lost the server
                    OnServerDisconnected();
                packetHandled = true;
            }
            else if (packetID == ID_CONNECTION_ATTEMPT_FAILED) // We've failed to connect to the server/peer
            {
                SendEvent(E_CONNECTFAILED);
                packetHandled = true;
            }

        // Denial of connection
            else if (packetID == ID_CONNECTION_BANNED) // We're a client and we're on the ban list
            {
                if (!isServer_)
                    SendEvent(E_NETWORKBANNED);
                packetHandled = true;
            }
            else if (packetID == ID_INVALID_PASSWORD) // We're a client, and we gave an invalid password
            {
                if (!isServer_)
                    SendEvent(E_NETWORKINVALIDPASSWORD);
                packetHandled = true;
            }
        // RakNet special messages
            else if (packetID == ID_DOWNLOAD_PROGRESS) // Part of a file transfer
            {
                
            }
            else if (packetID == ID_UNCONNECTED_PONG) // Host discovery response
            {
                // Timestamp included, need to eat it from the data size
                dataStart += sizeof(RakNet::TimeMS);

                VectorBuffer buffer(packet->data + dataStart, packet->length - dataStart);
                VariantMap srcData = buffer.ReadVariantMap();

                VariantMap& eventMap = context_->GetEventDataMap();
                using namespace NetworkHostDiscovered;
                eventMap[P_BEACON] = srcData;
                eventMap[P_ADDRESS] = String(packet->systemAddress.ToString(false));
                eventMap[P_PORT] = (int)packet->systemAddress.GetPort();
                SendEvent(E_NETWORKHOSTDISCOVERED, eventMap);
                packetHandled = true;
            }

        // Urho3D messages
            if (packetID >= ID_USER_PACKET_ENUM)
            {
                if (!isServer_)
                {
                    MemoryBuffer buffer(packet->data + dataStart, packet->length - dataStart);
                    serverConnection_->ProcessMessage(packetID, buffer);
                }
                else
                    HandleMessage(packet->systemAddress, 0, packetID, (const char*)(packet->data + dataStart), packet->length - dataStart);
                packetHandled = true;
            }

        // Log an error if we encounter a packet that we do not know how to handle
            if (!packetHandled && packetID < sizeof(RAKNET_MESSAGEID_STRINGS))
                URHO3D_LOGERROR("Unhandled network packet: " + String(RAKNET_MESSAGEID_STRINGS[packetID]));
            else if (!packetHandled)
                URHO3D_LOGERRORF("Unhandled network packet: %i", packetID);


            rakPeer_->DeallocatePacket(packet);
        }
    }
}

void Network::PostUpdate(float timeStep)
{
    URHO3D_PROFILE(PostUpdateNetwork);

    // Check if periodic update should happen now
    updateAcc_ += timeStep;
    bool updateNow = updateAcc_ >= updateInterval_;

    if (updateNow)
    {
        // Notify of the impending update to allow for example updated client controls to be set
        SendEvent(E_NETWORKUPDATE);
        updateAcc_ = fmodf(updateAcc_, updateInterval_);

        if (IsServerRunning())
        {
            // Collect and prepare all networked scenes
            {
                URHO3D_PROFILE(PrepareServerUpdate);

                networkScenes_.Clear();
                for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
                     i != clientConnections_.End(); ++i)
                {
                    Scene* scene = i->second_->GetScene();
                    if (scene)
                        networkScenes_.Insert(scene);
                }

                for (HashSet<Scene*>::ConstIterator i = networkScenes_.Begin(); i != networkScenes_.End(); ++i)
                    (*i)->PrepareNetworkUpdate();
            }

            {
                URHO3D_PROFILE(SendServerUpdate);

                // Then send server updates for each client connection
                for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
                     i != clientConnections_.End(); ++i)
                {
                    i->second_->SendServerUpdate();
                    i->second_->SendRemoteEvents();
                    i->second_->SendPackages();
                }
            }
        }

        if (serverConnection_)
        {
            // Send the client update
            serverConnection_->SendClientUpdate();
            serverConnection_->SendRemoteEvents();
        }

        // Notify that the update was sent
        SendEvent(E_NETWORKUPDATESENT);
    }
}

void Network::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
{
    using namespace BeginFrame;

    Update(eventData[P_TIMESTEP].GetFloat());
}

void Network::HandleRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace RenderUpdate;

    PostUpdate(eventData[P_TIMESTEP].GetFloat());
}

void Network::OnServerConnected(const RakNet::AddressOrGUID& address)
{
    serverConnection_->SetConnectPending(false);
    serverConnection_->SetAddressOrGUID(address);
    URHO3D_LOGINFO("Connected to server");

    // Send the identity map now
    VectorBuffer msg;
    msg.WriteVariantMap(serverConnection_->GetIdentity());
    serverConnection_->SendMessage(MSG_IDENTITY, true, true, msg);

    SendEvent(E_SERVERCONNECTED);
}

void Network::OnServerDisconnected()
{
    // Differentiate between failed connection, and disconnection
    bool failedConnect = serverConnection_ && serverConnection_->IsConnectPending();
    serverConnection_.Reset();

    if (!failedConnect)
    {
        URHO3D_LOGINFO("Disconnected from server");
        SendEvent(E_SERVERDISCONNECTED);
    }
    else
    {
        URHO3D_LOGERROR("Failed to connect to server");
        SendEvent(E_CONNECTFAILED);
    }
}

void Network::ConfigureNetworkSimulator()
{
    if (serverConnection_)
        serverConnection_->ConfigureNetworkSimulator(simulatedLatency_, simulatedPacketLoss_);

    for (HashMap<RakNet::AddressOrGUID, SharedPtr<Connection> >::Iterator i = clientConnections_.Begin();
         i != clientConnections_.End(); ++i)
        i->second_->ConfigureNetworkSimulator(simulatedLatency_, simulatedPacketLoss_);
}

void RegisterNetworkLibrary(Context* context)
{
    NetworkPriority::RegisterObject(context);
}

}
