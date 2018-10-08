// Copyright (C) Dominik Rosser 2018

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Interfaces/IHaveCheckpoint.h"
#include "PirateGameCharacterInput.h"
#include "PirateGameCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PIRATEGAME_API APirateGameCharacter : public APaperCharacter, public IHaveCheckpoint
{
	GENERATED_BODY()
	
public:
	/** APirateGameCharacter construction */
	APirateGameCharacter();

// AActor overrides
protected:
	/** Called when the actor enters the world */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind input functionality */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

// Input Bindings
private:
	/** Axis input mapping for x-axis movement */
	void MoveX(const float AxisValue);

	/** Action mapping, gets called when the jump command is pressed */
	void OnPressedJump();

	/** Action mapping, gets called when the jump command is released */
	void OnReleasedJump();

	/** Action mapping, gets called when the pause command is pressed */
	void OnPressedPause();

// Functions
protected:
	/** Handles unhandled movement input */
	void UpdateMovement(const float DeltaTime);

	/** Update rotation left or right based on the player movement. 
		Sets the rotation so that the character faces his direction of travel. */
	void UpdateRotation();

	/** Updates the animation based on the characters state */
	void UpdateAnimation();

// Player properties
protected:
	/** Defines how fast the player can run around */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character properties")
	float MovementSpeed;

	// Maybe used later
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character properties")
	// float WalljumpRange;

protected:
	/** Input class used to validate axis-mapped inputs */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Input")
	FPirateGameCharacterInput CharacterInput;

// Side-view camera components
protected:
	/** The player's camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Camera", meta = (AllowPrivateAccess = true))
	class UCameraComponent* SideViewCameraComponent;

	/** The spring arm for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Camera", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

// Public access to the camera components
public:
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

// Paper flipbook animations
protected:
	/** The animation to play while standing still */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	/** The animation to play while running around */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* RunningAnimation;
};
