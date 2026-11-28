#include "PickupStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FinalCharacter.h"
#include "InventoryComponent.h"
#include "PickupStationMinigame.h"
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
	Trigger->SetBoxExtent(FVector(80.0f, 80.0f, 80.0f));
}

void APickupStation::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickupStation::HandleBeginOverlap);
		Trigger->OnComponentEndOverlap.AddDynamic(this, &APickupStation::HandleEndOverlap);
	}
}

void APickupStation::EndPlay(const EEndPlayReason::Type Reason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RestockTimer);
		GetWorld()->GetTimerManager().ClearTimer(CloseUITimer);
	}
	CloseMinigameUI();

	Super::EndPlay(Reason);
}

void APickupStation::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										bool bFromSweep, const FHitResult& Sweep)
{
	if (!bAvailable || ActiveWidget) return;

	AFinalCharacter* PlayerCharacter = Cast<AFinalCharacter>(OtherActor);
	if (!PlayerCharacter || !MinigameWidgetClass) return;

	if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		ActiveWidget = CreateWidget<UPickupStationMinigame>(PC, MinigameWidgetClass);
		if (ActiveWidget)
		{
			ActiveWidget->AddToViewport();
			OverlappingPlayer = PlayerCharacter;
			bMinigameResolved = false;
			PlayerCharacter->SetActivePickupStation(this);
		}
	}
}

void APickupStation::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFinalCharacter* PlayerCharacter = Cast<AFinalCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter == OverlappingPlayer)
	{
		CloseMinigameUI();
	}
}

void APickupStation::AttemptStopMinigame()
{
	if (!ActiveWidget || bMinigameResolved || !OverlappingPlayer)
	{
		return;
	}

	bMinigameResolved = true;
	const int32 BlocksToAward = ActiveWidget->StopAndEvaluate();

	if (BlocksToAward > 0 && OverlappingPlayer->InventoryComponent)
	{
		for (int32 i = 0; i < BlocksToAward; ++i)
		{
			if (!OverlappingPlayer->InventoryComponent->AddBlockByType(BlockType))
			{
				break;
			}
		}
	}

	// Put station on cooldown even if Red (0) was hit so timing has risk/reward
	bAvailable = false;
	OnAvailabilityChanged(false);

	if (RestockSeconds > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(RestockTimer, this, &APickupStation::Restock, RestockSeconds, false);
	}
	else
	{
		Restock();
	}

	// Keep the stopped bar visible briefly (0.35s) before closing
	GetWorld()->GetTimerManager().SetTimer(CloseUITimer, this, &APickupStation::CloseMinigameUI, 0.35f, false);
}

void APickupStation::CloseMinigameUI()
{
	if (OverlappingPlayer)
	{
		OverlappingPlayer->SetActivePickupStation(nullptr);
		OverlappingPlayer = nullptr;
	}

	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}
}

void APickupStation::Restock()
{
	bAvailable = true;
	OnAvailabilityChanged(true);
}