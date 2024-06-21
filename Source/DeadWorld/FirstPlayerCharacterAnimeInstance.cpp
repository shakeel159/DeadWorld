#include "FirstPlayerCharacterAnimeInstance.h"
#include "FirstPlayerCharacter.h"
#include "Camera/CameraComponent.h" // include camera library

// Sets default values
UFirstPlayerCharacterAnimeInstance::UFirstPlayerCharacterAnimeInstance()
{

}

void UFirstPlayerCharacterAnimeInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//reference to character
	Character = Cast<AFirstPlayerCharacter>(TryGetPawnOwner());
	if (Character)
	{
		Mesh = Character->GetMesh();
		Character->CurrentWeaponChangedDelegate.AddDynamic(this, &UFirstPlayerCharacterAnimeInstance::CurrentWeaponChanged);
		CurrentWeaponChanged(Character->CurrentWeapon, nullptr);
	}
}

void UFirstPlayerCharacterAnimeInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!Character) return;

	SetVariables(DeltaTime);
	CalculateWeaponSway(DeltaTime);
}

void UFirstPlayerCharacterAnimeInstance::CurrentWeaponChanged(AWeapon* NewWeapon, const AWeapon* OldWeapon)
{
	CurrentWeapon = NewWeapon;
	if (CurrentWeapon)
	{
		IKProperties = CurrentWeapon->IKProperties;
	}
}

void UFirstPlayerCharacterAnimeInstance::SetVariables(const float DeltaTime)
{
	CameraTransform = FTransform(Character->GetBaseAimRotation(), Character->Camera->GetComponentLocation());

	const FTransform& RootOffset = Mesh->GetSocketTransform(FName("root"), RTS_Component).Inverse()* Mesh->GetSocketTransform(FName("ik_hand_root"));
	RelativeCameraTransform = CameraTransform.GetRelativeTransform(RootOffset);
}

void UFirstPlayerCharacterAnimeInstance::CalculateWeaponSway(const float DeltaTime)
{

}
