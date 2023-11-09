// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCameraActor.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h" 
#include "Kismet/KismetMathLibrary.h"

void AMainCameraActor::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Bind to input"));
	// This is not automatically called. Call it yourself here.
	BindToInput();
	TargetLocation = GetActorLocation();
	SetActorTickEnabled(true);
	////initial camera rotation
	//FRotator TargetRotation = GetActorRotation();
	//
	//const FRotator Rotation = GetActorRotation();
	//TargetRotation = FRotator(Rotation.Pitch + 50, Rotation.Yaw, 0.0f);
}
void AMainCameraActor::Zoom(const FInputActionValue& Value) {
	const EInputActionValueType asd = Value.GetValueType();

	if (asd == EInputActionValueType::Axis1D) {
		UE_LOG(LogTemp, Warning, TEXT("Action Triggered with value type Axis1D"));
	}
	const float CurrentValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Action Triggered with value: %f "), CurrentValue);

	if (Value.IsNonZero()) {
		UE_LOG(LogTemp, Warning, TEXT("Non Zeror Action Triggered with value"));
	}


	FVector ForwardVector = GetActorForwardVector();

	//20 is the move speed
	TargetLocation = (ForwardVector * CurrentValue * 300.0f) + TargetLocation;
	UE_LOG(LogTemp, Warning, TEXT("New Target Location"));
	//MovementComponent in order to use add movement input
	//etActorTransform() 

}

void AMainCameraActor::BindToInput()
{
	UE_LOG(LogTemp, Warning, TEXT("Bind to input"));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (Controller) {
		//UEnhancedLocalPlayerSubsystem* subsystem

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
		if (Subsystem) {
			Subsystem->AddMappingContext(CameraMappingContext, 0);
		}
		else {
			//log error...? should not be null bruh
		}

	}
	else {
		//log error...? should not be null bruh
	}

	//Old Way
	//// Initialize our component
	InputComponent = NewObject<UEnhancedInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		// Bind inputs here
		// InputComponent->BindAction("Jump", IE_Pressed, this, &AUnrealisticPawn::Jump);
		// etc...

		// Now hook up our InputComponent to one in a Player
		// Controller, so that input flows down to us
		EnableInput(GetWorld()->GetFirstPlayerController());
		SetupPlayerInputComponent(InputComponent);
	}

}

void AMainCameraActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponenet) {

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponenet)) {
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMainCameraActor::Zoom);
		UE_LOG(LogTemp, Warning, TEXT("Setup Component"));
	}
}

void AMainCameraActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	const FVector NewLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 300.0f);
	SetActorLocation(NewLocation);
}