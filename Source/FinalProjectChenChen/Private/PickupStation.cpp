// PickupStation.cpp
#include "PickupStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FinalCharacter.h"
#include "InventoryComponent.h"
#include "TimerManager.h"

APickupStation::APickupStation()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Trigger->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
}

void APickupStation::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickupStation::HandleOverlap);
	}
}

void APickupStation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								   bool bFromSweep, const FHitResult& Sweep)
{
	if (!bAvailable) return;

	AFinalCharacter* PlayerCharacter = Cast<AFinalCharacter>(OtherActor);
	if (!PlayerCharacter || !PlayerCharacter->InventoryComponent) return;

	int32 AddedCount = 0;
	for (int32 i = 0; i < AmountPerPickup; ++i)
	{
		if (PlayerCharacter->InventoryComponent->AddBlockByType(BlockType))
		{
			AddedCount++;
		}
		else
		{
			break; // Stop if the slot hit Capacity
		}
	}

	if (AddedCount > 0)
	{
		bAvailable = false;

		if (RestockSeconds > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(RestockTimer, this, &APickupStation::Restock, RestockSeconds, false);
		}
		else
		{
			Restock();
		}
	}
}

void APickupStation::Restock()
{
	bAvailable = true;
}