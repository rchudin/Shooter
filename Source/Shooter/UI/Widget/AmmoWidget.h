// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "AmmoWidget.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTER_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UAmmoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	void UpdateCurrentAmmo(const int& Count) const;
	
	void UpdateTotalAmmo(const int& Count) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTCurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
        class UTextBlock* TXTTotalAmmo;
};
