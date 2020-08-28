// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UdpNetworkingInterface.generated.h"


UINTERFACE(MinimalAPI)
class UUdpNetworkingInterface : public UInterface
{
    GENERATED_BODY()
};


class SHOOTER_API IUdpNetworkingInterface
{
    GENERATED_BODY()

public:
    virtual bool SendMessage(const FString& Message, const FInternetAddr& Destination)
    {
        check(0 && "You must override this");
        return false;
    }

    virtual FString WaitForRead(FInternetAddr *Source, const FTimespan& WaitTime)
    {
        check(0 && "You must override this");
        return FString();
    } 
};
