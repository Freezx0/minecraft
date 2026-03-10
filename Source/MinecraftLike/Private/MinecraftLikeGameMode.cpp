#include "MinecraftLikeGameMode.h"

#include "VoxelChunk.h"
#include "Engine/World.h"
#include "GameFramework/DefaultPawn.h"

AMinecraftLikeGameMode::AMinecraftLikeGameMode()
{
    DefaultPawnClass = ADefaultPawn::StaticClass();
}

void AMinecraftLikeGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TSubclassOf<AVoxelChunk> ClassToSpawn = ChunkClass;
    if (!ClassToSpawn)
    {
        ClassToSpawn = AVoxelChunk::StaticClass();
    }

    World->SpawnActor<AVoxelChunk>(ClassToSpawn, ChunkSpawnLocation, FRotator::ZeroRotator);
}
