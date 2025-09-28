// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPAVENTURE_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	FVector StartPoint;

	FVector EndPoint;

	bool bArePointsSet;

	UPROPERTY(EditAnywhere)
	float MoveTime;
	UPROPERTY(EditAnywhere)
	bool OwnerIsTriggerActor;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;
	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bAllTriggerActorsTriggered;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector NewStartPoint, FVector NewEndPoint);
	UFUNCTION()
	void OnPressurePlateActivated();
	UFUNCTION()
	void OnPressurePlateDeactivated();
};
