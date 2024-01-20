// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObjectInteraction.h"

#include "AITypes.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "SmartObjectComponent.h"
#include "Misc/InteractionFunctionLibrary.h"

void USmartObjectInteraction::BeginInteraction()
{
	const FSmartObjectSearchParams& SearchParams = OnConfigureSmartObjectSearchParams.Execute();
	CurrentContext = RequestSmartObject(SearchParams);

	ClaimAndUseCurrentContext();
}

void USmartObjectInteraction::EndInteraction()
{
	if(CurrentContext && CurrentContext->ClaimHandle)
	{
		ReleaseCurrentContext();
	}
}

void USmartObjectInteraction::BindConfigurationDelegates(
	const FOnConfigureSmartObjectSearchParams& OnConfigurationDelegate)
{
	OnConfigureSmartObjectSearchParams = OnConfigurationDelegate;
}

TSharedPtr<FSmartObjectContext> USmartObjectInteraction::RequestSmartObject(
	const FSmartObjectSearchParams& SearchParams) const
{
	FSmartObjectRequest SmartObjectRequest;

	const FBox QueryBox = FBox(SearchParams.QuerierLocation - SearchParams.SearchAreaSize,
		SearchParams.QuerierLocation + SearchParams.SearchAreaSize);
	SmartObjectRequest.QueryBox = QueryBox;
	SmartObjectRequest.Filter = SearchParams.RequestFilter;

	
	const USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
	const FSmartObjectRequestResult& SmartObjectRequestResult = SmartObjectSubsystem->FindSmartObject(SmartObjectRequest);
	
	if(SearchParams.PreferredSlotIndex == -1)
	{
		return MakeContext(SmartObjectRequestResult);
	}
	else
	{
		return MakeContextBySlotIndex(SearchParams.PreferredSlotIndex, SmartObjectRequestResult.SmartObjectHandle);
	}

	return nullptr;
}

void USmartObjectInteraction::ClaimAndUseCurrentContext()
{
	if(!CurrentContext)
		return;

	USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
	CurrentContext->ClaimHandle = SmartObjectSubsystem->Claim(CurrentContext->RequestResult.GetValue());

	TOptional<FTransform> SlotTransform = SmartObjectSubsystem->GetSlotTransform(CurrentContext->ClaimHandle.GetValue());
	
	UInteractionFunctionLibrary::AsyncMoveTo(GetMyPawn(), SlotTransform.GetValue(), FOnMoveFinished::CreateWeakLambda(this,
		                                         [this](bool bSuccess)
		                                         {
			                                         if(bSuccess)
			                                         {
				                                         UseCurrentContext();
			                                         }
		                                         }));
}

void USmartObjectInteraction::UseCurrentContext() const
{
	check(CurrentContext && CurrentContext->ClaimHandle.IsSet())
	
	USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
	
	const UGameplayBehaviorSmartObjectBehaviorDefinition* BehaviorDefinition =
		SmartObjectSubsystem->Use<UGameplayBehaviorSmartObjectBehaviorDefinition>(CurrentContext->ClaimHandle.GetValue());
	
	UGameplayBehavior* GameplayBehaviour = BehaviorDefinition->GameplayBehaviorConfig->GetBehavior(*GetWorld());
	GameplayBehaviour->Trigger(*GetMyPawn(), BehaviorDefinition->GameplayBehaviorConfig, CurrentContext->OwningActor);
}

void USmartObjectInteraction::ReleaseCurrentContext() const
{
	if(CurrentContext && CurrentContext->ClaimHandle.IsSet())
	{
		USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
		SmartObjectSubsystem->Release(CurrentContext->ClaimHandle.GetValue());
	}
	else
	{
		ensureMsgf(false, TEXT("Tried to release non existing context"));
	}
}

TSharedPtr<FSmartObjectContext> USmartObjectInteraction::MakeContext(
	FSmartObjectRequestResult SmartObjectRequestResult) const
{
	if(!SmartObjectRequestResult.IsValid())
		return nullptr;
	const USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
	const USmartObjectComponent* SmartObjectComponent = SmartObjectSubsystem->GetSmartObjectComponentByRequestResult(SmartObjectRequestResult);

	TSharedPtr<FSmartObjectContext> SmartObjectContext = MakeShared<FSmartObjectContext>();
	
	if(ASmartObjectActor* SmartObjectActor = SmartObjectComponent->GetOwner<ASmartObjectActor>())
	{
		SmartObjectContext->OwningActor = SmartObjectActor;
		SmartObjectContext->Occupant = GetMyPawn();
		SmartObjectContext->RequestResult = SmartObjectRequestResult;

		return SmartObjectContext;
	}

	return nullptr;
}

TSharedPtr<FSmartObjectContext> USmartObjectInteraction::MakeContextBySlotIndex(int SlotIndex,
	FSmartObjectHandle SmartObjectHandle) const
{
	const USmartObjectSubsystem* SmartObjectSubsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
	
	TArray<FSmartObjectSlotHandle> SlotHandles;
	SmartObjectSubsystem->GetAllSlots(SmartObjectHandle, SlotHandles);

	const FSmartObjectSlotHandle SlotHandle = SlotHandles[SlotIndex];
	
	FSmartObjectRequestResult RequestResult;
	RequestResult.SmartObjectHandle = SmartObjectHandle;
	RequestResult.SlotHandle = SlotHandle;

	return MakeContext(RequestResult);
}

APawn* USmartObjectInteraction::GetMyPawn() const
{
	if(APawn* Pawn = GetOwner<APawn>())
		return Pawn;

	if(const AController* Controller = GetOwner<AController>())
		return Controller->GetPawn();

	checkNoEntry();
	return nullptr;
}
