#define CROW_MAIN
#include "crow_all.h"
#include <iostream>
#include <string>
#include <libpq-fe.h>

PGconn* conn = nullptr;

bool connect_db() {
    const char* conn_str = "host=db port=5432 dbname=mydb user=user password=password";
    conn = PQconnectdb(conn_str);
    
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Ошибка подключения к БД: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return false;
    }
    std::cout << "Подключение к PostgreSQL успешно!" << std::endl;
    return true;
}

int main() {
    if (!connect_db()) {
        return 1; 
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([](){
        crow::json::wvalue response;
        response["message"] = "C++ сервер работает!";
        return crow::response(response);
    });

    CROW_ROUTE(app, "/save").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        try {
            auto body = crow::json::load(req.body);
            if (!body) {
                return crow::response(400, "{\"error\": \"Некорректный JSON\"}");
            }
            
            // Теперь ищет ключ "message", как и Python версия
            std::string text = body["message"].s();
            
            const char* paramValues[1] = { text.c_str() };
            PGresult* res = PQexecParams(conn, 
                "INSERT INTO logs (message) VALUES ($1)", 
                1, nullptr, paramValues, nullptr, nullptr, 0);
                
            if (PQresultStatus(res) == PGRES_COMMAND_OK) {
                PQclear(res);
                crow::json::wvalue response;
                response["status"] = "ok";
                // Унифицировала ответ
                response["saved_message"] = text; 
                return crow::response(response);
            } else {
                std::cerr << "Ошибка БД: " << PQerrorMessage(conn) << std::endl;
                PQclear(res);
                return crow::response(500, "{\"error\": \"Ошибка сохранения в БД\"}");
            }
        } catch (const std::exception& e) {
            return crow::response(500, std::string("{\"error\": \"") + e.what() + "\"}");
        }
    });

    std::cout << "C++ сервер запущен на порту 8080" << std::endl;
    app.port(8080).multithreaded().run();
    
    PQfinish(conn);
    return 0;
}