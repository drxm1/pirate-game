// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointBase.generated.h"

UCLASS()
class PIRATEGAME_API ACheckpointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* CheckpointRoot;

	UPROPERTY(EditAnywhere)
	class UShapeComponent* CheckpointTrigger;

	UPROPERTY(EditAnywhere)
	class UPaperSpriteComponent* CheckpointSpriteComponent;

	// Not needed:
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<AActor> SpecificOverlapActorClass;

public:
	UFUNCTION()
	void OnRadiusEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
