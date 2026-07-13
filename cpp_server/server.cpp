#include <iostream>
#include <string>
#include <postgresql/libpq-fe.h>
#include <crow.h>

using namespace std;

// Функция подключения к PostgreSQL
PGconn* connect_db() {
    string conn_str = "host=db port=5432 dbname=mydb user=user password=password";
    PGconn* conn = PQconnectdb(conn_str.c_str());
    
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Ошибка подключения к БД: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return nullptr;
    }
    
    cout << "Подключение к БД успешно!" << endl;
    return conn;
}

// Функция сохранения сообщения
bool save_message(const string& text) {
    PGconn* conn = connect_db();
    if (!conn) return false;
    
    // Экранируем текст для безопасности
    string escaped_text = PQescapeLiteral(conn, text.c_str(), text.length());
    
    string query = "INSERT INTO logs (message) VALUES (" + escaped_text + ")";
    PGresult* res = PQexec(conn, query.c_str());
    
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    
    PQclear(res);
    PQfinish(conn);
    
    return success;
}

int main() {
    crow::SimpleApp app;
    
    // 1. ЭНДПОИНТ: / - проверка работы сервера
    CROW_ROUTE(app, "/")([](){
        crow::json::wvalue response;
        response["message"] = "C++ сервер работает!";
        return crow::response(response);
    });
    
    
    // 2. ЭНДПОИНТ: /save - сохранение сообщения (POST)
    CROW_ROUTE(app, "/save").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        try {
            auto body = crow::json::load(req.body);
            if (!body) {
                crow::json::wvalue error;
                error["error"] = "Некорректный JSON";
                return crow::response(400, error);
            }
            
            string text = body["text"].s();
            
            if (save_message(text)) {
                crow::json::wvalue response;
                response["status"] = "ok";
                response["saved_text"] = text;
                return crow::response(response);
            } else {
                crow::json::wvalue error;
                error["error"] = "Ошибка сохранения в БД";
                return crow::response(500, error);
            }
        } catch (const exception& e) {
            crow::json::wvalue error;
            error["error"] = string("Ошибка: ") + e.what();
            return crow::response(500, error);
        }
    });
    
    
    cout << "C++ сервер запущен на порту 8080" << endl;
    app.port(8080).multithreaded().run();
    
    return 0;
}