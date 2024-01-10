// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObjectActor.h"

#include "SmartObjectComponent.h"


// Sets default values
ASmartObjectActor::ASmartObjectActor()
{
	SmartObjectComponent = CreateDefaultSubobject<USmartObjectComponent>("Smart Object");
}

USmartObjectComponent* ASmartObjectActor::GetSmartObjectComponent() const
{
	return SmartObjectComponent;
}


