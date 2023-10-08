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
	EDP_DeathRight UMETA(Display = "DeathRight"),
	EDP_DeathLeft UMETA(Display = "DeathLeft"),
	EDP_DeathFront UMETA(Display = "DeathFront"),
	EDP_DeathBackward UMETA(Display = "DeathBackward"),
};