#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "LandingControllerActor.h"

#include "LandingTriggerBox.generated.h"

/**
 * 
 */

UCLASS()
class SPACEADVENTURE3D_API ALandingTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	ALandingTriggerBox();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	class ALandingControllerActor* LandingControllerActor;
};
