#include "Weapon.h"

// Sets default values
AWeapon::AWeapon() //Contructor
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true); // NO IDEA WHAT THIS IS

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(!CurrentOwner)
		Mesh->SetVisibility(false); // NO IDEA WHAT THIS IS 
}

