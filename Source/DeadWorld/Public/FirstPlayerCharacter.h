// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentWeaponChangedDelegate, class AWeapon*, CurrentWeapon, const class AWeapon*, OldWeapon);

UCLASS()
class DEADWORLD_API AFirstPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //set if Visable in unreal
	class UCameraComponent* Camera; // call library for Camera


protected:
	// the weapon classes spawned by defualt
	UPROPERTY(EditDefaultsOnly, Category = "Configurations")
	TArray<TSubclassOf<class AWeapon>>DefualtWeapons; // Create array to list all weapons aquired


public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, Category = "State")
	TArray<class AWeapon*> Weapons;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon, Category = "State")
	class AWeapon* CurrentWeapon;

	// Called when ever current weapon is changed
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FCurrentWeaponChangedDelegate CurrentWeaponChangedDelegate;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State")
	int32 CurrentIndex = 0; // keep track of our iNDex(current weapon)

	UFUNCTION(BlueprintCallable, category = "Character")
	virtual void EquipWeapon(const int32 Index);

protected:
	UFUNCTION()
	virtual void OnRep_CurrentWeapon(const class AWeapon* OldWeapon);

	UFUNCTION(Server, Reliable)
	void Server_SetCurrentWeapon(class AWeapon* Weapon);
	virtual void Server_SetCurrentWeapon_Implementation(class AWeapon* NewWeapon);


protected:

	virtual void NextWeapon();
	virtual void LastWeapon();
	// not sure why one is virtual and other is void but probablt take note that virtual mthods are ones using Action mapping while void methods are using axis mapping for input
	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void LookUp(const float Value);
	void LookRight(const float Value);


};
