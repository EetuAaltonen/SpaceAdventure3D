#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShipPawn.generated.h"

UCLASS(Config = Game)
class SPACEADVENTURE3D_API APlayerShipPawn : public APawn
{
	GENERATED_BODY()

	// StaticMesh
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	// Spring arm to set camera offset
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	// Camera component
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	
	// Sets default values for this pawn's properties
	APlayerShipPawn();

	// AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Bond axis
	void MoveUpInput(float Val);

	void MoveRightInput(float Val);

	void ThrustInput(float Val);

	void RollInput(float Val);

	void BoostInput(float Val);

private:

	UPROPERTY(Category = Plane, EditAnywhere)
	float Acceleration;

	UPROPERTY(Category = Plane, EditAnywhere)
	float RollSpeed;

	UPROPERTY(Category = Plane, EditAnywhere)
	float TurnSpeed;

	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxForwardSpeed;

	UPROPERTY(Category = Yaw, EditAnywhere)
	float MaxReverseSpeed;

	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxBoostSpeed;

	float CurrentForwardSpeed;

	float CurrentYawSpeed;

	float CurrentPitchSpeed;

	float CurrentRollSpeed;

public:

	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }

	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	void ResetMovement();
};
