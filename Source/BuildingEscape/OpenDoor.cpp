// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"

UOpenDoor::UOpenDoor()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
    Super::BeginPlay();

    InitialYaw = CurrentYaw = GetOwner()->GetActorRotation().Yaw;
    OpenAngle += CurrentYaw;

    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has the door component on it, but no pressure plate set."), *GetOwner()->GetName());
    }

    ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!ActorThatOpensDoor)
    {
        UE_LOG(LogTemp, Error, TEXT("%s has the door component on it, but could not find player controller."), *GetOwner()->GetName());
    }
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
    {
        MoveDoor(DeltaTime, OpenAngle, DoorOpenSpeed);
        DoorLastOpenedTime = GetWorld()->GetTimeSeconds();
    }
    else if (GetWorld()->GetTimeSeconds() >= DoorLastOpenedTime + DoorCloseDelayInSeconds)
    {
        MoveDoor(DeltaTime, InitialYaw, DoorCloseSpeed);
    }
}

void UOpenDoor::MoveDoor(const float DeltaTime, const float Target, const float InterpSpeed)
{
    FRotator DoorRotation = GetOwner()->GetActorRotation();

    CurrentYaw = FMath::FInterpTo(CurrentYaw, Target, DeltaTime, InterpSpeed);
    DoorRotation.Yaw = CurrentYaw;

    GetOwner()->SetActorRotation(DoorRotation);
}
