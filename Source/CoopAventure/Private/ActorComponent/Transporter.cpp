#include "ActorComponent/Transporter.h"
#include "PressurePlate.h"

UTransporter::UTransporter()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	bArePointsSet = false;

	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;



}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	for (AActor *TA : TriggerActors)
	{
		APressurePlate* PressurePlateActor = Cast<APressurePlate>(TA);
		if (PressurePlateActor)
		{
			PressurePlateActor->OnActivatedDel.AddDynamic(this, &UTransporter::OnPressurePlateActivated);
			PressurePlateActor->OnDeactivatedDel.AddDynamic(this, &UTransporter::OnPressurePlateDeactivated);
		}
	}

}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* MyOwner = GetOwner();
	if (MyOwner && MyOwner->HasAuthority() && bArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;
		FVector TargetLocation = bAllTriggerActorsTriggered ? EndPoint : StartPoint;

		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporter::SetPoints(FVector NewStartPoint, FVector NewEndPoint)
{
	if (NewStartPoint.Equals(NewEndPoint)) return;

	StartPoint = NewStartPoint;
	EndPoint = NewEndPoint;
	bArePointsSet = true;
}

void UTransporter::OnPressurePlateActivated()
{
	ActivatedTriggerCount++;

	FString msg = FString::Printf(TEXT("Transporter Activated: %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, msg);

	bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();

	if (bAllTriggerActorsTriggered)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, "ALL TRIGGERS ACTIVATED!");
	}
}

void UTransporter::OnPressurePlateDeactivated()
{
	ActivatedTriggerCount--;

	FString msg = FString::Printf(TEXT("Transporter Deactivated: %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, msg);

	bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();
}

