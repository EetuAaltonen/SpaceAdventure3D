#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "LandingControllerActor.generated.h"

class UCurveFloat;

UCLASS()
class SPACEADVENTURE3D_API ALandingControllerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandingControllerActor();

	void SetLandingState(int stateIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void TimelineProgress(float Value);

	FORCEINLINE int GetLandingState() const { return LandingStateIndex; }

	UPROPERTY(EditAnywhere)
	class APlayerShipPawn* PlayerShipPawn;

	UPROPERTY(EditAnywhere)
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	class AActor* LandingTriggerActor;

	UPROPERTY(EditAnywhere)
	class AActor* LandingPadActor;

	UPROPERTY(EditAnywhere)
	class AActor* LandingCameraActor;

	UPROPERTY(EditAnywhere)
	class AActor* DefaultCameraActor;

private:

	int LandingStateIndex;
	FVector LandingStartLocation;
	FVector LandingTargetLocation;

	float LandingAlpha;
};
