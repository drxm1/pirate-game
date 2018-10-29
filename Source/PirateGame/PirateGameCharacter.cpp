// Copyright (C) Dominik Rosser 2018

#include "PirateGameCharacter.h"						// Header file

/* Engine files */
#include "GameFramework/SpringArmComponent.h"			// USpringArmComponent
#include "Camera/CameraComponent.h"						// UCameraComponent
#include "GameFramework/CharacterMovementComponent.h"	// UCharacterMovementComponent
#include "Components/CapsuleComponent.h"				// UCapsuleComponent
#include "PaperFlipbookComponent.h"						// UPaperFlipbookComponent
#include "Kismet/GameplayStatics.h"						// UGameplayStatics

/* Project files */
#include "PirateGameGameModeBase.h"						// APirateGameGameModeBase
#include "PirateGamePlayerState_Ingame.h"				// APirateGamePlayerState_Ingame
#include "PirateGameGameMode_Ingame.h"					// APirateGameGameMode_Ingame

/**
 * Constructs a APirateGameCharacter and initializes its values.
 */
APirateGameCharacter::APirateGameCharacter()
	// Initial Character Properties
	: MovementSpeed (100.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up the SpringArmComponent
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);

	// Change the camera feel with the spring arm
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 0.8f;

	// Set up the camera
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamraComponent"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;

	// Change the game size and ratio
	SideViewCameraComponent->OrthoWidth = 1980.0f;
	SideViewCameraComponent->AspectRatio = 9.0f / 16.0f;

	// Only use yaw from the controller and ignore the rest of the rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set up the character movement
	JumpMaxCount = 1;
	JumpMaxHoldTime = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.8f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Set up collision
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(32.0f);

	// Set some initial state data
	this->RemainingAirControlBlockingTime = 0.0f;
}

/** 
 * Called when the actor enters the world.
 */
void APirateGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Init first checkpoint location
	CurrentCheckpoint = GetActorLocation();
}

/** 
 * Called every frame.
 */
void APirateGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
	UpdateAirControlBlockingTime(DeltaTime);
	UpdateRotation();
	UpdateAnimation();
}

/** 
 * Called to bind input functionality.
 */
void APirateGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Axis Bindings
	InputComponent->BindAxis("MoveX", this, &APirateGameCharacter::MoveX);
	// Action Bindings
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APirateGameCharacter::OnPressedJump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APirateGameCharacter::OnReleasedJump);
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &APirateGameCharacter::OnPressedPause).bExecuteWhenPaused = true;
}

/** 
 * Axis input mapping for x-axis movement.
 */
void APirateGameCharacter::MoveX(const float AxisValue)
{
	CharacterInput.MoveX(AxisValue);
}

/** 
 * Action mapping, gets called when the jump command is pressed.
 */
void APirateGameCharacter::OnPressedJump()
{
	// When the player is on the floor, a normal jump can be performed
	if (!GetCharacterMovement()->IsFalling())
	{
		Jump();
		// JumpFX();
	}
	else
	{
		PerformWalljumpIfPossible();
	}
}

/** 
 * Action mapping, gets called when the jump command is released.
 */
void APirateGameCharacter::OnReleasedJump()
{
	StopJumping();
}

/** 
 * Action mapping, gets called when the pause command is pressed.
 */
void APirateGameCharacter::OnPressedPause()
{
	if (!IsAlive())
		return;

	APirateGameGameModeBase* gamemode = Cast<APirateGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode != nullptr)
	{
		const bool bIsGamePaused = UGameplayStatics::IsGamePaused(GetWorld());//gamemode->IsGamePaused();
		UGameplayStatics::SetGamePaused(GetWorld(), !bIsGamePaused);//gamemode->SetGamePaused(!bIsGamePaused);
		gamemode->ShowPauseMenuWidget(!bIsGamePaused);
	}
}

/**
 * Handles unhandled movement input.
 */
void APirateGameCharacter::UpdateMovement(const float DeltaTime)
{
	const float factor = DeltaTime * MovementSpeed;

	CharacterInput.Sanitize();
	const FVector DesiredMovementDirection = FVector(CharacterInput.MovementInput.X * factor,
													 CharacterInput.MovementInput.Y * factor,
													 0.0f);
	AddMovementInput(DesiredMovementDirection);
}

/**
 * Updates the AirControlBlockingTime
 */
void APirateGameCharacter::UpdateAirControlBlockingTime(const float DeltaTime)
{
	// Substract the passed time from the remaining air-control-blocking-time
	if (RemainingAirControlBlockingTime > 0.0f)
	{
		RemainingAirControlBlockingTime -= DeltaTime;
		// Reset the air control when the time is over
		if (RemainingAirControlBlockingTime <= 0.0f)
		{
			GetCharacterMovement()->AirControl = InitialAirControlValue;
		}
		// Set the air control to a low value if the time is not over
		else
		{
			GetCharacterMovement()->AirControl = 0.1f;
		}
	}
}

/**
 * Update rotation left or right based on the player movement.
 * Sets the rotation so that the character faces his direction of travel.
 */
void APirateGameCharacter::UpdateRotation()
{
	const float xVelocity = GetVelocity().X;
	if (Controller != nullptr) 
	{
		if (xVelocity < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
		else if (xVelocity > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

/** 
 * Updates the animation based on the characters state.
 */
void APirateGameCharacter::UpdateAnimation()
{
	// Determine which animation to use
	UPaperFlipbook* DesiredAnimation = nullptr;
	if (FMath::Abs(GetVelocity().X) > 0)
	{
		DesiredAnimation = RunningAnimation;
	}
	else
	{
		DesiredAnimation = IdleAnimation;
	}
	
	// Replace the animation
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

/**
 * This function sets the checkpoint to the given location
 */
void APirateGameCharacter::SetCheckpoint(const FVector & location)
{
	CurrentCheckpoint = location;
}

/**
 * This function returns the current checkpoint of this entity
 */
FVector APirateGameCharacter::GetCheckpoint() const
{
	return CurrentCheckpoint;
}

/**
 * To be called when the Actor's health is zero or lower
 */
void APirateGameCharacter::OnDie()
{
	APirateGameGameModeBase* gamemode = Cast<APirateGameGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gamemode != nullptr)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		
		// Show the game over widget
		APirateGameGameMode_Ingame *const gamemode = Cast<APirateGameGameMode_Ingame>(GetWorld()->GetAuthGameMode());
		if (gamemode != nullptr)
		{
			gamemode->ShowGameOverMenuWidget();
		}
	}
}

/** Returns the current health */
int APirateGameCharacter::GetHealth() const
{
	APirateGamePlayerState_Ingame *const pGameStateIngame = Cast<APirateGamePlayerState_Ingame>(this->GetController()->PlayerState);
	check(pGameStateIngame);

	return pGameStateIngame->GetRemainingLifes();
}

/**
 * Should set the health to a new value. Should also check if the player is dead inside the implementation
 */
void APirateGameCharacter::SetHealth(const int value)
{
	APirateGamePlayerState_Ingame *const pGameStateIngame = Cast<APirateGamePlayerState_Ingame>(this->GetController()->PlayerState);
	check(pGameStateIngame);

	pGameStateIngame->SetRemainingLifes(value);

	// We must check if the player is dead
	if (!IsAlive())
	{
		OnDie();
	}
}

/**
 * Checks by linetrace whether a walljump is possible
 * and jumps off of the object if possible
 */
void APirateGameCharacter::PerformWalljumpIfPossible()
{
	auto* cm = this->GetCharacterMovement();
	check(cm);
	check(GetCapsuleComponent());
	check(GetWorld());
	
	const FVector actorLocation = this->GetActorLocation();
	const FVector actorForwardVector = this->GetActorForwardVector();
	const float capsuleRadius = this->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float capsuleRadiusAddition = 5.0f;
	const float range = capsuleRadius + capsuleRadiusAddition;

	const FVector lineTraceStart = actorLocation - (actorForwardVector * range);
	const FVector lineTraceEnd = actorLocation + (actorForwardVector * range);

	FHitResult hitResult;
	FCollisionQueryParams cqp;
	cqp.AddIgnoredActor(this);

	// Search for an object to jump from by line-trace
	if (GetWorld()->LineTraceSingleByChannel(hitResult, lineTraceStart, lineTraceEnd, ECC_Visibility, cqp)
		&& hitResult.GetActor() != nullptr)
	{
		/* We found an actor to jump from */
		FVector launchVelocity;
		// Left or right depending on where the wall is -> jump away from wall
		launchVelocity.X = FMath::Sign(actorLocation.X - hitResult.Location.X) * cm->JumpZVelocity;
		launchVelocity.Z = cm->JumpZVelocity;
		// Override the z value only when the addition to the current velocity is higher
		// than the usual jump force
		const bool bOverrideZValue = this->GetVelocity().Z >= cm->JumpZVelocity ? true : false;
		/* Add a force to the characters movement. The x and y speed are always overridden,
		   the z speed only when it would be higher than the usual jump force.
		   If the z speed isn't higher (for example falling down),
		   then only add the jump force to the z speed. */
		this->LaunchCharacter(launchVelocity, true, bOverrideZValue);
		// Block movement in air by user input for a short amount of time to enable jumping away to the other side
		this->RemainingAirControlBlockingTime = this->AirControlBlockingTime;
	}
}