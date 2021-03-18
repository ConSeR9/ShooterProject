// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Gun.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ABullet::ABullet()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Delete the projectile after 3 seconds.
    InitialLifeSpan = 3.0f;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // Use a sphere as a simple collision representation.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // Set the sphere's collision profile name to "Projectile".
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        // Event called when component hits something.
        //CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::ProjectileOnHit);
        // Set the sphere's collision radius.
        CollisionComponent->InitSphereRadius(BulletCollisionSize);
        // Set the root component to be the collision component.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        /*static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Assets/Weapons/RifleBullet_556/Projectile_556x45.Projectile_556x45'"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }*/
        ProjectileMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }

    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        //ProjectileMovementComponent->InitialSpeed = 3000.0f;
        //ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
    Super::BeginPlay();

    AGun* OwnerGun = Cast<AGun>(GetOwner());
    ProjectileMovementComponent->InitialSpeed = OwnerGun->MuzzleVelocity;
    ProjectileMovementComponent->MaxSpeed = OwnerGun->MuzzleVelocity;
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DrawDebugPoint(GetWorld(), GetActorLocation(), 5, FColor::Red, false, 0.5f);
}

// Function that initializes the projectile's velocity in the shoot direction.
void ABullet::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABullet::ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Bullet Hit!"));
    UE_LOG(LogTemp, Warning, TEXT("The bone that got hit: %s"), *Hit.BoneName.ToString());
    if (OtherActor == GetOwner()->GetOwner())
        return;

    if (OtherComponent->IsSimulatingPhysics())
    {
        AShooterCharacter* OtherShooterCharacter = Cast<AShooterCharacter>(OtherActor);
        if (!IsValid(OtherShooterCharacter))
        {
            OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 0.8f, Hit.ImpactPoint);
        }
    }

    EPhysicalSurface HitSurfaceType = Hit.PhysMaterial->SurfaceType;
    if (HitSurfaceType == EPhysicalSurface::SurfaceType1) // if SurfaceType == "Flesh_Head"
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Head"));
        Damage *= HeadMultiplier;
    }
    else if (HitSurfaceType == EPhysicalSurface::SurfaceType2) // Torso
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Torso"));
        Damage *= TorsoMultiplier;
    }
    else if (HitSurfaceType == EPhysicalSurface::SurfaceType3) // Limb
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Limb"));
        Damage *= LimbMultiplier;
    }
    else // Hit Some other things
    {
        UE_LOG(LogTemp, Warning, TEXT("The thing you hit: %s"), *OtherActor->GetName());
    }

    FVector ShotDirection = -ProjectileMovementComponent->Velocity;
    FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
    OtherActor->TakeDamage(Damage, DamageEvent, OwnerController, this);

    AShooterCharacter* OtherShooterCharacter = Cast<AShooterCharacter>(OtherActor);
    if (IsValid(OtherShooterCharacter))
    {
        OtherShooterCharacter->AddImpulseToCharacter(Hit.BoneName, Hit.Location, ProjectileMovementComponent->Velocity, ImpactMultiplier);
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("OtherShooterCharacter is not valid"));
    }

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

    DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Green, false, 20.f);

    // Destroy();
}

void ABullet::SetOwnerController(AController* TargetController)
{
    OwnerController = TargetController;
}
