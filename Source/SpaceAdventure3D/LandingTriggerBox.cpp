#include "LandingTriggerBox.h"
#include "EngineUtils.h"

ALandingTriggerBox::ALandingTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ALandingTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ALandingTriggerBox::OnOverlapEnd);
}

void ALandingTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	if (!LandingControllerActor)
	{
		if (Destroy())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s destroyed because missing landing controller"), *GetName());
		}
	}
}

void ALandingTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	LandingControllerActor->OnOverlapBegin(OverlappedActor, OtherActor);
}

void ALandingTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	LandingControllerActor->OnOverlapEnd(OverlappedActor, OtherActor);
}

