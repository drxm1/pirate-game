// Copyright (C) Dominik Rosser 2018

#include "CheckpointBase.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
// #include "PirateGameCharacter.h"
#include "Interfaces/IHaveCheckpoint.h"

// Sets default values
ACheckpointBase::ACheckpointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckpointRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CheckpointRoot"));
	RootComponent = CheckpointRoot;

	CheckpointSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CheckpointSprite"));
	CheckpointSpriteComponent->SetupAttachment(CheckpointRoot);

	CheckpointTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointTrigger"));
	CheckpointTrigger->SetWorldScale3D(FVector(1.0f, 10.0f, 2.0f));
	CheckpointTrigger->bGenerateOverlapEvents = true;
	CheckpointTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointBase::OnRadiusEnter);
	CheckpointTrigger->SetupAttachment(CheckpointRoot);
}

// Called when the game starts or when spawned
void ACheckpointBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpointBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpointBase::OnRadiusEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const IHaveCheckpoint * actorWithCheckpoint = Cast<IHaveCheckpoint>(OtherActor);

	if (actorWithCheckpoint)
	{
		OtherActor->Destroy();
	}
}

