#include "LandingControllerActor.h"
#include "PlayerShipPawn.h"

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
	
	if (!LandingPadActor || !LandingCameraActor)
	{
		if (Destroy())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s destroyed because missing land pad or landing camera"), *GetName());
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

	StateIndex = 0;
	DefaultCameraActor = nullptr;
}

// Called every frame
void ALandingControllerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (StateIndex)
	{
		case 1:
		{
			AActor* DefaultViewTarget = PlayerController->GetViewTarget();
			
			if (!DefaultCameraActor) DefaultCameraActor = DefaultViewTarget;

			if (DefaultViewTarget != LandingCameraActor)
			{
				PlayerController->SetViewTarget(LandingCameraActor);
			}

			FVector LandingLocation = LandingPadActor->GetActorLocation();
			LandingLocation.Z += 60.f;
			PlayerShipPawn->ResetMovement();
			PlayerShipPawn->SetActorLocation(LandingLocation);
		} break;
		
		case 2:
		{
			AActor* ActiveViewTarget = PlayerController->GetViewTarget();

			if (ActiveViewTarget != DefaultCameraActor)
			{
				PlayerController->SetViewTarget(DefaultCameraActor);
				DefaultCameraActor = nullptr;
			}
		} break;

		default:
		{
			// Do nothing
		}
	}

}

void ALandingControllerActor::SetLandingState(int stateIndex)
{
	StateIndex = stateIndex;
}

void ALandingControllerActor::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	AActor* PlayerActor = Cast<AActor>(PlayerShipPawn);
	if (OtherActor && (OtherActor != this) && OtherActor == PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has entered the landing trigger"), *OtherActor->GetName());
		StateIndex = 1;
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
