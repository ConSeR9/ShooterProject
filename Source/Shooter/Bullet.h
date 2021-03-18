// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "MyEnum.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTER_API ABullet : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImpactMultiplier = 0;

    // Sets default values for this actor's properties
    ABullet();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Sphere collision component
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
    UStaticMeshComponent* ProjectileMeshComponent;

    // Projectile movement component.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float BulletCollisionSize = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    EProjectileType ProjectileType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float HeadMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float TorsoMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float LimbMultiplier;

    // Function that initializes the projectile's velocity in the shoot direction.
    UFUNCTION(BlueprintCallable)
    void FireInDirection(const FVector& ShootDirection);

    // Function that is called when the projectile hits something.
    UFUNCTION(BlueprintCallable)
    void ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    // Set the controller who shot this bullet
    UFUNCTION(BlueprintCallable)
    void SetOwnerController(AController* TargetController);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    AController* OwnerController = nullptr;

    UPROPERTY(EditAnywhere)
    UParticleSystem* ImpactEffect;

    UPROPERTY(EditAnywhere)
    USoundBase* ImpactSound;

};
