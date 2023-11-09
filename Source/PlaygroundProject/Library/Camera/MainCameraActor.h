// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h" 
#include "InputActionValue.h"
#include "MainCameraActor.generated.h"


class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class PLAYGROUNDPROJECT_API AMainCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void BindToInput();
protected:
	//this annotation allows blueprint mapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UInputMappingContext* CameraMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UInputAction* ZoomAction;

	FVector TargetLocation;

	void Zoom(const FInputActionValue& Value);

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponenet);
	
};
