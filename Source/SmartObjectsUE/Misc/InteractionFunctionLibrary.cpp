// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionFunctionLibrary.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"

void UInteractionFunctionLibrary::AsyncMoveTo(APawn* Pawn, FVector Location, const FOnMoveFinished& OnMoveFinished)
{
	AController* Controller = Pawn->GetController();

	if(UPathFollowingComponent* PathFollowingComponent = Controller->FindComponentByClass<UPathFollowingComponent>())
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Location);
		PathFollowingComponent->OnRequestFinished.AddLambda([OnMoveFinished](FAIRequestID RequestID, const FPathFollowingResult& Result)
		{
			OnMoveFinished.Execute(Result.IsSuccess());
		});
	}
	
}
