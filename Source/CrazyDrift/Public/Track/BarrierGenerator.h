// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "BarrierGenerator.generated.h"

UCLASS()
class CRAZYDRIFT_API ABarrierGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrierGenerator();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(Category = Generator, EditAnywhere, meta = (ClampMin = "1.0", UIMin = "1.0"))
	float MeshSize;

	UPROPERTY(Category = Generator, EditAnywhere)
	UStaticMesh* BarrierStaticMesh;

	UPROPERTY(Category = Generator, EditAnywhere)
	UMaterialInterface* BarrierMaterial;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Scene;

	UPROPERTY(Category = Generator, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USplineComponent* Spline;

};
