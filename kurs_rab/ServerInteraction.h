/** 
* @file ClientManager.h
* @author Присяжный А.Ю.
* @version 1.0
* @date 11.09.2023
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля ClientManager
*/
#pragma once
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>
#include <locale>
#include <codecvt>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


/** 
* @brief Класс взаимодействия с сервером
* @param serverAddress_ Адрес сервера
* @param serverPort_ Порт сервера
* @param vectorDataFile_ Имя файла с векторами
* @param resultFile_ Имя файла с результатами
* @param clientAuthFile_ Имя файла с логином и паролем клиента
*/
class ServerInteraction {
public:

    // Метод для установки адреса сервера
    void SetServerAddress(int address);

    // Метод для установки порта сервера
    void SetServerPort(int port);

    // Метод для установки имени файла с векторами
    void SetVectorDataFile(const std::string& filename);

    // Метод для установки имени файла с результатами
    void SetResultFile(const std::string& filename);

    // Метод для установки имени файла с логином и паролем клиента
    void SetClientAuthFile(const std::string& filename);
    
    // Метод для получения адреса сервера
    int GetServerAddress() const;

    // Метод для получения порта сервера
    int GetServerPort() const;

    // Метод для получения имени файла с векторами
    std::string& GetVectorDataFile();

    // Метод для получения имени файла с результатами
    std::string& GetResultFile();

    // Метод для получения имени файла с логином и паролем клиента
    std::string& GetClientAuthFile();


    // Функция для отправки на сервер логина и пароля
    int authenticate(int sock, string username, string password);

    // Функиция для отправки на сервер векторов и получение результата вычислений
    int send_data(int sock);

    // Функия для сохранения результатов в файл
    int write_results_to_file(const std::vector<double>& vectors);

    // Функция для чтения векторов из файла
    vector<std::vector<double>> read_vector_in_file();

    // Функция для получения из файла логина и пароля
    pair<std::string, std::string> getLoginAndPassword();

    // Основной метод взаимодействия с сервером
    int perform_server_interaction();

private:
    int serverAddress_;             // Адрес сервера
    int serverPort_;                // Порт сервера
    std::string vectorDataFile_;    // Имя файла с векторами
    std::string resultFile_;        // Имя файла с результатами
    std::string clientAuthFile_;    // Имя файла с логином и паролем клиента
};


/** @brief Класс предназначен для обработки и управления ошибками ExceptionManager
* @details Управляющий выводом сообщений о возникших ошибках
*/
class ExceptionManager: public std::invalid_argument
{
public:
	explicit ExceptionManager (const std::string& what_arg):
		std::invalid_argument(what_arg) {}
	explicit ExceptionManager (const char* what_arg):
		std::invalid_argument(what_arg) {}
};


    