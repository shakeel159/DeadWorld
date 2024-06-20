// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPlayerCharacter.h"
#include "../Weapon.h"
#include "Camera/CameraComponent.h" // include camera library
#include <Net/UnrealNetwork.h>

// Sets default values
AFirstPlayerCharacter::AFirstPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camerea")); // create Camera Component
	Camera->bUsePawnControlRotation = true; // set it to face foward of Pawn position
	Camera->SetupAttachment(GetMesh(), FName("head")); // set camera position to location of socket names "head"
}

// Called when the game starts or when spawned
void AFirstPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//SERVER SHIT
	// ON BEGIN check for server
	if (HasAuthority())
	{
		for (const TSubclassOf<AWeapon>& WeaponClass : DefualtWeapons) // iterate thru weapons
		{
			if (!WeaponClass) continue; // if weapons is not valid skip
			FActorSpawnParameters Params; // call in some parameters 
			Params.Owner = this; // set owner param to us
			AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Params); // get reference to weapon to spawn // Spawning here ??
			const int32 Index = Weapons.Add(SpawnedWeapon); // add aquired weapons to list 
			if (Index == CurrentIndex)
			{
				// current weapon(In firstinstance spawned weapond) to new weapon
				CurrentWeapon = SpawnedWeapon;
				OnRep_CurrentWeapon(nullptr);
			}
		}
	}
}
// Called to bind functionality to input
void AFirstPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get Input names from names set in Input mapping in settings
	// Get and Set method names from .h varient of file(method names you set)
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AFirstPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AFirstPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AFirstPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &AFirstPlayerCharacter::LookRight);

	PlayerInputComponent->BindAction(FName("NextWeapon"), IE_Pressed, this, &AFirstPlayerCharacter::NextWeapon);
	PlayerInputComponent->BindAction(FName("LastWeapon"), IE_Pressed, this, &AFirstPlayerCharacter::LastWeapon);

}


void AFirstPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFirstPlayerCharacter, Weapons, COND_None);
	DOREPLIFETIME_CONDITION(AFirstPlayerCharacter, CurrentWeapon, COND_None);
}


void AFirstPlayerCharacter::OnRep_CurrentWeapon(const AWeapon* OldWeapon)
{
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->CurrentOwner)
		{
			CurrentWeapon->SetActorTransform(GetMesh()->GetSocketTransform(FName("Weapon_R")), false, nullptr, ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Weapon_R"));

			CurrentWeapon->CurrentOwner = this;
		}


		CurrentWeapon->Mesh->SetVisibility(true);
	}

	if (OldWeapon)
	{
		OldWeapon->Mesh->SetVisibility(false);
	}
}

void AFirstPlayerCharacter::EquipWeapon(const int32 Index)
{
	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return; // check if weapon is valid

	if (IsLocallyControlled()) //if local swap wapons immedietly// i think that means first do the evnt in local mechine to then copy on to other mechines(THIS IS A ONLINE FPS TUTORIAL)
	{
		CurrentIndex = Index;

		const AWeapon* OldWeapon = CurrentWeapon;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(OldWeapon);
	}
	else if (!HasAuthority())
	{
		Server_SetCurrentWeapon(Weapons[Index]);
	}

}

void AFirstPlayerCharacter::Server_SetCurrentWeapon_Implementation(AWeapon* NewWeapon)
{
	const AWeapon* OldWeapon = CurrentWeapon;
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(OldWeapon);
}

void AFirstPlayerCharacter::NextWeapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex + 1) ? CurrentIndex + 1 : 0;
	EquipWeapon(Index);
}

void AFirstPlayerCharacter::LastWeapon()
{

	const int32 Index = Weapons.IsValidIndex(CurrentIndex - 1) ? CurrentIndex - 1 : Weapons.Num() - 1;
	EquipWeapon(Index);
}

void AFirstPlayerCharacter::MoveForward(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X); //use nothing but yaw value and control rotation to get vector of diraction
	AddMovementInput(Direction, Value);
}

void AFirstPlayerCharacter::MoveRight(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFirstPlayerCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void AFirstPlayerCharacter::LookRight(const float Value)
{
	AddControllerYawInput(Value);
}

