// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
#include "DMGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ADMGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void PawnKilled(APawn* PawnKilled) override;

private:
    void EndGame(bool bIsPlayerWinner);

};
