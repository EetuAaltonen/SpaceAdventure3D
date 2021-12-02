#include "PlayerShipPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

// Sets default values
APlayerShipPawn::APlayerShipPawn()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f; // Camera offset distance
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Set movement parameters
	Acceleration = 500.f;
	TurnSpeed = 100.f;
	RollSpeed = 50.f;
	MaxForwardSpeed = 4000.f;
	MaxReverseSpeed = -1000.f;

	// Set controls parameters
	IsCinematicControlled = false;

	// Reset movement
	ResetMovement();

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerShipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerShipPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	if (!IsCinematicControlled)
	{
		const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

		// Move plan forwards (with sweep so we stop when we collide with things)
		AddActorLocalOffset(LocalMove, true);

		// Calculate change in rotation this frame
		FRotator DeltaRotation(0, 0, 0);
		DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
		DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
		DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

		// Rotate plane
		AddActorLocalRotation(DeltaRotation);
	}
}

void APlayerShipPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp,
								bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void APlayerShipPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind control axis' to callback functions
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerShipPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerShipPawn::MoveRightInput);
	PlayerInputComponent->BindAxis("Thrust", this, &APlayerShipPawn::ThrustInput);
	PlayerInputComponent->BindAxis("Roll", this, &APlayerShipPawn::RollInput);
	PlayerInputComponent->BindAxis("Boost", this, &APlayerShipPawn::ThrustInput);
}

void APlayerShipPawn::MoveUpInput(float Val)
{
	if (IsCinematicControlled) return;

	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void APlayerShipPawn::MoveRightInput(float Val)
{
	if (IsCinematicControlled) return;

	float TargetYawSpeed = (Val * TurnSpeed);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void APlayerShipPawn::ThrustInput(float Val)
{
	if (IsCinematicControlled) return;

	float CurrentAcc = Val * Acceleration;
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MaxReverseSpeed , MaxForwardSpeed);
}

void APlayerShipPawn::RollInput(float Val)
{
	if (IsCinematicControlled) return;

	float TargetRollSpeed = Val * RollSpeed;

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void APlayerShipPawn::BoostInput(float Val)
{
	if (IsCinematicControlled) return;

	/*bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);*/
}

void APlayerShipPawn::ResetMovement()
{
	CurrentForwardSpeed = 0.f;
	CurrentYawSpeed = 0.f;
	CurrentPitchSpeed = 0.f;
	CurrentRollSpeed = 0.f;
}

bool APlayerShipPawn::RequestSetCinematicControl()
{
	IsCinematicControlled = true;
	return true;
}

bool APlayerShipPawn::RequestReleaseCinematicControl()
{
	IsCinematicControlled = false;
	return true;
}
