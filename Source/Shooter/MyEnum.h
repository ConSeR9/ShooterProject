#include "MyEnum.generated.h"

#pragma once

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
    RIFLE = 0 UMETA(DisplayName = "Rifle"),
    SMG = 1 UMETA(DisplayName = "SMG")
};

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
    SLOT1 = 0 UMETA(DisplayName = "Slot_1"),
    SLOT2 = 1 UMETA(DisplayName = "Slot_2")
};