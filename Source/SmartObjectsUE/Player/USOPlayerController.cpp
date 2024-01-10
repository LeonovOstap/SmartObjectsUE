// Fill out your copyright notice in the Description page of Project Settings.


#include "USOPlayerController.h"

AUSOPlayerController::AUSOPlayerController()
{
	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>("Path Following");
}
