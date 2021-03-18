// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "MyEnum.h"
#include "ShooterCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCarriedAmmo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CarriedAmmoCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EProjectileType ProjectileType;

};

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	AShooterCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    float SprintSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    float SprintAcceleration;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    bool bIsMovingForward;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    bool bCanSprint;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    bool bTryingToSprint;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Crouching")
    bool bCanCrouch;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Crouching")
    bool bTryingToCrouch;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Crouching")
    bool bIsCrouching;

    // 1: Rifle
    // 2: SMG
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<FCarriedAmmo> CarriedAmmo;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure)
    bool IsDead() const;

    UFUNCTION(BlueprintPure)
    float GetHP() const;

    UFUNCTION(BlueprintPure)
    float GetHPPercent() const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void Dead();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void DeadCamera();

    // Shooting mechanism
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void StartFire();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void StopFire();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SwitchFireMode();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void AddImpulseToCharacter(FName BoneName, FVector HitLocation, FVector Velocity, float ImpactMultiplier);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void AddRecoil(FVector2D RecoilPattern, float RecoilTime, float RecoveryTime);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetIsStillFiring(bool IsStillFiring);

private:
    // Moving mechanism
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void ControllerPitchInput(float AxisValue);
    void ControllerYawInput(float AxisValue);
    //void TrySprint();
    //void StopTrySprint();
    //void Sprint();
    //void StopSprint();
    //void CheckMovingForward();
    //void CheckNotMovingForward();

    UPROPERTY(EditDefaultsOnly)
    float MaxHP = 100;

    UPROPERTY(VisibleAnywhere)
    float HP;

};