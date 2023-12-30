#include "butility.h" 
#include "grpc/redtangle_server.h"

int main(int argc, char* argv[]) {
    try {
            auto options = butility::parse_commandline(argc, argv); 
            std::string ip = "0.0.0.0"; 
            int port = 50052;
            if (options.find("ip") != options.end()) {
                ip = options["ip"]; 
            }
            if (options.find("port") != options.end()) {
                port = stoi(options["port"]); 
            }
            RedtangleServer server(port, ip); 
            server.start_server(); 
            std::cout << "Press any key to stop..." << std::endl;
            std::cin.get();
        }  
    catch (std::exception& e) {
        std::cout << e.what() << std::endl; 
        return EXIT_FAILURE; 
    }     
    return EXIT_SUCCESS; 
}