// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"
#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "ROSIntegration/Public/std_msgs/String.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogClass, Log, TEXT("***** ATestActor"))
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	UE_LOG(LogClass, Log, TEXT("***** BeginPlay"))
	Super::BeginPlay();
	
	// Initialize a topic
	UTopic *ExampleTopic = NewObject<UTopic>(UTopic::StaticClass());
	UROSIntegrationGameInstance* rosinst = Cast<UROSIntegrationGameInstance>(GetGameInstance());
	ExampleTopic->Init(rosinst->ROSIntegrationCore, TEXT("/fries"), TEXT("std_msgs/String"));
	
	// (Optional) Advertise the topic
	ExampleTopic->Advertise();
	
	// Publish a string to the topic
	TSharedPtr<ROSMessages::std_msgs::String> StringMessage(new ROSMessages::std_msgs::String("This is an example"));
	ExampleTopic->Publish(StringMessage);

	// Create a std::function callback object
	std::function<void(TSharedPtr<FROSBaseMsg>)> SubscribeCallback = [](TSharedPtr<FROSBaseMsg> msg) -> void
	{
		auto Concrete = StaticCastSharedPtr<ROSMessages::std_msgs::String>(msg);
		UE_LOG(LogClass, Log, TEXT("***** SubscribeCallback: %s"), Concrete.IsValid())
			if (Concrete.IsValid())
			{
				UE_LOG(LogTemp, Log, TEXT("***** Incoming string was: %s"), (*(Concrete->_Data)));
			}
		return;
	};
	
	// Subscribe to the topic
	ExampleTopic->Subscribe(SubscribeCallback);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




