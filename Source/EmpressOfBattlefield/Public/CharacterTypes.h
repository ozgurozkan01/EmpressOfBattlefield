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
	EDP_DeathToRight UMETA(Display = "DeathToRight"),
	EDP_DeathToLeft UMETA(Display = "DeathToLeft"),
	EDP_DeathToFront UMETA(Display = "DeathToFront"),
	EDP_DeathToBack UMETA(Display = "DeathToBack"),
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_RightToLeft UMETA(Display = "RTL"),
	EAT_UpToDown UMETA(Display = "UTD"),
	EAT_Default UMETA(Display = "Default")
};