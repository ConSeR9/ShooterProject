// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    /*Mesh_3P = GetMesh();*/

    //// Create a CameraComponent	
    //Camera_1P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_1P"));
    //Camera_1P->SetupAttachment(GetCapsuleComponent());
    //Camera_1P->SetRelativeLocation(FVector(0, 0, 64.f)); // Position the camera
    //Camera_1P->bUsePawnControlRotation = true;

    //// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    //Mesh_1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_1P"));
    //Mesh_1P->SetOnlyOwnerSee(true);				// Set so only owner can see mesh
    //Mesh_1P->SetupAttachment(Camera_1P);	// Attach mesh to FirstPersonCameraComponent
    //Mesh_1P->bCastDynamicShadow = false;			// Disallow mesh to cast dynamic shadows
    //Mesh_1P->CastShadow = false;				// Disallow mesh to cast other shadows

    bIsMovingForward = false;
    bIsSprinting = false;
    bCanCrouch = true;
    bIsCrouching = false;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    HP = MaxHP;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("CurrentHP: %f"), HP));
	
    //Weapon_1P = GetWorld()->SpawnActor<AGun>(WeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));
    //Weapon_1P->AttachToComponent(Mesh_1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint_r"));
    //Weapon_1P->SetOwner(this);
    //WeaponMesh_1P = Weapon_1P->FindComponentByClass<UStaticMeshComponent>();
    //WeaponMesh_1P->SetCastShadow(false);
    //WeaponMesh_1P->SetOnlyOwnerSee(true);
    //Weapon_3P = GetWorld()->SpawnActor<AGun>(WeaponClass, FVector(0, 0, 0), FRotator(0, -180, 0));
    //Weapon_3P->AttachToComponent(Mesh_3P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint_r"));
    //Weapon_3P->SetOwner(this);
    //WeaponMesh_3P = Weapon_3P->FindComponentByClass<UStaticMeshComponent>();
    //WeaponMesh_3P->SetCastShadow(true);
    //WeaponMesh_3P->SetOwnerNoSee(true);
    
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
    //PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::ControllerPitchInput);
    //PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::ControllerYawInput);
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
    //PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CrouchToggle);
    //PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AShooterCharacter::TrySprint);
    //PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AShooterCharacter::StopTrySprint);
    //PlayerInputComponent->BindAction(TEXT("IsMovingForward"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CheckMovingForward);
    //PlayerInputComponent->BindAction(TEXT("IsMovingForward"), EInputEvent::IE_Released, this, &AShooterCharacter::CheckNotMovingForward);
    //PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartFire);
    //PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AShooterCharacter::StopFire);
    //PlayerInputComponent->BindAction(TEXT("Keyboard_B"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchFireMode);
}

//---------------------------------------------------------------------------------------
// Take damage
//---------------------------------------------------------------------------------------
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    DamageToApply = FMath::Min(HP, DamageToApply);
    HP -= DamageToApply;
    UE_LOG(LogTemp, Warning, TEXT("HP left: %f"), HP);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("CurrentHP: %f"), HP));

    if (IsDead())
    {
        AController* ThisController = this->GetController();
        if (!IsValid(ThisController))
        {
            UE_LOG(LogTemp, Warning, TEXT("ShooterCharacter::ThisController is not valid"));
            return 0;
        }

        if (ThisController->IsPlayerController()) // if is player
        {
            DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            DeadCamera();
            UE_LOG(LogTemp, Warning, TEXT("Player Dead!"));
        }
        else // if is AI
        {
            //DetachFromControllerPendingDestroy();
            UE_LOG(LogTemp, Warning, TEXT("Enemy Dead!"));
        }

        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Dead();
        UE_LOG(LogTemp, Error, TEXT("Call Dead"));

        AShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
        if (IsValid(GameMode))
        {
            GameMode->PawnKilled(this);
        }
    }

    return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
    return HP <= 0;
}

float AShooterCharacter::GetHP() const
{
    return HP;
}

float AShooterCharacter::GetHPPercent() const
{
    return HP / MaxHP;
}

//---------------------------------------------------------------------------------------
// Moving mechanism
//---------------------------------------------------------------------------------------

void AShooterCharacter::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
    AddMovementInput(GetActorRightVector() * AxisValue);
}

//---------------------------------------------------------------------------------------
// Moving mechanism
//---------------------------------------------------------------------------------------

