#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(Display = "Unequipped"),
	ECS_EquippedOneHandWeapon UMETA(Display = "Equipped One-Hand Weapon"),
	ECS_EquippedTwoHandWeapon UMETA(Display = "Equipped Two-Hand Weapon"),
	ECS_Dead UMETA(Display = "Dead")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(Display = "Unoccupied"),
	EAS_HitReaction UMETA(Display = "HitReaction"),
	EAS_Attacking UMETA(Display = "Attacking"),
	EAS_EquipWeapon UMETA(Display = "Equip Weapon")
};

UENUM(BlueprintType)
enum class ESlashDeathPose : uint8
{
	ESDP_Death1 UMETA(Display = "Death1"),
	ESDP_Death2 UMETA(Display = "Death2")
};

UENUM(BlueprintType)
enum class EEnemyDeathPose : uint8
{
	EEDP_DeathToRight UMETA(Display = "DeathToRight"),
	EEDP_DeathToLeft UMETA(Display = "DeathToLeft"),
	EEDP_DeathToFront UMETA(Display = "DeathToFront"),
	EEDP_DeathToBack UMETA(Display = "DeathToBack"),
}; 

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_RightToLeft UMETA(Display = "RTL"),
	EAT_UpToDown UMETA(Display = "UTD"),
	EAT_Default UMETA(Display = "Default")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(Display = "Dead"),
	EES_Patroling UMETA(Display = "Patroling"),
	EES_Chasing UMETA(Display = "Chasing"),
	EES_Attacking UMETA(Display = "Attacking"),
	EES_Engaged UMETA(Display = "Engaged"),
};