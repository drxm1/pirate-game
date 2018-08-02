// Copyright (C) Dominik Rosser 2018

#include "PickupBase.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "PirateGameCharacter.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	PickupSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PickupSprite"));
	PickupSpriteComponent->SetupAttachment(PickupRoot);

	PickupTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetWorldScale3D(FVector(1.0f, 10.0f, 1.0f));
	PickupTrigger->bGenerateOverlapEvents = true;
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnRadiusEnter);
	PickupTrigger->SetupAttachment(PickupRoot);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
