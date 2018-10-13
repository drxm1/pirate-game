// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RelocateAreaBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(RelocateAreaBaseLog, Log, All);

UCLASS()
class PIRATEGAME_API ARelocateAreaBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARelocateAreaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* RelocateAreaRoot;

	UPROPERTY(EditAnywhere)
	class UShapeComponent* RelocateTrigger;

public:
	UFUNCTION()
	void OnRadiusEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
