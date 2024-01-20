// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFunctionLibrary.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

void UInteractionFunctionLibrary::AsyncMoveTo(APawn* Pawn, const FTransform& Transform, const FOnMoveFinished& OnMoveFinished)
{
	AController* Controller = Pawn->GetController();

	if(UPathFollowingComponent* PathFollowingComponent = Controller->FindComponentByClass<UPathFollowingComponent>())
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Transform.GetLocation());
		PathFollowingComponent->OnRequestFinished.AddLambda([=](FAIRequestID RequestID, const FPathFollowingResult& Result)
		{
			Pawn->GetMovementComponent()->StopMovementImmediately();
			Pawn->TeleportTo(FVector(
				Transform.GetLocation().X,
				Transform.GetLocation().Y,
				Pawn->GetActorLocation().Z),
				Transform.GetRotation().Rotator());
			
			OnMoveFinished.ExecuteIfBound(true);
		});
	}
	else
	{
		OnMoveFinished.ExecuteIfBound(false);
	}
	
}
