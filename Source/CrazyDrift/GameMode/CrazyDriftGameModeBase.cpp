// Fill out your copyright notice in the Description page of Project Settings.

#include "CrazyDrift.h"
#include "CrazyDriftGameModeBase.h"
#include "PlayerWheeledVehicle/PlayerWheeledVehicle.h"

ACrazyDriftGameModeBase::ACrazyDriftGameModeBase()
{
	DefaultPawnClass = APlayerWheeledVehicle::StaticClass();
}
