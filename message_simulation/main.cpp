#include <iostream>
#include <cstring>
#include <string>

// Define the header_t structure
typedef struct {
    uint32_t type;
    uint32_t root_task_id;
    uint32_t src_task_id;
    uint32_t des_task_id;
    uint32_t sig;
    uint32_t if_src_task_id;
    uint32_t if_des_task_id;
    uint32_t if_src_type;
    uint32_t if_des_type;
    uint32_t if_sig;
    uint32_t len;
    void *payload;
} header_t;

// Define the ak_msg_t structure
typedef struct ak_msg_t {
    ak_msg_t *next;
    ak_msg_t *prev;
    header_t *header;
} ak_msg_t;


// Function to simulate message handling
void handle_message(ak_msg_t *msg) {
    if (msg->header && msg->header->payload && msg->header->len > 0) {
        // Convert payload to std::string using the described method
        std::string payload_str((char *)msg->header->payload, msg->header->len);
        std::cout << "Processed Payload: " << payload_str << std::endl;
    } else {
        std::cout << "Invalid message or payload." << std::endl;
    }
}


int main() {
    // Sample JSON payload
    const char *json_payload = R"({"name":"Alice", "age":30})";
    uint32_t payload_length = strlen(json_payload);

    // Prepare the message structures
    header_t header;
    header.len = payload_length;
    header.payload = (void*)json_payload;

    ak_msg_t message;
    message.header = &header;

    // Call the handling function
    handle_message(&message);

    return 0;
}
