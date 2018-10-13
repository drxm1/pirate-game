// Copyright (C) Dominik Rosser 2018

#include "RelocateAreaBase.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Interfaces/IHaveCheckpoint.h"
#include "Interfaces/ICanDie.h"

DEFINE_LOG_CATEGORY(RelocateAreaBaseLog);

// Sets default values
ARelocateAreaBase::ARelocateAreaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RelocateAreaRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RelocateAreaRoot"));
	RootComponent = RelocateAreaRoot;

	RelocateTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RelocateTrigger"));
	RelocateTrigger->SetWorldScale3D(FVector(4.0f, 10.0f, 2.0f));
	RelocateTrigger->bGenerateOverlapEvents = true;
	RelocateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARelocateAreaBase::OnRadiusEnter);
	RelocateTrigger->SetupAttachment(RelocateAreaRoot);
}

// Called when the game starts or when spawned
void ARelocateAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARelocateAreaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARelocateAreaBase::OnRadiusEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHaveCheckpoint *const actorWithCheckpoint = Cast<IHaveCheckpoint>(OtherActor);
	ICanDie *const actorWithHealth = Cast<ICanDie>(OtherActor);
	
	UE_LOG(RelocateAreaBaseLog, Log, TEXT("Actor entered RelocateArea"));

	if (actorWithCheckpoint)
	{
		// Remove a life from the actor if the actor has health
		if (actorWithHealth)
		{
			actorWithHealth->LoseHealth(1);
		}
		// Relocate the actor to the checkpoint if the actor didn't die
		if (actorWithHealth->IsAlive())
		{
			OtherActor->SetActorLocation(actorWithCheckpoint->GetCheckpoint(), false, nullptr, ETeleportType::None);
		}
	}
}

