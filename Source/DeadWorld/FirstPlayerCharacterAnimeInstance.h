#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Weapon.h"
#include "FirstPlayerCharacterAnimeInstance.generated.h"

UCLASS()
class DEADWORLD_API UFirstPlayerCharacterAnimeInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	UFirstPlayerCharacterAnimeInstance();

protected:
	// Called when the game starts or when spawned
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void CurrentWeaponChanged(class AWeapon* NewWeapon, const class AWeapon* OldWeapon);
	virtual void SetVariables(const float DeltaTime);
	virtual void CalculateWeaponSway(const float DeltaTime);

public:
	/*
	*  REFERENCES
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	class AFirstPlayerCharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	class AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anime")
	FIKProperties IKProperties;
	/*
	*  IK Variables
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FTransform CameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FTransform RelativeCameraTransform;
};
