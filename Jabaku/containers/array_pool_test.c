#include "array_pool_test.h"
#include "array_pool.h"

#include "test/testmacros.h"

typedef struct Item {
	int a;
} Item;
static Item array[2];

JBKArrayPool pool;

static void before() {
	JBKArrayPool_Init(&pool, array, sizeof(array) / sizeof(array[0]), sizeof(array[0]));
}

static void after() {
	JBKArrayPool_Cleanup(&pool);
}

static void add() {
	Item item = {13};
	int32_t index = JBKArrayPool_Add(&pool, &item);
	Item* res = JBKArrayPool_Get(&pool, index);
	ASSERT_EQUAL_INT(res->a, item.a);
}

static void addTwo() {
	Item item1 = { 13 };
	Item item2 = { 13 };
	int32_t index1 = JBKArrayPool_Add(&pool, &item1);
	int32_t index2 = JBKArrayPool_Add(&pool, &item2);
	Item* res1 = JBKArrayPool_Get(&pool, index1);
	Item* res2 = JBKArrayPool_Get(&pool, index2);
	ASSERT_EQUAL_INT(res1->a, item1.a);
	ASSERT_EQUAL_INT(res2->a, item2.a);
}

static void addSameTwice() {
	Item item = { 13 };
	int32_t index1 = JBKArrayPool_Add(&pool, &item);
	int32_t index2 = JBKArrayPool_Add(&pool, &item);

	ASSERT_NEQ_INT(index1, index2);

	Item* res1 = JBKArrayPool_Get(&pool, index1);
	Item* res2 = JBKArrayPool_Get(&pool, index2);
	ASSERT_EQUAL_INT(res1->a, item.a);
	ASSERT_EQUAL_INT(res2->a, item.a);
}

static void exceedCapacity() {
	Item item = { 13 };
	JBKArrayPool_Add(&pool, &item);
	JBKArrayPool_Add(&pool, &item);
	JBKArrayPool_Add(&pool, &item);

	ASSERT_FAILURE();
}

void JBKArrayList_RunTests() {
	DO_TEST(add);
	DO_TEST(addTwo);
	DO_TEST(addSameTwice);
	DO_TEST(exceedCapacity);
}