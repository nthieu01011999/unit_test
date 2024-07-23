#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AK_MSG_NULL ((ak_msg_t *)0)
#define PURE_MSG_TYPE (0x40)
#define DYNAMIC_MSG_TYPE (0x80)
#define COMMON_MSG_TYPE (0xC0)

/* Error handling macro */
#define FATAL(module, error_code) do { \
    printf("Fatal error in %s with code %x\n", module, error_code); \
    exit(EXIT_FAILURE); \
} while (0)

typedef struct header_t{
    unsigned int type;
    unsigned int len;
    void* payload;
} header_t;

typedef struct ak_msg_t {
    struct ak_msg_t* next;
    struct ak_msg_t* prev;
    header_t* header;
} ak_msg_t;

ak_msg_t* get_common_msg() {
    ak_msg_t* msg = (ak_msg_t*)malloc(sizeof(ak_msg_t));
    msg-> header = (header_t*)malloc(sizeof(header_t));
    msg->header->type = COMMON_MSG_TYPE;
    return msg;
}

unsigned int get_msg_type(ak_msg_t* msg) {
    return msg->header->type;
}

ak_msg_t* ak_memcpy_msg(ak_msg_t* src) {
    ak_msg_t* ret_msg = AK_MSG_NULL;

    if (src == AK_MSG_NULL) {
        FATAL("AK", 0x06);
    }

    switch (get_msg_type(src))
    {
    case COMMON_MSG_TYPE:
        ret_msg = get_common_msg();
        memcpy(ret_msg->header, src->header, sizeof(header_t));
        set_data_common_msg(ret_msg,(char*)src->header->payload, src->header->len );
        break;
    
    default:
        break;
    }

    return ret_msg;
}

void set_data_common_msg(ak_msg_t* msg, char* data, unsigned int len) {
    msg->header->payload = malloc(len);
    memcpy(msg->header->payload,data, len);
}

 int main() {
    ak_msg_t* src = get_common_msg();
    src->header->payload = malloc(10);
    src->header->len = 10;

    ak_msg_t* copied_msg = ak_memcpy_msg(src);
    
    return 0;
}
