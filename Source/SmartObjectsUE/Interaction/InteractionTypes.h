#pragma once
#include "CoreMinimal.h"
#include "GameplayBehavior.h"
#include "SmartObjectSubsystem.h"

#include "InteractionTypes.generated.h"

USTRUCT(BlueprintType)
struct FSmartObjectContext
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	AActor* OwningActor;

	UPROPERTY(BlueprintReadOnly)
	APawn* Occupant;

	TOptional<FSmartObjectRequestResult> RequestResult;
	TOptional<FSmartObjectClaimHandle> ClaimHandle;
	TOptional<UGameplayBehavior*> GameplayBehavior;

	bool operator==(const FSmartObjectContext& Other) const
	{
		if(RequestResult.IsSet() && Other.RequestResult.IsSet())
		{
			return RequestResult == Other.RequestResult;
		}

		if(ClaimHandle.IsSet() && Other.ClaimHandle.IsSet())
		{
			return ClaimHandle == Other.ClaimHandle;
		}
		
		return OwningActor == Other.OwningActor;
	}
};