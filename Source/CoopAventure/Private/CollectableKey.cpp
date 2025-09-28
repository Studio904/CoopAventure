// Fill out your copyright notice in the Description page of Project Settings.
#include "CollectableKey.h"
#include "../CoopAventureCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComp);
	Capsule->SetIsReplicated(true);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.0f);
	Capsule->SetCapsuleRadius(100.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	RotationSpeed = 100.f;
	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->bAutoActivate = false;
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//
	DOREPLIFETIME(ACollectableKey, bIsCollected);
}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		//Rotate the key
		Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

		TArray<AActor*> OverlappingActors;
		Capsule->GetOverlappingActors(OverlappingActors, ACoopAventureCharacter::StaticClass());

		if (!OverlappingActors.IsEmpty() && !bIsCollected) 
		{
			bIsCollected = true;
			OnRep_IsCollected();
		}
	}


}

void ACollectableKey::OnRep_IsCollected()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Key collected on server"));
		if (bIsCollected)
		{
			OnCollectedDel.Broadcast();
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key collected on client"));
	}
	Mesh->SetVisibility(!bIsCollected);

	CollectAudio->Play();

	if (bIsCollected && KeyHolderRef) {
		KeyHolderRef->ActivateKeyMesh();
	}
}

