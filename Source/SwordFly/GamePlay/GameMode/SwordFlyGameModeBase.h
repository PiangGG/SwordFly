// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SwordFlyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFLY_API ASwordFlyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	public:
	ASwordFlyGameModeBase();

	virtual void Logout(AController* Exiting) override;

	virtual void Tick(float DeltaSeconds) override;
};

