from fastapi import FastAPI
from pydantic import BaseModel
import asyncpg # Импортируем библиотеку для базы данных

app = FastAPI()

class Message(BaseModel):
    text: str

@app.get("/")
async def root():
    return {"message": "Сервер работает!"}


@app.post("/save")
async def save_data(item: Message):
    conn = await asyncpg.connect("postgresql://user:password@db:5432/mydb")
    
    await conn.execute("INSERT INTO logs (message) VALUES ($1)", item.text)
    
    await conn.close()
    
    return {"status": "ok", "saved_text": item.text}