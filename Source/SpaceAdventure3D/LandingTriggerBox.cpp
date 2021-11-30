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

	if (!SpecificActor)
	{
		SpecificActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void ALandingTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has entered the landing trigger"), *OtherActor->GetName());
	}
}

void ALandingTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has lef the landing trigger"), *OtherActor->GetName());
	}
}

