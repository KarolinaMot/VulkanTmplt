#include "../Headers/Terrain.h"

Mesh* Terrain::GenerateTerrain(Renderer* renderer)
{
    gradientMap = Random::CreateGradientMap(80, 80);
    std::vector<uint16_t> indices;
    std::vector<Vertex> vertices;
    uint vertexRowCount = info->verticesRow;
    uint vertexDepthCount = info->verticesCol;
    float heightIntensity = info->h;
    heightMap = new uint[vertexRowCount * vertexRowCount];

    static const float spacing = info->size / (float)vertexRowCount;
    glm::vec4 texValue;

    for (int x = 0; x < vertexRowCount; x++) {
        for (int z = 0; z < vertexRowCount; z++) {
            Vertex v;
            float heightValue = GetHeightInPoint(glm::vec2(x, z));
            v.pos.x = (float)x * spacing;
            if ((x > 15 && x < vertexRowCount - 15) || (z > 15 && z < vertexRowCount - 15)) {
                texValue = GetTextureValue(GetBiome(heightValue / heightIntensity));
                v.pos.y = GetBiome(heightValue / heightIntensity) < Biomes::ROCK ? heightValue : heightValue * 1.3f;
            }
            else {
                texValue = GetTextureValue(Biomes::GRASS);
                v.pos.y = heightValue / heightIntensity;
            }
            v.pos.z = (float)z * spacing;

            v.texCoord.x = ((float)z / ((float)vertexRowCount - 1)) * vertexRowCount / 3;
            v.texCoord.y = ((float)x / ((float)vertexRowCount - 1)) * vertexRowCount / 3;

            v.texMultisampling = texValue;

            vertices.push_back(v);
            heightMap[(z * vertexRowCount) + x] = v.pos.y;
        }
    }


    for (uint x = 0; x < (uint)vertexRowCount - 1; x++)
    {
        for (uint z = 0; z < (uint)vertexRowCount - 1; z++)
        {
            uint topLeft = (z * (uint)vertexRowCount) + x;
            uint topRight = topLeft + 1;
            uint bottomLeft = ((z + 1) * (uint)vertexRowCount) + x;
            uint bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);


            CalculateNormal(vertices, topLeft, bottomLeft, topRight);
            CalculateNormal(vertices, topRight, topLeft, bottomRight);
            CalculateNormal(vertices, bottomRight, topRight, bottomLeft);
            CalculateNormal(vertices, bottomLeft, topLeft, bottomRight);
        }
    }


    return new Mesh(renderer, vertices, indices);
}

Biomes Terrain::GetBiome(float perlinValue)
{
    if (perlinValue <= 0.005) return Biomes::WATER;
    if (perlinValue < 0.015f) return Biomes::SAND;
    else if (perlinValue < 0.08) return Biomes::GRASS;
    else if (perlinValue < 0.3) return Biomes::ROCK;
    else return Biomes::SNOW;
}

glm::vec4 Terrain::GetTextureValue(Biomes biome)
{
    glm::vec4 texValue = glm::vec4(0);
    switch (biome) {
    case Biomes::SNOW:
        texValue = glm::vec4(1.f, 0.f, 0.f, 0.f);
        break;
    case Biomes::ROCK:
        texValue = glm::vec4(0.f, 1.f, 0.f, 0.f);
        break;
    case Biomes::GRASS:
        texValue = glm::vec4(0.f, 0.f, 1.f, 0.f);
        break;
    case Biomes::SAND:
        texValue = glm::vec4(0.f, 0.f, 0.f, 1.f);
        break;
    case Biomes::WATER:
        texValue = glm::vec4(0);
    }
    return texValue;
}

void Terrain::CalculateNormal(std::vector<Vertex>& vertices, uint ID, uint ID1, uint ID2)
{
    glm::vec3 pos = glm::vec3(vertices[ID].pos.x, vertices[ID].pos.y, vertices[ID].pos.z);
    glm::vec3 pos1 = glm::vec3(vertices[ID1].pos.x, vertices[ID1].pos.y, vertices[ID1].pos.z);
    glm::vec3 pos2 = glm::vec3(vertices[ID2].pos.x, vertices[ID2].pos.y, vertices[ID2].pos.z);

    glm::vec3 vec1 = pos1 - pos;
    glm::vec3 vec2 = pos2 - pos;
    glm::vec3 crossPr = glm::normalize(glm::cross(vec2, vec1));

    vertices[ID].norm.x = crossPr.x;
    vertices[ID].norm.y = crossPr.y;
    vertices[ID].norm.z = crossPr.z;
}


float Terrain::GetHeightInPoint(glm::vec2 pos)
{
    return Random::PerlinWithOctaves(pos.x, pos.y, 2.f, 0.1f, 0.5f, 0.2f, 5, gradientMap) * info->h;
}
