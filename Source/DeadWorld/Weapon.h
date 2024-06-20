#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType) //struct thats blue oprint type meaningit is accessible in Unreal UI
struct FIKProperties {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimSequence* AnimPose;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Aimoffset = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform CustomOffsietTransform;

};

UCLASS(Abstract) //Abstract Class //TAKE NOTES TAKE NOTES PEOPLE // C++ SHIT RIGHT HERE
class DEADWORLD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //create ROOT of component
	class USceneComponent* Root; // set root

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") // create skeltal mesh
	class USkeletalMeshComponent* Mesh; // set mesh

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	class AFirstPlayerCharacter* CurrentOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
	FIKProperties IKProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurations")
	FTransform PlacementTransform;
};
