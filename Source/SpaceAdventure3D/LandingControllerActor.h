#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandingControllerActor.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	class APlayerShipPawn* PlayerShipPawn;

	UPROPERTY(EditAnywhere)
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	class AActor* LandingPadActor;

	UPROPERTY(EditAnywhere)
	class AActor* LandingCameraActor;

	UPROPERTY(EditAnywhere)
	class AActor* DefaultCameraActor;

private:

	int StateIndex;
};
