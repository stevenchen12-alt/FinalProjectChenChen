// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = StaticMesh;
	
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInterface* ABlock::GetBlockMaterial() const {
	if (StaticMesh) {
		return StaticMesh->GetMaterial(0);
	}

	return nullptr;
}

void ABlock::SetBlockMaterial(UMaterialInterface* NewMaterial) {
	if (!NewMaterial) {
		return;
	}

	if (StaticMesh) {
		StaticMesh->SetMaterial(0, NewMaterial);
	}
}

