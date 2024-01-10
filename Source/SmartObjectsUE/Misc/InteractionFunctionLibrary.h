// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionFunctionLibrary.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnMoveFinished, bool)

UCLASS()
class SMARTOBJECTSUE_API UInteractionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void AsyncMoveTo(APawn* Pawn, FVector Location, const FOnMoveFinished& OnMoveFinished);
};
