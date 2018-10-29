// Copyright (C) Dominik Rosser 2018

#include "CheckpointBase.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "PirateGameGameMode_Ingame.h"
#include "Interfaces/IHaveCheckpoint.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(CheckpointBaseLog);

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

	// By default, a checkpoint will not end the level
	bIsEndOfLevel = false;
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
	IHaveCheckpoint *const actorWithCheckpoint = Cast<IHaveCheckpoint>(OtherActor);

	if (actorWithCheckpoint)
	{
		const FVector location = this->GetActorLocation();
		// Only update x and z, because we don't wan't the player to move in the y-axis
		actorWithCheckpoint->SetCheckpoint(FVector (location.X, actorWithCheckpoint->GetCheckpoint ().Y, location.Z));
		
		const FVector log_set_location = actorWithCheckpoint->GetCheckpoint();
		UE_LOG(CheckpointBaseLog, Log, TEXT("Set new Checkpoint for Actor: (%f, %f, %f)"), log_set_location.X, log_set_location.Y, log_set_location.Z);

		if (bIsEndOfLevel)
		{
			APirateGameGameMode_Ingame *const gamemode = Cast<APirateGameGameMode_Ingame>(GetWorld()->GetAuthGameMode());
			if (gamemode != nullptr)
			{
				gamemode->ShowGameWonMenuWidget();
				UGameplayStatics::SetGamePaused(GetWorld(), true);
			}
		}
	}
}

