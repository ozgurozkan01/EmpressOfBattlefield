#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(Display = "Unequipped"),
	ECS_EquippedOneHandWeapon UMETA(Display = "Equipped One-Hand Weapon"),
	ECS_EquippedTwoHandWeapon UMETA(Display = "Equipped Two-Hand Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(Display = "Unoccupied"),
	EAS_Attacking UMETA(Display = "Attacking"),
	EAS_EquipWeapon UMETA(Display = "Equip Weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(Display = "Alive"),
	EDP_Death1 UMETA(Display = "Death1"),
	EDP_Death2 UMETA(Display = "Death2"),
	EDP_Death3 UMETA(Display = "Death3"),
	EDP_Death4 UMETA(Display = "Death4"),
};