// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlaygroundProjectGameMode.h"
#include "PlaygroundProjectPlayerController.h"
#include "PlaygroundProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Library/Camera/CameraPawn.h"

APlaygroundProjectGameMode::APlaygroundProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APlaygroundProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character

	//NICE: Lookup blueprint that is not instatiated
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter")); //Original Code
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_CameraPawn")); 
	//if (PlayerPawnBPClass.Class != nullptr)
	//{
	//	FName str = PlayerPawnBPClass.Class->GetFName(); 
	//	FString strr = str.ToString();
	//	UE_LOG(LogTemp, Warning, TEXT("AAAAAAAA %s"), *FString(strr));
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("Not Found"));
	//}

	//// set default controller to our Blueprinted controller
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	//if(PlayerControllerBPClass.Class != NULL)
	//{
	//	PlayerControllerClass = PlayerControllerBPClass.Class;
	//}
}