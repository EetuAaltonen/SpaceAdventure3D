

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
