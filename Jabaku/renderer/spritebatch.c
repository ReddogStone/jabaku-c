#include "spritebatch.h"

#include <SDL.h>

#include "utility/assertion.h"
#include "math/vector.h"
#include "math/rectangle.h"
#include "renderer/renderapi.h"

typedef struct {
	float x;
	float y;
	float z;
	float u;
	float v;
} Vertex;

typedef struct {
	float u;
	float v;
} TextureCoordinates;

static uint8_t g_buffer[1 * 1024 * 1024];
static const INSTANCE_DATA_SIZE = 4 * sizeof(JBKVector4)+sizeof(uint32_t)+sizeof(TextureCoordinates);

static uint8_t* setColor(uint8_t* mem, const uint32_t color) {
	mem[0] = (uint8_t)((color >> 24) & 0xFF);
	mem[1] = (uint8_t)((color >> 16) & 0xFF);
	mem[2] = (uint8_t)((color >> 8) & 0xFF);
	mem[3] = (uint8_t)((color >> 0) & 0xFF);
	return mem + 4;
}

static uint8_t* setTexCoords(uint8_t* mem, const TextureCoordinates* coords) {
	SDL_memcpy(mem, coords, sizeof(TextureCoordinates));
	return mem + sizeof(TextureCoordinates);
}

static uint8_t* setDestination(uint8_t* mem, const JBKRectangle* destination) {
	float world[] = {
		destination->width, 0.0f, 0.0f, destination->x,
		0.0f, destination->height, 0.0f, destination->y,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	SDL_memcpy(mem, world, sizeof(world));
	return mem + sizeof(world);
}

void JBKSpriteBatch_Init(JBKSpriteBatch* inst, JBKNativeTexture* atlas, const uint32_t columns, const uint32_t rows, JBKVertexShader* vertexShader, JBKPixelShader* pixelShader, const uint32_t maxSprites) {
	inst->atlas = atlas;
	inst->atlasColumns = columns;
	inst->atlasRows = rows;
	inst->vertexShader = vertexShader;
	inst->pixelShader = pixelShader;
	inst->maxSprites = maxSprites;

	Vertex quadData[] = {
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f / rows },
		{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f / columns, 1.0f / rows },
		{ 1.0f, 0.0f, 0.0f, 1.0f / columns, 1.0f / rows },
		{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f / columns, 0.0f }
	};
	inst->quadBuffer = JBKRender_CreateVertexBuffer(quadData, sizeof(quadData));

	inst->instanceBuffer = JBKRender_CreateDynamicVertexBuffer(maxSprites * INSTANCE_DATA_SIZE);

	inst->currentSpriteCount = 0;
	inst->currentMemory = 0;
}

void JBKSpriteBatch_Cleanup(JBKSpriteBatch* inst) {
	JBKRender_DestroyVertexBuffer(inst->quadBuffer);
	JBKRender_DestroyVertexBuffer(inst->instanceBuffer);
}

void JBKSpriteBatch_Begin(JBKSpriteBatch* inst) {
	JBKRender_SetEffect(inst->vertexShader, inst->pixelShader);
	JBKRender_SetTextures(&inst->atlas, 1);

	JBKRender_SetDefaultSampling(1);

	inst->currentMemory = JBKRender_LockVertexBuffer(inst->instanceBuffer);
	inst->currentSpriteCount = 0;
}

void JBKSpriteBatch_AddSprite(JBKSpriteBatch* inst, const uint32_t atlasIndex, const JBKRectangle* destination, const uint32_t color) {
	ASSERT(inst->currentSpriteCount < inst->maxSprites, "Too many sprites (%d)", inst->currentSpriteCount);
	
	void* mem = inst->currentMemory;
	mem = setDestination(mem, destination);
	mem = setColor(mem, color);

	TextureCoordinates coords = { (float)(atlasIndex % inst->atlasColumns) / inst->atlasColumns, (float)(atlasIndex / inst->atlasColumns) / inst->atlasRows };
	mem = setTexCoords(mem, &coords);

	inst->currentMemory = mem;
	inst->currentSpriteCount++;
}

void JBKSpriteBatch_End(JBKSpriteBatch* inst) {
	JBKRender_UnlockVertexBuffer(inst->instanceBuffer);

	JBKVertexBuffer* buffers[] = { inst->quadBuffer, inst->instanceBuffer };
	uint32_t strides[] = { sizeof(Vertex), INSTANCE_DATA_SIZE };
	uint32_t offsets[] = { 0, 0 };
	JBKRender_SetVertexBuffers(buffers, 2, strides, offsets);

	JBKRender_DrawInstancedPrimitives(JBK_PRIMITIVE_TRIANGLE_LIST, 6, 0, inst->currentSpriteCount);
}
