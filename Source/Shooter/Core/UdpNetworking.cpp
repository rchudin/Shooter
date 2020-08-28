// Copyright © 2020 ruslanchudin.com


#include "UdpNetworking.h"
#include "Networking/Public/Common/UdpSocketBuilder.h"


#define UDP_NETWORKING_SOCKET_DESCRIPTION "ue4-dgram-send"


FUdpNetworking::FUdpNetworking()
{
    InitSocket();
}

FUdpNetworking::~FUdpNetworking()
{
    CloseSocket();
}

void FUdpNetworking::InitSocket()
{
    Socket = FUdpSocketBuilder(UDP_NETWORKING_SOCKET_DESCRIPTION).AsReusable().WithBroadcast();
    Socket->SetSendBufferSize(MaxBufferSize, MaxBufferSize);
    Socket->SetReceiveBufferSize(MaxBufferSize, MaxBufferSize);
}

void FUdpNetworking::CloseSocket()
{
    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
    }
}

bool FUdpNetworking::SendMessage(const FString& Message, const FInternetAddr& Destination)
{
    if (!Socket) return false;
    const FTCHARToUTF8 Data(*Message);
    int32 BytesSent = 0;
    const bool Success = Socket->SendTo((uint8*)Data.Get(), Data.Length(), BytesSent, Destination);
    return Success && BytesSent > 0;
}
