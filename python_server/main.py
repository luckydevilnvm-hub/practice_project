from fastapi import FastAPI
from pydantic import BaseModel
import asyncpg

app = FastAPI()

# Унифицировала имя поля на "message"
class Message(BaseModel):
    message: str

@app.get("/")
async def root():
    return {"message": "Python сервер работает!"}

@app.post("/save")
async def save_data(item: Message):
    # Подключаемся, сохраняем, закрываем 
    conn = await asyncpg.connect("postgresql://user:password@db:5432/mydb")
    await conn.execute("INSERT INTO logs (message) VALUES ($1)", item.message)
    await conn.close()
    
    # Унифицировала ответ
    return {"status": "ok", "saved_message": item.message}