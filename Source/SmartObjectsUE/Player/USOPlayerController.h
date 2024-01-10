// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "USOPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SMARTOBJECTSUE_API AUSOPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AUSOPlayerController();
private:
	UPROPERTY(VisibleAnywhere)
	UPathFollowingComponent* PathFollowingComponent;
};
