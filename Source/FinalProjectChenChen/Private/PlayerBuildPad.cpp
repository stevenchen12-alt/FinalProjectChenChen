// PlayerBuildPad.cpp


#include "PlayerBuildPad.h"

// Sets default values
APlayerBuildPad::APlayerBuildPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBuildPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerBuildPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

