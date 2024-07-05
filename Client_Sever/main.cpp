#include <iostream>
#include <memory>
#include "Client.h"
#include "Server.h"

int main() {

    std::shared_ptr<Server> server = std::make_shared<Server>();

    std::shared_ptr<Client> client1 = std::make_shared<Client>(1);
    std::shared_ptr<Client> client2 = std::make_shared<Client>(2);

    server->addClient(client1);
    server->addClient(client2);

    server->listClients();

    client1->interactWithServer();
    client2->interactWithServer();

    return 0;
}
