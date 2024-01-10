// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionTypes.h"
#include "GameFramework/Actor.h"
#include "SmartObjectActor.generated.h"

class USmartObjectComponent;

UCLASS()
class SMARTOBJECTSUE_API ASmartObjectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmartObjectActor();
	
	USmartObjectComponent* GetSmartObjectComponent() const;

private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true))
	USmartObjectComponent* SmartObjectComponent;

private:
	UPROPERTY()
	TArray<FSmartObjectContext> CurrentContexts;
};
