// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"


#define  LOG_INSTANCE(CategoryName, Verbosity, IsServer, Format, ...)\
{\
UE_LOG(CategoryName, Verbosity, TEXT("(%s) " Format), IsServer?TEXT("Server"):TEXT("Client"), ##__VA_ARGS__);\
}


UCLASS()
class SHOOTER_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    template <typename T>
    static FORCEINLINE FString GetEnumValueAsString(const FString& Name, T Value)
    {
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
        if (!EnumPtr) return FString("Invalid");
        return EnumPtr->GetNameByValue(static_cast<int64>(Value)).ToString();
    }
};
