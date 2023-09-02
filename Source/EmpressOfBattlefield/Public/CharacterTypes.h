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
	EAS_Unoccupied UMETA(Display = "Occupied"),
	EAS_Attacking UMETA(Display = "Attacking")
};