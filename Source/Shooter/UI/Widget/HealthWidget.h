// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HealthWidget.generated.h"


UCLASS()
class SHOOTER_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
	UHealthWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateHealth(const float &Count, const float &Percent) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
        class UTextBlock *TXTHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar *PBHealth;

};
