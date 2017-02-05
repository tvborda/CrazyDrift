// Fill out your copyright notice in the Description page of Project Settings.

#include "CrazyDrift.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "BarrierGenerator.h"


// Sets default values
ABarrierGenerator::ABarrierGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshSize = 1.0f;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetMobility(EComponentMobility::Static); 
	RootComponent = Scene;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetMobility(EComponentMobility::Static);
	Spline->SetupAttachment(Scene);
}

void ABarrierGenerator::OnConstruction(const FTransform& Transform)
{
	float Remainder;
	uint32 NumberOfMeshes = UKismetMathLibrary::FMod(Spline->GetSplineLength(), MeshSize, Remainder);
	for (uint32 i = 0; i < NumberOfMeshes; i++)
	{
		UStaticMeshComponent* StaticMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());

		StaticMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		StaticMesh->SetMobility(EComponentMobility::Static);
		StaticMesh->SetupAttachment(Spline);

		StaticMesh->bCastDynamicShadow = true;
		StaticMesh->bCastStaticShadow = true;
		StaticMesh->SetStaticMesh(BarrierStaticMesh);
		StaticMesh->SetMaterial(0, BarrierMaterial);
		
		FVector BarrierLocation = Spline->GetLocationAtDistanceAlongSpline(i * MeshSize, ESplineCoordinateSpace::Local);
		FRotator BarrierRotator = Spline->GetRotationAtDistanceAlongSpline(i * MeshSize, ESplineCoordinateSpace::Local);
		StaticMesh->SetRelativeLocationAndRotation(BarrierLocation, BarrierRotator);
	}
	RegisterAllComponents();
}

