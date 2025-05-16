// Fill out your copyright notice in the Description page of Project Settings.


#include "doSteve.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Sets default values
AdoSteve::AdoSteve()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create spring arm and attach to root (capsule)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// Create camera and attach to spring arm
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false; 
}


// Called when the game starts or when spawned
void AdoSteve::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (defaultMappingContext)
				{
					Subsystem->AddMappingContext(defaultMappingContext, 0);
					UE_LOG(LogTemp, Warning, TEXT("Manually added mapping context in BeginPlay"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is NULL in BeginPlay"));
				}
			}
		}
	}
}


void AdoSteve::moveForward(const FInputActionValue& value)
{
	float AxisValue = value.Get<float>();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("moveForward Axis: %f"), AxisValue));
	}

	AddMovementInput(GetActorForwardVector(), AxisValue);
}

// Called every frame
void AdoSteve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AdoSteve::Turn(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	AddControllerYawInput(AxisValue);
}

// Called to bind functionality to input
void AdoSteve::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called"));

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully cast to EnhancedInputComponent"));

		if (moveForwardAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveForwardAction is valid. Binding it now."));
			EnhancedInput->BindAction(moveForwardAction, ETriggerEvent::Triggered, this, &AdoSteve::moveForward);
		
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MoveForwardAction is NULL!"));
		}
		if (TurnAction)
		{
			EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AdoSteve::Turn);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TurnAction is NULL!"));  
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerInputComponent to EnhancedInputComponent"));
	}
}


