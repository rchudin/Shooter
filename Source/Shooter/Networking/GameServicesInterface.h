// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameServicesInterface.generated.h"


USTRUCT()
struct FServerInstance
{
    GENERATED_BODY()

    FString Name;
};

UENUM(BlueprintType)
enum class EGameServicesStatus : uint8
{
    Undefined UMETA(DisplayName = "Undefined"),
    Success UMETA(DisplayName = "Success"),
    Error UMETA(DisplayName = "Error"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameServicesInterface : public UInterface
{
    GENERATED_BODY()
};


class SHOOTER_API IGameServicesInterface
{
    GENERATED_BODY()

public:
    virtual bool FindServers(TArray<FServerInstance>* Servers)
    {
        check(0 && "You must override this");
        return false;
    };
};
