// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "UdpNetworking.h"
#include "GameServicesInterface.h"


class SHOOTER_API FGameServices : public IGameServicesInterface
{
    EGameServicesStatus Status = EGameServicesStatus::Undefined;

    TSharedPtr<FUdpNetworking> UdpNetworking;

    FString& Address;

    int32& Port;

    bool InitUdpNetworking();

    bool Ping();

public:
    FGameServices(FString AddressServices, int32 PortServices);

    inline bool operator==(const EGameServicesStatus& Other) const { return Status == Other; }

    inline operator EGameServicesStatus() const { return Status; }

    virtual bool FindServers(TArray<FServerInstance>* Servers) override;
};
