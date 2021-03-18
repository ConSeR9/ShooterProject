// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

    UFUNCTION()
    void GameOver(class AActor* EndGameFocus = nullptr, bool bIsWinner = false);

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UW_ShooterWidget> HUDClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UW_ShooterWidget> WinScreenClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UW_ShooterWidget> LoseScreenClass;

    UPROPERTY(EditAnywhere)
    float GameOverDelay = 3.f;

    UPROPERTY(EditAnywhere)
    float RestartDelay = 5.f;

    FTimerDelegate GameOverDel;
    FTimerHandle GameOverTimer;
    FTimerHandle RestartTimer;

    UPROPERTY()
    UUserWidget* HUD;
};
