// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Math/Vector2D.h"
#include "MyEnum.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RPM = 600;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MuzzleVelocity = 10000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxRange = 1000000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RecoilTime = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RecoveryTime = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadTime = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxMagAmmoCount = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentBulletNum = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SwaySpeed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float XRecoilDeviation = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YRecoilDeviation = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector2D> RecoilPatterns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    EProjectileType ProjectileType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
    TSubclassOf<class ABullet> ProjectileClass;

	AGun();

    UFUNCTION(BlueprintCallable)
    void PullTrigger();

    UFUNCTION(BlueprintCallable)
    void ReleaseTrigger();

    UFUNCTION(BlueprintCallable)
    void SwitchFireMode();

    UFUNCTION(BlueprintCallable)
    int32 Reload();

    UFUNCTION(BlueprintCallable)
    UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void FireShot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere)
    USoundBase* MuzzleSound;

    UPROPERTY(EditAnywhere)
    USoundBase* DryFireSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    int32 CurrentRecoilPattern_Idx;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    bool bFullAuto = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    bool bIsStillFiring = false;

    bool bCanFire = true;

    // Fire---------------------
    bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

    bool GetCanFire();

    UFUNCTION()
    void SetCanFire(bool bSetCanFire = true);
    // Fire---------------------

    // Handle gun refire 
    FTimerHandle TH_GunRefire;
    FTimerDelegate TD_SetCanFire;

    // Get owner controller
    AController* GetOwnerController() const;
    
};
