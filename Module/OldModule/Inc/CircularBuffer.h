#define CIRCULARBUFFER_MAX_SIZE_BUFFER 500
#define circ_t  uint8_t /*!<Typ danych przechowywany w buforze*/
typedef struct CircularBufferStruct CircularBufferStruct;

struct CircularBufferStruct {
	circ_t buffer[CIRCULARBUFFER_MAX_SIZE_BUFFER];
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t(*isFull)(CircularBufferStruct* const me);
	uint32_t(*isEmpty)(CircularBufferStruct* const me);
	uint32_t(*getSize)(CircularBufferStruct* const me);
	void (*insert)(CircularBufferStruct* const me, circ_t value);
	circ_t* (*getData)(CircularBufferStruct* const me);
	void (*show)(CircularBufferStruct* const me);
	circ_t* (*getDataToDMA)(CircularBufferStruct* const me);
};



CircularBufferStruct *CircularBuffer_Create();
void CircularBuffer_Init(CircularBufferStruct* const me,
						uint32_t(*isFull)(CircularBufferStruct* const me),
						uint32_t(*isEmpty)(CircularBufferStruct* const me),
						uint32_t(*getSize)(CircularBufferStruct* const me),
						void (*insert)(CircularBufferStruct* const me, circ_t value),
						circ_t* (*getData)(CircularBufferStruct* const me),
						void (*show)(CircularBufferStruct* const me),
						circ_t* (*getDataFromDMA)(CircularBufferStruct* const me));
uint32_t CircularBuffer_isFull(CircularBufferStruct* const me);
uint32_t CircularBuffer_isEmpty(CircularBufferStruct* const me);
uint32_t CircularBuffer_getSize(CircularBufferStruct* const me);
void CircularBuffer_insert(CircularBufferStruct* const me, circ_t value);
circ_t* CircularBuffer_getData(CircularBufferStruct* const me);
void CircularBuffer_show(CircularBufferStruct* const me);
circ_t* CircularBuffer_getDataToDMA(CircularBufferStruct* const me);
