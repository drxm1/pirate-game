// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class PIRATEGAME_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* PickupRoot;

	UPROPERTY(EditAnywhere)
	class UShapeComponent* PickupTrigger;

	UPROPERTY(EditAnywhere)
	class UPaperSpriteComponent* PickupSpriteComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpecificOverlapActorClass;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnRadiusEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
