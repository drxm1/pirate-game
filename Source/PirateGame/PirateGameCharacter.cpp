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
	CameraBoom->CameraLagSpeed = 0.4f;

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
		// TODO: A walljump might be possible if a wall is in front (LineTrace check for Wall ? Yes -> Walljump : No : Nothing)
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
		// TODO: show not the pause menu widget but the GameOver widget...
		// gamemode->ShowPauseMenuWidget(!bIsGamePaused);
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