#include "server.h"
#include <shared_mutex>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <cstring>
#include <string>
#include <vector>
#include "ICommand.h"
#include "PATCHCommand.h"
#include "POSTCommand.h"
#include "DELETECommand.h"
#include "GETCommand.h"
#include "helpCommand.h"
#include "IMenu.h"
#include "SocketMenu.h"
#include "Runnable.h"
#include "executor.h"
#include "ThreadFactory.h"
#include "App.h"
#include "DBFile.h"



server::server(int port) : port(port) {

}



void server::start() {
    std::shared_mutex rw_mutex;
    struct sockaddr_in sin;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        return;
    }
    if (listen(sock, 100) < 0) {
        return;
    }
    ThreadFactory tf;
    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *)&client_sin, &addr_len);
        if (client_sock < 0) {
            continue;
        }
        IMenu* clientMenu = new SocketMenu(client_sock);
        
        IDataBase* data = new DBFile("../data");
        std::map<std::string, ICommand*> commands;
        std::vector<ICommand*> *helpCommands = new std::vector<ICommand*>();
        ICommand* PATCH = new PATCHCommand(*data, *clientMenu);
        ICommand* POST = new POSTCommand(*data, *clientMenu);
        ICommand* DELETE = new DELETECommand(*data, *clientMenu);
        ICommand* GET = new GETCommand(*data, *clientMenu);
        ICommand* help = new helpCommand(*helpCommands, *clientMenu);
        helpCommands->push_back(DELETE);
        helpCommands->push_back(GET);
        helpCommands->push_back(PATCH);
        helpCommands->push_back(POST);
        helpCommands->push_back(help);
        // map the commands to the appropriate instances of commands
        commands["PATCH"] = PATCH;
        commands["POST"] = POST;
        commands["DELETE"] = DELETE;
        commands["help"] = help;
        commands["GET"] = GET;
        Runnable* r = new App(commands, *clientMenu, rw_mutex);
        tf.execute(r, false);
        
        
    }
}