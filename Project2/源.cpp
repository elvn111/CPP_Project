#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define NODE_POOL_SIZE   20   /* 节点池容量 */
#define PRIORITY_COUNT   10    /* 优先级数量，合法范围 0 ~ PRIORITY_COUNT-1 */
#define TASK_BLOCK_NUM   32
#define TASK_BLOCK_SIZE  64
#define TASK_HEAP_SIZE (TASK_BLOCK_NUM * TASK_BLOCK_SIZE)

typedef enum {
    TASK_STATE_READY = 0,
    TASK_STATE_RUNNING = 1,
    TASK_STATE_BLOCKED = 2,
    TASK_STATE_SUSPENDED = 3
} TaskState;


/* 链表节点：内存来自静态数组 node_pool[] */
typedef struct TaskControlBlock {
    struct TaskControlBlock* prev;     /* 前驱节点 */
    struct TaskControlBlock* next;     /* 后继节点 */
    uint8_t priority;          /* 所属优先级（分配时写入，释放时据此定位队列） */
    uint8_t state;              
    uint8_t task_id;
    uint32_t size;
    uint32_t* stack_base;
    uint32_t* sp;              // 当前栈指针
    uint32_t* stack_top;   /* 栈底（分配时记录） */
} TaskControlBlock;

/* 节点分配状态 */
typedef enum {
    STATE_FREE = 0,   /* 空闲：可分配 */
    STATE_BUSY = 1    /* 占用：已入队 */
} State;



typedef void(*TaskFunction)(void* point);

/* ---- 节点内存池 ---- */
/* 编译期静态分配，运行期零堆内存申请；静态存储期默认零初始化 */
static TaskControlBlock  node_pool[NODE_POOL_SIZE];
/* 节点状态表，与 node_pool[] 按下标一一对应 */
static State node_pool_state[NODE_POOL_SIZE];

static uint32_t task_heap[TASK_BLOCK_NUM][TASK_BLOCK_SIZE];
static State task_block_state[TASK_BLOCK_NUM];

/* ---- 按优先级分链的就绪队列 ---- */
static TaskControlBlock* ready_head[PRIORITY_COUNT];   /* 各优先级队列头指针 */
static TaskControlBlock* ready_tail[PRIORITY_COUNT];   /* 各优先级队列尾指针 */
static uint16_t  ready_count[PRIORITY_COUNT];  /* 各优先级队列节点计数 */
static uint16_t  ready_total = 0;              /* 全部队列节点总数 */
uint16_t Task_ID = 0;

uint8_t Heap_Allocate(uint16_t size,uint32_t * *stack_base,uint32_t * *stack_top){
    if (size == 0)return 0;
    int num = (size + TASK_BLOCK_SIZE - 1) / TASK_BLOCK_SIZE;
    int count = 0;
    int block_head;
    uint32_t* addr = 0;
    for (int i = 0;i < TASK_BLOCK_NUM;i++){
        if (task_block_state[i] == STATE_BUSY){
            count = 0;
        }
        else if (task_block_state[i] == STATE_FREE){
            if (count == 0){
                addr = (uint32_t*)&task_heap[i][0];
                block_head = i;
            }
            count++;
            if (count == num){
                for (int j = 0;j < num;j++){
                    task_block_state[i-j] = STATE_BUSY;
                }
                *stack_top = (uint32_t*)&task_heap[i][0];
                *stack_base = addr;
                return 1;
            }
        }
    }
    return 0;
}

void Heap_Release(uint32_t *stack_base, uint32_t* stack_top)
{
    uint32_t byte_offset;
    uint32_t block_index;
    uint32_t block_count;
    uint32_t size;
    uint32_t i;

    /* 参数校验：node 和栈底都不能为空 */
    if (stack_top == NULL || stack_base == NULL) {
        return;
    }

    /* 栈大小 = 上边界 - 下边界（字节） */
    size = (uint32_t)((uint8_t*)stack_top - (uint8_t*)stack_base);
    if (size == 0) {
        return;
    }

    /* 关键：偏移必须从栈底算，栈底才是分配块的起点 */
    byte_offset = (uint32_t)((uint8_t*)stack_base - (uint8_t*)task_heap);
    if (byte_offset >= TASK_HEAP_SIZE || (byte_offset % TASK_BLOCK_SIZE) != 0) {
        return;   /* 不属于本池，或栈底不在块边界 */
    }

    block_index = byte_offset / TASK_BLOCK_SIZE;
    block_count = (size + TASK_BLOCK_SIZE - 1) / TASK_BLOCK_SIZE;

    /* 越界检查 */
    if (block_index + block_count > TASK_BLOCK_NUM) {
        return;
    }

    /* 防重复释放：起始块必须是 BUSY */
    if (task_block_state[block_index] != STATE_BUSY) {
        return;
    }

    /* 标记空闲 */
    for (i = 0; i < block_count; i++) {
        task_block_state[block_index + i] = STATE_FREE;
    }

    /* 清零释放区域：从栈底开始 */
    memset((uint8_t*)stack_base, 0, TASK_BLOCK_SIZE * block_count);
}


/**
 * @brief  从节点池分配一个节点，并挂入指定优先级的就绪队列尾部
 * @param  priority 目标优先级，取值范围 [0, PRIORITY_COUNT)
 * @return 成功返回节点指针；节点池已满或优先级非法返回 NULL
 */
TaskControlBlock* NodePool_Allocate(uint8_t priority)
{
    uint32_t i;
    TaskControlBlock* node;

    /* 边界检查：优先级必须小于队列数量上限，否则数组越界 */
    if (priority >= PRIORITY_COUNT) {
        return NULL;
    }

    /* 线性扫描节点池，寻找首个空闲节点 */
    for (i = 0; i < NODE_POOL_SIZE; i++) {
        if (node_pool_state[i] != STATE_FREE) {
            continue;
        }

        node = &node_pool[i];

        /* 占用节点 */
        node_pool_state[i] = STATE_BUSY;
        /* 记录优先级：释放时需依据该字段定位所属队列 */
        node->priority = priority;
        /* 清空指针域，避免复用历史数据产生悬空引用 */
        node->prev = NULL;
        node->next = NULL;

        /* 入队：追加到对应优先级队列的尾部 */
        if (ready_count[priority] == 0) {
            ready_head[priority] = node;
            ready_tail[priority] = node;
        } else {
            ready_tail[priority]->next = node;
            node->prev = ready_tail[priority];
            ready_tail[priority] = node;
        }

        /* 更新队列计数与总计数 */
        ready_count[priority]++;
        ready_total++;
        return node;
    }

    /* 节点池已满，分配失败 */
    return NULL;
}


/**
 * @brief  将节点从就绪队列中摘除，并归还至节点池
 * @param  node 待释放节点指针
 * @return 成功返回 1；参数非法、重复释放或队列为空返回 0
 * @note   本函数不适用于释放不属于本节点池的指针
 */
int NodePool_Release(TaskControlBlock* node)
{
    uint32_t index;
    uint8_t priority;

    /* 参数合法性校验：任一条件不满足则直接拒绝 */
    if (ready_total == 0) {                          /* 队列为空 */
        return 0;
    }
    if (node == NULL) {                              /* 空指针 */
        return 0;
    }
    if (node < node_pool || node >= node_pool + NODE_POOL_SIZE) {  /* 指针越界 */
        return 0;
    }

    /* 依据指针差值计算节点在节点池中的下标 */
    index = (uint32_t)(node - node_pool);
    /* 读取节点所属优先级 */
    priority = node->priority;
    /* 重复释放保护：节点状态非占用即视为已释放 */
    if (node_pool_state[index] != STATE_BUSY) {
        return 0;
    }

    /* 摘链操作，按节点位置分四种情况处理：
     * 1) 该优先级队列中唯一节点（队首与队尾重合）：队列整体置空；
     * 2) 队首节点：队首指针后移；
     * 3) 队尾节点：队尾指针前移；
     * 4) 中间节点：前驱与后继节点直接互连。
     *
     * 注意：单节点判定必须基于 ready_count[priority]（本队列计数），
     * 而非 ready_total（全局总数），否则其他优先级的节点存在时，
     * 本队列唯一节点会走队首/队尾分支并对空指针解引用。 */
    if (ready_count[priority] == 1) {
        ready_head[priority] = NULL;
        ready_tail[priority] = NULL;
    } else if (node == ready_head[priority]) {
        ready_head[priority] = node->next;
        ready_head[priority]->prev = NULL;
    } else if (node == ready_tail[priority]) {
        ready_tail[priority] = node->prev;
        ready_tail[priority]->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    /* 统一收尾：清除指针域、归还节点、更新计数 */
    node->prev = NULL;
    node->next = NULL;
    node_pool_state[index] = STATE_FREE;
    ready_count[priority]--;
    ready_total--;
    return 1;
}

uint8_t ETask_Creat(TaskFunction taskfunction,uint8_t priority,uint32_t size)
{
    uint32_t* stack_top;
    uint32_t* stack_base;
    if (Heap_Allocate(size,&stack_base,&stack_top))return 0;
    if (stack_top == 0) return 0;

    TaskControlBlock *node=NodePool_Allocate(priority);
    if (node == 0) {
        Heap_Release(stack_base, stack_top);
        return 0;
    }
    node->size = size;
    node->stack_base = stack_base;
    node->stack_top = stack_top;
    node->task_id = Task_ID;
    Task_ID++;
    uint32_t* sp = stack_top;

    *--sp = 0x01000000;              // xPSR (Thumb位=1)
    *--sp = (uint32_t)taskfunction;    // PC (任务入口)
    *--sp = 0xFFFFFFFD;              // LR (EXC_RETURN)
    *--sp = 0;                       // R12
    *--sp = 0;                       // R3
    *--sp = 0;                       // R2
    *--sp = 0;                       // R1
    *--sp = 0;                       // R0

    *--sp = 0;                       // R4
    *--sp = 0;                       // R5
    *--sp = 0;                       // R6
    *--sp = 0;                       // R7
    *--sp = 0;                       // R8
    *--sp = 0;                       // R9
    *--sp = 0;                       // R10
    *--sp = 0;                       // R11
    // 保存到任务控制块
    node->sp = sp;      // 指向 R0
    node->state = 0;
    //创建成功
    return 1;
}



int main(void)
{
    uint8_t priority;
    const TaskControlBlock* node;

    /* 每个优先级各分配一个节点 */
    for (priority = 9; priority < PRIORITY_COUNT; ) {
        if (NodePool_Allocate(priority) == NULL) {
            break;
        }
    }

    /* 打印优先级 9 的队列内容 */
    for (node = ready_head[9]; node != NULL; node = node->next) {
        printf("%p\n", (void*)node);
    }
    return 0;
}
