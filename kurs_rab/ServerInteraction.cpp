/**
 * @file ServerInteraction.cpp
 */
#include "ServerInteraction.h"
#include "./md5/md5.h"

/**
* @brief Сеттер для параметра serverAddress_
*/
void ServerInteraction::SetServerAddress(int address) {
    serverAddress_ = address;
}

/**
* @brief Сеттер для параметра serverPort_
*/
void ServerInteraction::SetServerPort(int port) {
    serverPort_ = port;
}

/**
* @brief Сеттер для параметра vectorDataFile_
*/
void ServerInteraction::SetVectorDataFile(const std::string& filename) {
    vectorDataFile_ = filename;
}

/**
* @brief Сеттер для параметра resultFile_
*/
void ServerInteraction::SetResultFile(const std::string& filename) {
    resultFile_ = filename;
}

/**
* @brief Сеттер для параметра clientAuthFile_
*/
void ServerInteraction::SetClientAuthFile(const std::string& filename) {
    clientAuthFile_ = filename;
}
    
/**
* @brief Геттер для параметра serverAddress_
*/
int ServerInteraction::GetServerAddress() const {
    return serverAddress_;
}

/**
* @brief Геттер для параметра serverPort_
*/
int ServerInteraction::GetServerPort() const {
    return serverPort_;
}

/**
* @brief Геттер для параметра vectorDataFile_
*/
std::string& ServerInteraction::GetVectorDataFile() {
    return vectorDataFile_;
}

/**
* @brief Геттер для параметра resultFile_
*/
std::string& ServerInteraction::GetResultFile() {
    return resultFile_;
}

/**
* @brief Геттер для параметра clientAuthFile_
*/
std::string& ServerInteraction::GetClientAuthFile() {
    return clientAuthFile_;
}

/**
* @brief Функция для сохранения результатов вычислений на сервер
*/
int ServerInteraction::write_results_to_file(const std::vector<double>& vectors) {
    ofstream outfile(GetResultFile(), ios::binary);
    // Проверка файла на открытие
    if (!outfile.is_open()) {
        outfile.close();
        return 1;
    }

    uint32_t vec_size = vectors.size();
    outfile.write(reinterpret_cast<const char*>(&vec_size), sizeof(uint32_t));

    outfile.write(reinterpret_cast<const char*>(&vectors[0]), vectors.size() * sizeof(double));

    outfile.close();
    return 0;
}

/**
* @brief Функция для взаимодействия с сервером
* @return Возвращает 0 в случае успешной аутентификации клианта на сервере, иначе тип ошибки
*/
int ServerInteraction::authenticate(int sock, string login, string password){
    // Буфер для взаимодействия клиента с севером
    char buf[1024];
    int bytes_read;

    // Отправка логина клиента
    strcpy(buf,login.c_str());
    send(sock, buf, login.length(), 0);

    // Прнятие ответа либо SALT, либо ERR
    bytes_read = recv(sock, buf, sizeof(buf), 0);
    buf[bytes_read] = '\0';
    string flag1 = string(buf);
    if (flag1 == "ERR"){
        return 1;
    }

    // Отправка HASHMD5( SALT16 || PASSWORD)
    string mes = flag1 + password;
    string HASHMD5 = MD5_hash(mes);

    strcpy(buf,HASHMD5.c_str());
    send(sock, buf, HASHMD5.length(), 0);

    // Прнятие ответа либо ОК, либо ERR
    bytes_read = recv(sock, buf, sizeof(buf), 0);
    buf[bytes_read] = '\0';
    string flag2 = string(buf);
    if (flag2 == "ERR"){
        return 2;
    }
    return 0;
}

/**
* @brief Функция для чтения векторов из файла
* @return возвращает массив векторов типа <double>
*/
vector<std::vector<double>> ServerInteraction::read_vector_in_file(){
    // Открытие файла для чтения
    ifstream infile(GetVectorDataFile(), ios::binary);
    if (!infile.is_open()) {
        throw ExceptionManager("Ошибка. Функция: read_vector_in_file.\n Не возможно открыть файл");
    }

    if(infile.peek() == EOF){
        throw ExceptionManager("Ошибка. Функция: read_vector_in_file.\n Файл пуст");
    }

    // Считываем количество векторов (uint32_t)
    vector<std::vector<double>> data;
    uint32_t numVectors;
    infile.read(reinterpret_cast<char*>(&numVectors), sizeof(uint32_t));

    // Считываем каждый вектор
    for (uint32_t i = 0; i < numVectors; ++i) {
        uint32_t vectorSize;
        infile.read(reinterpret_cast<char*>(&vectorSize), sizeof(uint32_t));

        std::vector<double> vector(vectorSize);
        for (double& value : vector) {
            infile.read(reinterpret_cast<char*>(&value), sizeof(double));
        }

        data.push_back(vector);
    }

    infile.close();

    return data;
}

/**
* @brief Функция для отправки на сервер векторов и записи результатов в файл
*/
int ServerInteraction::send_data(int sock){
    // Считывание векторов
    vector<vector<double>> vectors = read_vector_in_file();

    // Отправка количества векторов типа double
    int vec_size = vectors.size();
    send(sock, &vec_size, sizeof(vec_size), 0);

    // Создание переменной для результатов вычислений
    vector<double> res_vectors;

    // Отправка векторов
    for(int i = 0; i < vec_size; i++){

        // Отправка длины каждого вектора
        int vec_len = vectors[i].size();
        send(sock, &vec_len, sizeof(vec_len), 0);


        // Создание буфера для передачи на сервер вектора uint32_t
        double arr[vec_len] = {0};
        for (size_t j = 0; j < vectors[i].size(); j++) {
            arr[j] = vectors[i][j];
        }

        // Отправка вектора
        send(sock, &arr, sizeof(arr), 0);

        // Получение результата
        double res = 0;
        recv(sock, &res, sizeof(res), 0);

        res_vectors.push_back(res);

    }

    write_results_to_file(res_vectors);

    return 0;

}
/**
* @brief Функция извлечение логина и пароля
* @return упорядоченную пару значений типа string, string соответствующих логину и паролю
*/
std::pair<std::string, std::string> ServerInteraction::getLoginAndPassword() {
    ifstream file(GetClientAuthFile());
    // Проверка файла на открытие
    if(!file.is_open()){
        file.close();
        throw ExceptionManager("Ошибка. Функция: getLoginAndPassword.\n Не возможно отрыть файл");
        }
    // Проверка файла на пустоту
    if (file.peek() == std::ifstream::traits_type::eof()) {
        file.close();
        throw ExceptionManager("Ошибка. Функция: getLoginAndPassword.\n Файл пуст");
    }
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string login, password;
    iss >> login >> password;
    file.close();
    return std::make_pair(login, password);

}

/**
* @brief Функция для взаимодействия с сервером
* @throw ExceptionManager при возникновении ошибки
*/
int ServerInteraction::perform_server_interaction() {

    std::string user_login, user_password;

    std::tie(user_login, user_password) = getLoginAndPassword();


    // Создание сокета для взаимодействия клиент-сервер
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {   
        throw ExceptionManager("Ошибка. Функция: perform_server_interaction.\n Не возможно создать сокет");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(GetServerPort());
    addr.sin_addr.s_addr = htonl(GetServerAddress());
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        throw ExceptionManager("Ошибка. Функция: perform_server_interaction.\n Сервер не доступен");
    }

    int flag = authenticate(sock, user_login, user_password);
    if (flag != 0){
        close(sock);
        if (flag){
            throw ExceptionManager("Ошибка. Функция: perform_server_interaction.\n Некорректно введеный логин");
        }
        else{
            throw ExceptionManager("Ошибка. Функция: perform_server_interaction.\n Некорректно введеный пароль");
        }
    }

    int flag2 = send_data(sock);
    if (flag2 == 0){
        cout << "В файл:" << GetResultFile() << " записаны результаты вычислений."<< endl;
    }
    
    close(sock);

    return 0;
    }
