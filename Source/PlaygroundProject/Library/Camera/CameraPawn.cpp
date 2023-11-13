// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "PlaygroundProject/PlaygroundProjectPlayerController.h"


// Sets default values
ACameraPawn::ACameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1500.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation();
	TargetZoom = 3000.0f;

	const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + -45, Rotation.Yaw, 0.0f);

	BindToInput();
}
void ACameraPawn::BindToInput() {

	UE_LOG(LogTemp, Warning, TEXT("Bind to input"));


	//if (Controller) {
	//	//UEnhancedLocalPlayerSubsystem* subsystem
	//	if (Controller->IsA(APlaygroundProjectPlayerController::StaticClass())) {
	//		UE_LOG(LogTemp, Warning, TEXT("Controller is of the correct type"));
	//	}

	//	//WE CAN USE THIS TO MAP TO CUSTOM MAPPING CONTEXT: Mapping context is like the section of controll mappings i,e, menu controls, vehcile controls etc
	//	//for now we will use the default one
	//	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController());
	//	if (Subsystem) {
	//		Subsystem->AddMappingContext(CameraMappingContext, 0);
	//	}
	//	else {
	//		//log error...? should not be null bruh
	//	}

	//}
	//else {
	//	//log error...? should not be null bruh
	//}
}
// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBounds();

	//const FVector InterpolatedLocation = UKismetMathLibrary::
	const FVector NewLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(NewLocation);

	//Zoom
	const float NewZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = NewZoom;

	//Rotate
	const FRotator NewRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent->SetRelativeRotation(NewRotation);
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{  
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mapping actions"));
		// Setup mouse input events
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACameraPawn::Zoom);
		EnhancedInputComponent->BindAction(ForwardAction, ETriggerEvent::Triggered, this, &ACameraPawn::Forward);
		EnhancedInputComponent->BindAction(BackwardAction, ETriggerEvent::Triggered, this, &ACameraPawn::Backward);
		EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &ACameraPawn::Right);
		EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &ACameraPawn::Left);
		EnhancedInputComponent->BindAction(RotateRightAction, ETriggerEvent::Triggered, this, &ACameraPawn::RotateRight);
		EnhancedInputComponent->BindAction(RotateLeftAction, ETriggerEvent::Triggered, this, &ACameraPawn::RotateLeft);

		EnhancedInputComponent->BindAction(EnableRotateAction, ETriggerEvent::Started, this, &ACameraPawn::EnableRotate);
		EnhancedInputComponent->BindAction(EnableRotateAction, ETriggerEvent::Completed, this, &ACameraPawn::DisableRotate);
		EnhancedInputComponent->BindAction(RotateHorizontalAction, ETriggerEvent::Triggered, this, &ACameraPawn::RotateHorizontal);
		EnhancedInputComponent->BindAction(RotateVerticalAction, ETriggerEvent::Triggered, this, &ACameraPawn::RotateVertical);
		if (RotateHorizontalAction) {
			UE_LOG(LogTemp, Warning, TEXT("RotateHorizontalAction IS NULL"));
		}
		UE_LOG(LogTemp, Warning, TEXT("End mapping actions"));
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AMyProject3PlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AMyProject3PlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AMyProject3PlayerController::OnSetDestinationReleased);

		//// Setup touch input events
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyProject3PlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyProject3PlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyProject3PlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyProject3PlayerController::OnTouchReleased);
	}
}

void ACameraPawn::MoveToPosition(FVector targetLocation)
{
	TargetLocation = targetLocation;
}

void ACameraPawn::Forward()
{ 
	TargetLocation = (SpringArmComponent->GetForwardVector() *  MoveSpeed) + TargetLocation;
}

void ACameraPawn::Backward()
{
	TargetLocation = (SpringArmComponent->GetForwardVector() * -1 * MoveSpeed) + TargetLocation;
}

void ACameraPawn::Right()
{
	TargetLocation = (SpringArmComponent->GetRightVector() * MoveSpeed) + TargetLocation;
}

void ACameraPawn::Left()
{
	TargetLocation = (SpringArmComponent->GetRightVector() * -1 * MoveSpeed) + TargetLocation;
} 

void ACameraPawn::Zoom(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	if (AxisValue == 0.0f) {
		return;
	}
	const float Zoom = -1 * AxisValue * ZoomSensitivity;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, MinZoom, MaxZoom);
}

void ACameraPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, (-1) * 1 * RotationSensitivity, 0.0f));
	// (SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed) + TargetLocation;
}

void ACameraPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 1 * RotationSensitivity, 0.0f));
}

void ACameraPawn::EnableRotate()
{
	UE_LOG(LogTemp, Warning, TEXT("Enable Rotate"));
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch > RotatePitchMax) { //max is 10f
		NewPitch = RotatePitchMax;
	}
	else if (TargetRotation.Pitch < RotatePitchMin) { //min is 80f
		NewPitch = RotatePitchMin;
	}
	UE_LOG(LogTemp, Warning, TEXT("Enable Rotate, %f"), NewPitch);
	CanRotate = true;
}

void ACameraPawn::DisableRotate()
{
	UE_LOG(LogTemp, Warning, TEXT("Disable Rotate"));

	CanRotate = false;
}

void ACameraPawn::RotateHorizontal(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	if (AxisValue == 0.0f) {
		return;
	}

	if (CanRotate) {
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, AxisValue * RotationAxisSensitivity, 0.0f));
	}
}

void ACameraPawn::RotateVertical(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	if (AxisValue == 0.0f) {
		return;
	}
	if (CanRotate && VerticalRotationEnabled) {
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue * RotationAxisSensitivity, 0.0f, 0.0f));
	}
}

void ACameraPawn::CameraBounds()
{
	//I dont get that???
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch > RotatePitchMax) { //max is 10f
		NewPitch = RotatePitchMax;
	}
	else if (TargetRotation.Pitch < RotatePitchMin) { //min is 80f
		NewPitch = RotatePitchMin;
	}

	//Set the new pitch and prevent any camera roll
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.0f);

	//Clamp location within bounds (currently there arent any besies Vertical 0)
	//The camera is a pawn that is actually moving around x/y without collision
	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.0f);
}