// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);

    TD_SetCanFire.BindUFunction(this, FName("SetCanFire"), true);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
    float GunRefireDelay = 60.f / RPM;

    if (!bCanFire)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Cannot fire"));
        return;
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("Trigger pulled"));
        if (bFullAuto)
        {
            FireShot();
            if (AmmoCount > 0) // only when has ammo can refire
            {
                GetWorldTimerManager().SetTimer(TH_GunRefire, this, &AGun::FireShot, GunRefireDelay, true, GunRefireDelay);
            }
        }
        else
        {
            FireShot();
            SetCanFire(false);
            GetWorldTimerManager().SetTimer(TH_GunRefire, TD_SetCanFire, GunRefireDelay, false);
        }
    }
}

void AGun::ReleaseTrigger()
{
    if (bCanFire)
    {
        GetWorldTimerManager().ClearTimer(TH_GunRefire);
    }

    bIsStillFiring = false;
}

void AGun::SwitchFireMode()
{
    if (bFullAuto)
    {
        bFullAuto = false;
    }
    else
    {
        bFullAuto = true;
    }
}

// Return the amount of ammo that you reloaded
int32 AGun::Reload()
{
    AShooterCharacter* OwnerSC = Cast<AShooterCharacter>(GetOwner());
    if (!IsValid(OwnerSC))
    {
        return 0;
    }

    int32 idx = 0;
    for (int32 i = 0; i < OwnerSC->CarriedAmmo.Num(); ++i)
    {
        if (OwnerSC->CarriedAmmo[i].ProjectileType == ProjectileType)
        {
            idx = i;
        }
    }

    int32 ReloadAmount = 0;
    if (AmmoCount > 0)
    {
        ReloadAmount = MaxMagAmmoCount - AmmoCount + 1;
        if (OwnerSC->CarriedAmmo[idx].CarriedAmmoCount - ReloadAmount < 0)
        {
            ReloadAmount = OwnerSC->CarriedAmmo[idx].CarriedAmmoCount;
        }
        AmmoCount += ReloadAmount;
    }
    else
    {
        ReloadAmount = MaxMagAmmoCount - AmmoCount;
        if (OwnerSC->CarriedAmmo[idx].CarriedAmmoCount - ReloadAmount < 0)
        {
            ReloadAmount = OwnerSC->CarriedAmmo[idx].CarriedAmmoCount;
        }
        AmmoCount += ReloadAmount;
    }

    return ReloadAmount;
}

UStaticMeshComponent* AGun::GetMesh()
{
    return Mesh;
}

void AGun::FireShot()
{
    AShooterCharacter* OwnerShooterCharacter = Cast<AShooterCharacter>(GetOwner());
    if (!IsValid(OwnerShooterCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not Valid"));
        return;
    }

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No bullet left."));

        UGameplayStatics::SpawnSoundAttached(DryFireSound, Mesh, TEXT("Center_Socket"));
        bIsStillFiring = false; // if there is no ammo, let player recover from recoil
        OwnerShooterCharacter->SetIsStillFiring(bIsStillFiring);
        GetWorldTimerManager().ClearTimer(TH_GunRefire);
        // OwnerShooterCharacter->AddRecoil(FVector2D(0, 0), RecoilTime, RecoveryTime);
        return;
    }
    else
    {
        if (bFullAuto)
        {
            bIsStillFiring = true;
        }
        else
        {
            bIsStillFiring = false;
        }
    }

    AmmoCount -= 1;

    //UE_LOG(LogTemp, Warning, TEXT("Shot fired"));
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle_Socket"));
    UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("Muzzle_Socket"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    FVector MuzzleLocation = Mesh->GetSocketLocation(TEXT("Muzzle_Socket"));
    FRotator MuzzleRotation = Mesh->GetSocketRotation(TEXT("Muzzle_Socket"));

    UWorld* World = GetWorld();
    // Spawn the bullet at the muzzle.
    ABullet* Bullet = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
    if (Bullet)
    {
        // Set the projectile's initial trajectory.
        FVector LaunchDirection = MuzzleRotation.Vector();
        Bullet->FireInDirection(LaunchDirection);
        Bullet->SetOwnerController(GetOwnerController());
        Bullet->Damage += Damage;
    }

    int32 NumOfPatterns = RecoilPatterns.Num();
    if (CurrentRecoilPattern_Idx >= RecoilPatterns.Num()) // if the pattern exceeds all patterns, replay the pattern
    {
        CurrentRecoilPattern_Idx = 10;
    }

    FVector2D Recoil = RecoilPatterns[CurrentRecoilPattern_Idx];
    int32 Seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
    FMath::RandInit(Seed);

    Recoil.X += FMath::RandRange(-(Recoil.X * XRecoilDeviation), Recoil.X * XRecoilDeviation);
    Recoil.Y += FMath::RandRange(-(Recoil.Y * YRecoilDeviation), Recoil.Y * YRecoilDeviation);

    OwnerShooterCharacter->AddRecoil(Recoil, RecoilTime, RecoveryTime);
    //UE_LOG(LogTemp, Warning, TEXT("Recoil_X: %f, Recoil_Y: %f"), Recoil.X, Recoil.Y);

    CurrentBulletNum += 1;
    CurrentRecoilPattern_Idx += 1;
}

//---------------------------------------Shoot scan method----------------------------------------
// Trace the gunshot, return true if hit something
//bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
//{
//    AController* OwnerController = GetOwnerController();
//    if (!IsValid(OwnerController))
//        return false;
//
//    FVector MuzzleLocation = Mesh->GetSocketLocation(TEXT("MuzzleFlashSocket"));
//    FRotator MuzzleRotation = Mesh->GetSocketRotation(TEXT("MuzzleFlashSocket"));
//
//    ShotDirection = -MuzzleRotation.Vector();
//    FVector LineTraceEnd = MuzzleLocation + MuzzleRotation.Vector() * MaxRange;
//
//    FCollisionQueryParams Params;
//    Params.AddIgnoredActor(this);
//    Params.AddIgnoredActor(GetOwner());
//    return GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, LineTraceEnd, ECollisionChannel::ECC_GameTraceChannel1);
//}
//---------------------------------------Shoot scan method----------------------------------------

bool AGun::GetCanFire()
{
    return bCanFire;
}

void AGun::SetCanFire(bool bSetCanFire)
{
    bCanFire = bSetCanFire;
}

AController* AGun::GetOwnerController() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!IsValid(OwnerPawn))
        return nullptr;

    return OwnerPawn->GetController();
}
