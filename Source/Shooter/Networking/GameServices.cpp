// Copyright © 2020 ruslanchudin.com


#include "GameServices.h"
#include "SocketSubsystem.h"


FGameServices::FGameServices(FString AddressServices, const int32& PortServices): Address(MoveTemp(AddressServices)),
                                                                                  Port(PortServices)
{
    InitUdpNetworking();
    Ping();
}

bool FGameServices::InitUdpNetworking()
{
    if (!UdpNetworking)
    {
        auto RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
        bool bRemoteAddressIsValid;
        RemoteAddress->SetIp(*Address, bRemoteAddressIsValid);
        RemoteAddress->SetPort(Port);
        if (bRemoteAddressIsValid)
        {
            UdpNetworking = MakeShareable(new FUdpNetworking);
            Status = EGameServicesStatus::Success;
            return true;
        }
        Status = EGameServicesStatus::Error;
        return false; 
    }
    return true; 
}

bool FGameServices::Ping()
{
    //TMP...
    if (!UdpNetworking) return false;
    auto RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool bRemoteAddressIsValid;
    RemoteAddress->SetIp(*Address, bRemoteAddressIsValid);
    RemoteAddress->SetPort(Port);
    const auto Ok = UdpNetworking->SendMessage(FString("Hello world!"), *RemoteAddress);
    if (Ok)
    {
        const FTimespan WaitTime = FTimespan::FromSeconds(5);
        const FString ReadMsg = UdpNetworking->WaitForRead(nullptr, WaitTime);
        if (ReadMsg.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("Error read msg GameServices!!!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Msg from server: %s"), *ReadMsg);
        }
    }
    return false;
    //TMP
}

bool FGameServices::FindServers(TArray<FServerInstance>* Servers)
{
    return false;
}
