// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectSubsystem.h"
#include "Components/ActorComponent.h"
#include "Interaction/SmartObjectActor.h"
#include "SmartObjectInteraction.generated.h"

USTRUCT(BlueprintType)
struct FSmartObjectSearchParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector QuerierLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SearchAreaSize = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSmartObjectRequestFilter RequestFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PreferredSlotIndex = -1;
};

DECLARE_DYNAMIC_DELEGATE_RetVal(FSmartObjectSearchParams, FOnConfigureSmartObjectSearchParams);


UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class SMARTOBJECTSUE_API USmartObjectInteraction : public UActorComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Smart Object Interaction")
	void BeginInteraction();

	UFUNCTION(BlueprintCallable, Category="Smart Object Interaction")
	void EndInteraction();

	UFUNCTION(BlueprintCallable)
	void BindConfigurationDelegates(const FOnConfigureSmartObjectSearchParams& OnConfigurationDelegate);

protected:

	TSharedPtr<FSmartObjectContext> RequestSmartObject(const FSmartObjectSearchParams& SearchParams) const;

	void ClaimAndUseCurrentContext();
	void UseCurrentContext() const;

	
	void ReleaseCurrentContext() const;
	TSharedPtr<FSmartObjectContext> MakeContext(FSmartObjectRequestResult SmartObjectRequestResult) const;
	TSharedPtr<FSmartObjectContext> MakeContextBySlotIndex(int SlotIndex, FSmartObjectHandle SmartObjectHandle) const;

private:
	APawn* GetMyPawn() const;

	UPROPERTY()
	FOnConfigureSmartObjectSearchParams OnConfigureSmartObjectSearchParams;

	TSharedPtr<FSmartObjectContext> CurrentContext;
		
};
