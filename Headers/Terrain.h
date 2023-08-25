#pragma once
#include "GameObject.h"
#include "Random.h"

struct TerrainInfo {
	~TerrainInfo() { delete snow; delete rock; delete grass; delete sand; delete water; }
	int verticesRow = 120;
	int verticesCol = 120;
	float size = 120;
	float h = 7;
	Material* snow;
	Material* rock;
	Material* grass;
	Material* sand;
	Material* water;
};

enum class Biomes {
	WATER,
	SAND,
	GRASS,
	ROCK,
	SNOW
};

class Terrain : GameObject
{
public:
	Terrain(std::string name, Renderer* renderer, vec3 pos, DescriptorPool* pool, TerrainInfo* terrainInfo) : GameObject() {
		info = terrainInfo;
		mesh = GenerateTerrain(renderer);
	};
	~Terrain() { delete info; }
private:
	Biomes GetBiome(float perlinValue);
	glm::vec4 GetTextureValue(Biomes biome);
	void CalculateNormal(std::vector<Vertex>& vertices, uint ID, uint ID1, uint ID2);
	float GetHeightInPoint(glm::vec2 pos);
	Mesh* GenerateTerrain(Renderer* renderer);


	inline float lerp(float x, float y, float t) {
		return x * (1.f - t) + y * t;
	}

	TerrainInfo* info = nullptr;
	GradientMap gradientMap;
	uint* heightMap = nullptr;
};

