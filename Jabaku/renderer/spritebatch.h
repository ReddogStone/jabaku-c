#ifndef JBK_SPRITE_BATCH
#define JBK_SPRITE_BATCH

#include <stdint.h>

typedef enum JBKSpriteBatch_Status {
	JBK_SPRITE_BATCH_OK,
	JBK_SPRITE_BATCH_TOO_MANY_SPRITES
} JBKSpriteBatch_Status;

typedef struct JBKNativeTexture JBKNativeTexture;
typedef struct JBKRectangle JBKRectangle;
typedef struct JBKVertexShader JBKVertexShader;
typedef struct JBKPixelShader JBKPixelShader;
typedef struct JBKVertexBuffer JBKVertexBuffer;

typedef struct JBKSpriteBatch {
	JBKNativeTexture* atlas;
	uint32_t atlasColumns;
	uint32_t atlasRows;
	JBKVertexShader* vertexShader;
	JBKPixelShader* pixelShader;
	uint32_t maxSprites;

	JBKVertexBuffer* quadBuffer;
	JBKVertexBuffer* instanceBuffer;

	uint32_t currentSpriteCount;
	void* currentMemory;
} JBKSpriteBatch;

void JBKSpriteBatch_Init(JBKSpriteBatch* inst, JBKNativeTexture* atlas, const uint32_t columns, const uint32_t rows, JBKVertexShader* vertexShader, JBKPixelShader* pixelShader, const uint32_t maxSprites);
void JBKSpriteBatch_Cleanup(JBKSpriteBatch* inst);

void JBKSpriteBatch_Begin(JBKSpriteBatch* inst);
void JBKSpriteBatch_AddSprite(JBKSpriteBatch* inst, const uint32_t atlasIndex, const JBKRectangle* destination, const uint32_t color);
void JBKSpriteBatch_End(JBKSpriteBatch* inst);

#endif // JBK_SPRITE_BATCH