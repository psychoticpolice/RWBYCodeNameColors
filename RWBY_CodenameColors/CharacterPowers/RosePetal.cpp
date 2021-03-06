// Authour: Antonio Morales.

#include "RWBY_CodenameColors.h"
#include "RosePetal.h"
#include "RWBY_CodenameColorsCharacter.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"

// Sets default values
ARosePetal::ARosePetal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PetalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PetalMesh;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	Trigger->SetSphereRadius(400);
	Trigger->AttachTo(PetalMesh);

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Box"));
	HitBox->AttachTo(PetalMesh);


	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARosePetal::OnOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ARosePetal::OnOverlapEnd);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ARosePetal::RoseOverlap);
}

// Called when the game starts or when spawned
void ARosePetal::BeginPlay()
{
	Super::BeginPlay();

	//SetPetalMaterial("Fire");
	//

	


}

// Called every frame
void ARosePetal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	
	if (!ActorsToFollow.Num() == 0) {

		FVector Character = ActorsToFollow[0]->GetActorLocation();

		//PetalMesh->MoveComponent(Character, FQuat(), true, false, false, false);

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;

		FVector Direction = Character - PetalMesh->GetComponentLocation();

		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		UKismetSystemLibrary::MoveComponentTo(PetalMesh, Character, Rot, false, false, .35f, EMoveComponentAction::Move, LatentInfo);
	}
	//Force
	//


	if (Instigator != nullptr) {

		SetPetalMaterial(MaterialChoice);
	}

	
}

void ARosePetal::SetMesh(class UStaticMeshComponent* NewMesh) {
	PetalMesh = NewMesh;
}

void ARosePetal::OnOverlap(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult & SweepResult){
	/*
	if (OtherActor == ActorToIgnore) {
		ActorsToFollow.Remove(ActorToIgnore);
		return;
	}


	ARWBY_CodenameColorsCharacter* OverlapCharacter = Cast<ARWBY_CodenameColorsCharacter>(OtherActor);

	if (OverlapCharacter) {

		ActorsToFollow.Add(OverlapCharacter);

		FTimerHandle Delay;
		GetWorldTimerManager().SetTimer(Delay, this, &ARosePetal::DestroySelf, 5.f);

	}
	*/
}

void ARosePetal::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	ARWBY_CodenameColorsCharacter* OverlapCharacter = Cast<ARWBY_CodenameColorsCharacter>(OtherActor);

	if (OverlapCharacter) {
		ActorsToFollow.Remove(OverlapCharacter);

	}

}


void ARosePetal::RoseOverlap(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult & SweepResult){


	ARWBY_CodenameColorsCharacter* HitCharacter = Cast<ARWBY_CodenameColorsCharacter>(OtherActor);
	
	ARWBY_CodenameColorsCharacter* Insta = Cast<ARWBY_CodenameColorsCharacter>(GetInstigator());
	
	if(HitCharacter){

		if(Insta){

			if(ActorsToFollow.IsValidIndex(0)){

				if (HitCharacter == ActorsToFollow[0]) {

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("RWBY HIT"));

					HitCharacter->DetermineElementalDamage(Insta, 0);

					HitCharacter->GetCharacterMovement()->AddImpulse(PetalMesh->GetForwardVector() * 50000, false);

					HitCharacter->RemoveStateWithDelay();

					DestroySelf();
				}
			}
		}
	}
}

void ARosePetal::SetPetalMaterial(FString Material){

	
	if (Material == "Fire") {
		PetalMesh->SetMaterial(0, DustTypes[0]);
	}
	else if (Material == "Electricty") {
		PetalMesh->SetMaterial(0, DustTypes[1]);
	}
	else if (Material == "Water") {
		PetalMesh->SetMaterial(0, DustTypes[2]);
	}
	else if (Material == "Ground") {
		PetalMesh->SetMaterial(0, DustTypes[3]);
	}
	else if (Material == "Ice") {
		PetalMesh->SetMaterial(0, DustTypes[4]);
	}
	
	
	/*
	//switch(GetInstigator()->GetMaterial)

	if (Instigator != nullptr) {

		ARWBY_CodenameColorsCharacter* Testing = Cast<ARWBY_CodenameColorsCharacter>(GetInstigator());

		if (Testing) {

			switch (Testing->PoweredUpState) {

			case(EPoweredUpState::FiredUp) :
				PetalMesh->SetMaterial(0, DustTypes[0]);
				break;

			case(EPoweredUpState::GravityUp) :
				PetalMesh->SetMaterial(0, DustTypes[3]);
				break;

			case(EPoweredUpState::IcedUp) :
				PetalMesh->SetMaterial(0, DustTypes[4]);
				break;

			case(EPoweredUpState::ShockedUp) :
				PetalMesh->SetMaterial(0, DustTypes[1]);
				break;

			case(EPoweredUpState::WateredUp) :
				PetalMesh->SetMaterial(0, DustTypes[2]);
				break;

			}

		}
	}
	*/
}


void ARosePetal::DestroySelf() {

	Destroy();
}

void ARosePetal::SetActorToIgore(ARWBY_CodenameColorsCharacter* IgnoreThis){

	ActorToIgnore = IgnoreThis;
}

void ARosePetal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARosePetal, ActorsToFollow);
	DOREPLIFETIME(ARosePetal, MaterialChoice);
	

}