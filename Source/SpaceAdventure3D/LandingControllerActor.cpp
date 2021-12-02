#include "LandingControllerActor.h"
#include "PlayerShipPawn.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALandingControllerActor::ALandingControllerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALandingControllerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!LandingTriggerActor || !LandingPadActor || !LandingCameraActor)
	{
		if (Destroy())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s destroyed because missing landing trigger or land pad or landing camera"), *GetName());
		}
	}

	if (!PlayerShipPawn)
	{
		PlayerShipPawn = Cast<APlayerShipPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	if (!PlayerController)
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
	}

	// Timeline
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);
	}

	// Landing
	LandingStateIndex = 0;
	DefaultCameraActor = nullptr;

	LandingStartLocation = LandingTriggerActor->GetActorLocation();

	LandingTargetLocation = LandingPadActor->GetActorLocation();
	LandingTargetLocation.Z += 50.f;

	LandingAlpha = 0.f;
}

// Called every frame
void ALandingControllerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

	switch (LandingStateIndex)
	{
		case 1:
		{
			if (PlayerShipPawn->RequestSetCinematicControl())
			{
				AActor* DefaultViewTarget = PlayerController->GetViewTarget();

				if (!DefaultCameraActor) DefaultCameraActor = DefaultViewTarget;

				if (DefaultViewTarget != LandingCameraActor)
				{
					PlayerController->SetViewTarget(LandingCameraActor);
				}
				PlayerShipPawn->ResetMovement();

				CurveTimeline.PlayFromStart();
				LandingStateIndex = 2;
			}
		} break;
		case 2:
		{
			// TODO: Move and rotate player towards landing spot
			FVector PlayerPosition = PlayerShipPawn->GetActorLocation();
			float DistanceToTarget = (LandingTargetLocation - PlayerPosition).Size();

			if (DistanceToTarget > 0.5f)
			{
				FVector NewPosition = UKismetMathLibrary::VLerp(LandingStartLocation, LandingTargetLocation, LandingAlpha);
				FVector Direction = (NewPosition + LandingTargetLocation);
				Direction.Normalize(1.0);
				PlayerShipPawn->SetActorLocation(NewPosition, false, nullptr, ETeleportType::ResetPhysics);
				PlayerShipPawn->SetActorRotation(LandingPadActor->GetActorRotation(), ETeleportType::ResetPhysics);
			}
		} break;
		case 3:
		{
			// TODO: Perform last landing step and rotation
		} break;
		case 4:
		{
			// TODO: Wait until launch request
		} break;
		case 5:
		{
			// TODO: Perform launch
		} break;
		case 6:
		{
			// TODO: Move and rotate player towards exit
		} break;
		case 7:
		{
			if (PlayerShipPawn->RequestReleaseCinematicControl())
			{
				AActor* ActiveViewTarget = PlayerController->GetViewTarget();

				if (ActiveViewTarget != DefaultCameraActor)
				{
					PlayerController->SetViewTarget(DefaultCameraActor);
					DefaultCameraActor = nullptr;
				}
			}
		} break;
		default:
		{
			// Idle state
		}
	}

}

void ALandingControllerActor::SetLandingState(int landingStateIndex)
{
	LandingStateIndex = landingStateIndex;
}

void ALandingControllerActor::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	AActor* PlayerActor = Cast<AActor>(PlayerShipPawn);
	if (OtherActor && (OtherActor != this) && OtherActor == PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has entered the landing trigger"), *OtherActor->GetName());
		LandingStateIndex = 1;
	}
}

void ALandingControllerActor::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	AActor* PlayerActor = Cast<AActor>(PlayerShipPawn);
	if (OtherActor && (OtherActor != this) && OtherActor == PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has lef the landing trigger"), *OtherActor->GetName());
	}
}

void ALandingControllerActor::TimelineProgress(float Value)
{
	LandingAlpha = UKismetMathLibrary::FMin(Value, 1.f);
}
