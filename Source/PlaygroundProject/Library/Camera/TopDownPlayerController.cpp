// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"

#include "PlaygroundProject/Library/Characters/SimpleCharacter.h"

#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h" 
#include "Engine/LocalPlayer.h"
#include <CharacterAIController.h>
#include <CameraPawn.h>

DEFINE_LOG_CATEGORY(TopDownPlayerController);

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
}

void ATopDownPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (shouldCameraFollow) {
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn != nullptr) {
			ACameraPawn* cameraPawn = Cast<ACameraPawn>(ControlledPawn);
			if (cameraPawn != nullptr) {
				if(ControlledCharacter!=nullptr)
				cameraPawn->MoveToPosition(ControlledCharacter->GetTargetLocation());
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is not ACameraPawn. Can not follow character."));
			}
		}
	}
}

void ATopDownPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATopDownPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATopDownPlayerController::OnSetDestinationReleased);


		EnhancedInputComponent->BindAction(EnableCameraFollowAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::FollowCharacter);
		EnhancedInputComponent->BindAction(EnableCameraFollowAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::StopFollowingCharacter);
	}
}

void ATopDownPlayerController::OnInputStarted()
{
	
	FHitResult HitResult;
	bool hasHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);
	if (!hasHit)
		return;
	if (TrySelectCharacter(HitResult)) {
		return;
	}
	else if (TryMoveCharacter(HitResult)) {

	}
	return;
	// If we hit a surface, cache the location
	/*if (hitPawn)
	{
		AActor* actor = Hit.GetActor();
		if (actor != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Actor is not null, %s"), *FString(actor->GetClass()->GetDisplayNameText().ToString()));
			ASimpleCharacter* charcater = Cast<ASimpleCharacter>(actor);
			if (actor->IsA(ASimpleCharacter::StaticClass()) || dynamic_cast<const ASimpleCharacter*>(actor) != nullptr || charcater != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Hit actor of type ASimpleCharacter"));
				if (CharacterController != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("Posessing pawn ASimpleCharacter"));
					CharacterController->Possess(Cast<APawn>(actor));
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("select successful"));
	}*/
}

bool ATopDownPlayerController::TryMoveCharacter(FHitResult hit) {
	 
	if (ControlledCharacter != nullptr) { 
		StopMovement();
		//FollowTime += GetWorld()->GetDeltaSeconds();
		CachedDestination = hit.Location;
		FVector WorldDirection = (CachedDestination - ControlledCharacter->GetActorLocation()).GetSafeNormal();
		ControlledCharacter->AddMovementInput(WorldDirection, 1.0, false);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControlledCharacter->GetController(), CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(ControlledCharacter->GetController(), FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ControlledCharacter is not selected. Nothing to move move."));
		return false;
	}

	return true;
}

void ATopDownPlayerController::FollowCharacter()
{
	shouldCameraFollow = true;
}

void ATopDownPlayerController::StopFollowingCharacter()
{
	shouldCameraFollow = false;
}

bool ATopDownPlayerController::TrySelectCharacter(FHitResult hit) {
	 
	UE_LOG(LogTemp, Warning, TEXT("Selecting Pawn"));
	AActor* actor = hit.GetActor();
	if (actor != nullptr) {
		FString actorType = actor->GetClass()->GetDisplayNameText().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Hit actor of type: %s"), *FString(actorType));
		if (actor->IsA(ASimpleCharacter::StaticClass())) {
			ControlledCharacter = Cast<ASimpleCharacter>(actor);
			UE_LOG(LogTemp, Warning, TEXT("Selecting %s"), *FString(actorType));
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("CharacterController is null, cant posses: %s"), *FString(actorType));
			return false;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is null, cant select it!"));
		return false;
	}

	return true;
}

// Triggered every frame when the input is held down
void ATopDownPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	//FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	//FHitResult Hit;
	//bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	//if (bHitSuccessful)
	//{
		//CachedDestination = Hit.Location;
		/*AActor* actor = Hit.GetActor();
		if (actor != nullptr) {
			if (actor->IsA(ASimpleCharacter::StaticClass()) || dynamic_cast<const ASimpleCharacter*>(actor) != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Hit actor of type ASimpleCharacter"));
				if (CharacterController != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("Posessing pawn ASimpleCharacter"));
					CharacterController->Possess(Cast<APawn>(actor));
				}
			}
		}*/

		//UE_LOG(LogTemp, Warning, TEXT("hit successful")); 

	//}

	//if (CharacterController != nullptr) {
	//	// Move towards mouse pointer or touch
	//	APawn* ControlledPawn = CharacterController->GetPawn();
	//	if (ControlledPawn != nullptr)
	//	{
	//		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	//		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	//	}
	//}
}

void ATopDownPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	//if (FollowTime <= ShortPressThreshold)
	{
		if (ControlledCharacter != nullptr) {
			// We move there and spawn some particles  
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(ControlledCharacter->GetController(), CachedDestination);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(ControlledCharacter->GetController(), FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}
	}

	//FollowTime = 0.f;
}