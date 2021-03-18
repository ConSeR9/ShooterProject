// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ShooterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UW_ShooterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintImplementableEvent)
        void FadeIn();

    UFUNCTION(BlueprintImplementableEvent)
        void FadeOut();

};
