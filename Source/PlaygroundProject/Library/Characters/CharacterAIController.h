// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CharacterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDPROJECT_API ACharacterAIController : public AAIController
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ACharacterAIController();

	FGuid Id;
};
