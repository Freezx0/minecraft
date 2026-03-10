#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinecraftLikeGameMode.generated.h"

class AVoxelChunk;

UCLASS()
class MINECRAFTLIKE_API AMinecraftLikeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMinecraftLikeGameMode();

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, Category = "Voxel")
    TSubclassOf<AVoxelChunk> ChunkClass;

    UPROPERTY(EditAnywhere, Category = "Voxel")
    FVector ChunkSpawnLocation = FVector::ZeroVector;
};
