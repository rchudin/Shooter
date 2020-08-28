// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Shooter/Core/UdpNetworkingInterface.h"


class SHOOTER_API FUdpNetworking : public IUdpNetworkingInterface
{
    FSocket* Socket;

    int32 MaxBufferSize = 1500;

    void InitSocket();

    void CloseSocket();
    
public:
    FUdpNetworking();
    
    ~FUdpNetworking();

    virtual bool SendMessage(const FString& Message, const FInternetAddr& Destination) override;
};
