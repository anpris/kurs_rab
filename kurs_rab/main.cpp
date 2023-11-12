#include "ServerInteraction.h"
#include <getopt.h>
#pragma one

/** 
* @file main.cpp
* @brief Функция для получения параметров от пользователя для взаимодействия с сервером 
*/

/**
 * @brief Главный модуль программы для получения параметров от пользователя
*/
int main(int argc, char** argv) {
    int serverAddress;
    int serverPort = 33333;  // Порт сервера (необязательный)
    std::string inputDataFile;
    std::string outputFile;
    std::string authFile = "./vclient.conf";  // Файл для аутентификации (необязательный)

    struct option long_options[] = {
        {"address", required_argument, nullptr, 'a'},
        {"port", optional_argument, nullptr, 'p'},
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {"auth", optional_argument, nullptr, 'f'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

        int opt;
        while ((opt = getopt_long(argc, argv, "a:p:i:o:f:h::", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'a':
                serverAddress = std::stoi(optarg);
                break;
            case 'p':
                serverPort = optarg ? std::stoi(optarg) : 33333;
                break;
            case 'i':
                inputDataFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'f':
                authFile = optarg ? optarg : "./vclient.conf";
                break;
            case 'h':
                std::cout << "Опция для вывода справки для используеймой программы" << std::endl;
                std::cout << "Использование: " << argv[0] << " --address <адрес_сервера> [--port <порт>] --input <файл_для_ввода> --output <файл_для_вывода> [--auth <файл_аутентификации>]" << std::endl;
                return 1;
            default:
                std::cerr << "Использование: " << argv[0] << " --address <адрес_сервера> [--port <порт>] --input <файл_для_ввода> --output <файл_для_вывода> [--auth <файл_аутентификации>]" << std::endl;
                return 1;
            }
        }

    // Проверяем наличие обязательных параметров
    if (serverAddress == 0 || inputDataFile.empty() || outputFile.empty()) {
        std::cerr << "Ошибка. Не указаны обязательные параметры." << std::endl;
        return 1;
    }

    // Создание экземпляра класса
    ServerInteraction ServerConnection;
    ServerConnection.SetServerAddress(serverAddress);
    ServerConnection.SetServerPort(serverPort);
    ServerConnection.SetVectorDataFile(inputDataFile);
    ServerConnection.SetResultFile(outputFile);
    ServerConnection.SetClientAuthFile(authFile);

    // Вызов основной функции класса ServerConnection
    ServerConnection.perform_server_interaction();

    //./client -a 2130706433 -p33333 -i output.bin -o resultat.txt -f./config/vclient.conf
    
    return 0;
}
