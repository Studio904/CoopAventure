// Fill out your copyright notice in the Description page of Project Settings.


#include "WinArea.h"
#include "../CoopAventureCharacter.h"

// Sets default values
AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);

	bWinCondition = false;

}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!bWinCondition)
		{
			TArray<AActor*> OverlappingActors;
			WinAreaBox->GetOverlappingActors(OverlappingActors, ACoopAventureCharacter::StaticClass());

			bWinCondition = OverlappingActors.Num() == 2;
			if (bWinCondition)
			{
				UE_LOG(LogTemp, Warning, TEXT("Win Condition Met!"));
			}	
		}
	}

}

