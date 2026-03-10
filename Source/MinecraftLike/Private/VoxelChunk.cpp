#include "VoxelChunk.h"

#include "ProceduralMeshComponent.h"

AVoxelChunk::AVoxelChunk()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = Mesh;
    Mesh->bUseAsyncCooking = true;
}

void AVoxelChunk::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    GenerateFlatTerrain();
    BuildMesh();
}

int32 AVoxelChunk::Index(int32 X, int32 Y, int32 Z) const
{
    return X + Y * ChunkSizeX + Z * ChunkSizeX * ChunkSizeY;
}

bool AVoxelChunk::IsInside(int32 X, int32 Y, int32 Z) const
{
    return X >= 0 && X < ChunkSizeX
        && Y >= 0 && Y < ChunkSizeY
        && Z >= 0 && Z < ChunkSizeZ;
}

EBlockType AVoxelChunk::GetBlock(int32 X, int32 Y, int32 Z) const
{
    if (!IsInside(X, Y, Z))
    {
        return EBlockType::Air;
    }
    return Blocks[Index(X, Y, Z)];
}

void AVoxelChunk::GenerateFlatTerrain(int32 GroundHeight)
{
    Blocks.SetNumZeroed(ChunkSizeX * ChunkSizeY * ChunkSizeZ);

    for (int32 Z = 0; Z < ChunkSizeZ; ++Z)
    {
        for (int32 Y = 0; Y < ChunkSizeY; ++Y)
        {
            for (int32 X = 0; X < ChunkSizeX; ++X)
            {
                if (Z > GroundHeight)
                {
                    Blocks[Index(X, Y, Z)] = EBlockType::Air;
                }
                else if (Z == GroundHeight)
                {
                    Blocks[Index(X, Y, Z)] = EBlockType::Grass;
                }
                else if (Z > GroundHeight - 4)
                {
                    Blocks[Index(X, Y, Z)] = EBlockType::Dirt;
                }
                else
                {
                    Blocks[Index(X, Y, Z)] = EBlockType::Stone;
                }
            }
        }
    }
}

void AVoxelChunk::AddFace(
    TArray<FVector>& Vertices,
    TArray<int32>& Triangles,
    TArray<FVector>& Normals,
    TArray<FVector2D>& UVs,
    TArray<FProcMeshTangent>& Tangents,
    const FVector& Base,
    const FVector& AxisA,
    const FVector& AxisB,
    const FVector& Normal
) const
{
    const int32 StartIndex = Vertices.Num();

    Vertices.Add(Base);
    Vertices.Add(Base + AxisA);
    Vertices.Add(Base + AxisA + AxisB);
    Vertices.Add(Base + AxisB);

    Triangles.Add(StartIndex + 0);
    Triangles.Add(StartIndex + 1);
    Triangles.Add(StartIndex + 2);

    Triangles.Add(StartIndex + 0);
    Triangles.Add(StartIndex + 2);
    Triangles.Add(StartIndex + 3);

    Normals.Add(Normal);
    Normals.Add(Normal);
    Normals.Add(Normal);
    Normals.Add(Normal);

    UVs.Add(FVector2D(0.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 0.0f));
    UVs.Add(FVector2D(1.0f, 1.0f));
    UVs.Add(FVector2D(0.0f, 1.0f));

    const FVector TangentDir = AxisA.GetSafeNormal();
    Tangents.Add(FProcMeshTangent(TangentDir.X, TangentDir.Y, TangentDir.Z));
    Tangents.Add(FProcMeshTangent(TangentDir.X, TangentDir.Y, TangentDir.Z));
    Tangents.Add(FProcMeshTangent(TangentDir.X, TangentDir.Y, TangentDir.Z));
    Tangents.Add(FProcMeshTangent(TangentDir.X, TangentDir.Y, TangentDir.Z));
}

void AVoxelChunk::BuildMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> Colors;
    TArray<FProcMeshTangent> Tangents;

    for (int32 Z = 0; Z < ChunkSizeZ; ++Z)
    {
        for (int32 Y = 0; Y < ChunkSizeY; ++Y)
        {
            for (int32 X = 0; X < ChunkSizeX; ++X)
            {
                const EBlockType Block = GetBlock(X, Y, Z);
                if (Block == EBlockType::Air)
                {
                    continue;
                }

                const FVector Base(X * BlockSize, Y * BlockSize, Z * BlockSize);
                const FVector Right(BlockSize, 0.0f, 0.0f);
                const FVector Forward(0.0f, BlockSize, 0.0f);
                const FVector Up(0.0f, 0.0f, BlockSize);

                if (GetBlock(X + 1, Y, Z) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base + Right, Forward, Up, FVector::RightVector);
                }
                if (GetBlock(X - 1, Y, Z) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base, Up, Forward, FVector::LeftVector);
                }
                if (GetBlock(X, Y + 1, Z) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base + Forward, Up, Right, FVector::ForwardVector);
                }
                if (GetBlock(X, Y - 1, Z) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base, Right, Up, FVector::BackwardVector);
                }
                if (GetBlock(X, Y, Z + 1) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base + Up, Right, Forward, FVector::UpVector);
                }
                if (GetBlock(X, Y, Z - 1) == EBlockType::Air)
                {
                    AddFace(Vertices, Triangles, Normals, UVs, Tangents, Base, Forward, Right, FVector::DownVector);
                }
            }
        }
    }

    Colors.Init(FLinearColor::White, Vertices.Num());

    Mesh->ClearAllMeshSections();
    Mesh->CreateMeshSection_LinearColor(
        0,
        Vertices,
        Triangles,
        Normals,
        UVs,
        Colors,
        Tangents,
        true
    );
}
