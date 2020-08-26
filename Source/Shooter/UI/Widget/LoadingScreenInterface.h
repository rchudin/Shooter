#pragma once


#include "UObject/Interface.h"
#include "LoadingScreenInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class ULoadingScreenInterface : public UInterface
{
    GENERATED_BODY()
};

class ILoadingScreenInterface
{
    GENERATED_BODY()
public:
    virtual void AddLoadingScreenToViewport(){ check(0 && "You must override this"); }

    virtual void RemoveLoadingScreenFromViewport(){ check(0 && "You must override this"); }
};
