// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableKeyHolder.generated.h"

UCLASS()
class COOPAVENTURE_API ACollectableKeyHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectableKeyHolder();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* HolderMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* KeyMesh;

	UFUNCTION()
	void ActivateKeyMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float KeyMeshRotationSpeed;

};
