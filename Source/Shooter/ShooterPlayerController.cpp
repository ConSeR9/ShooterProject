// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "W_ShooterWidget.h"

// Called when the game starts or when spawned
void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (!IsValid(HUD))
    {
        //Error, display log
        UE_LOG(LogTemp, Warning, TEXT("HUD is not valid"));
    }
    else
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    
    //EndGameFocus = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    /*
    if (IsValid(EndGameFocus))
        UE_LOG(LogTemp, Warning, TEXT("EndGameFocus is: %s"), *EndGameFocus->GetName());
    */

    GameOverDel.BindUFunction(this, FName("GameOver"), EndGameFocus, bIsWinner);
    GetWorldTimerManager().SetTimer(GameOverTimer, GameOverDel, GameOverDelay, false);

    UE_LOG(LogTemp, Warning, TEXT("Game has ended"));
}

void AShooterPlayerController::GameOver(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    
    //if (bIsWinner)
    //{
    //    UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
    //    if (!IsValid(WinScreen))
    //    {
    //        //Error, display log
    //        UE_LOG(LogTemp, Warning, TEXT("WinScreen is not valid"));
    //    }
    //    else
    //    {
    //        //UE_LOG(LogTemp, Warning, TEXT("Win Screen display"));
    //        UW_ShooterWidget* UWWinScreen = Cast<UW_ShooterWidget>(WinScreen);
    //        WinScreen->AddToViewport();
    //        UWWinScreen->FadeOut();
    //    }
    //}
    //else //Player lose
    //{
    //    UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
    //    if (!IsValid(LoseScreen))
    //    {
    //        //Error, display log
    //        UE_LOG(LogTemp, Warning, TEXT("LoseScreen is not valid"));
    //    }
    //    else
    //    {
    //        //UE_LOG(LogTemp, Warning, TEXT("Lose Screen display"));
    //        UW_ShooterWidget* UWLoseScreen = Cast<UW_ShooterWidget>(LoseScreen);
    //        LoseScreen->AddToViewport();
    //        UWLoseScreen->FadeOut();
    //    }
    //}

    //GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
