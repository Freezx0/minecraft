#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelChunk.generated.h"

class UProceduralMeshComponent;

UENUM(BlueprintType)
enum class EBlockType : uint8
{
    Air = 0,
    Dirt,
    Grass,
    Stone
};

UCLASS()
class MINECRAFTLIKE_API AVoxelChunk : public AActor
{
    GENERATED_BODY()

public:
    AVoxelChunk();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voxel")
    TObjectPtr<UProceduralMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voxel")
    int32 ChunkSizeX = 16;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voxel")
    int32 ChunkSizeY = 16;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voxel")
    int32 ChunkSizeZ = 64;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voxel")
    float BlockSize = 100.0f;

private:
    TArray<EBlockType> Blocks;

    int32 Index(int32 X, int32 Y, int32 Z) const;
    bool IsInside(int32 X, int32 Y, int32 Z) const;
    EBlockType GetBlock(int32 X, int32 Y, int32 Z) const;

    void GenerateFlatTerrain(int32 GroundHeight = 20);
    void BuildMesh();

    void AddFace(
        TArray<FVector>& Vertices,
        TArray<int32>& Triangles,
        TArray<FVector>& Normals,
        TArray<FVector2D>& UVs,
        TArray<FProcMeshTangent>& Tangents,
        const FVector& Base,
        const FVector& AxisA,
        const FVector& AxisB,
        const FVector& Normal
    ) const;
};
