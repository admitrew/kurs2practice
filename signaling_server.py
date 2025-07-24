import asyncio
import websockets

clients = set()

async def handler(websocket):
    print("Новый клиент подключён")
    clients.add(websocket)
    try:
        async for message in websocket:
            # переслать сообщение всем другим клиентам
            for client in clients:
                if client != websocket:
                    await client.send(message)
    except websockets.exceptions.ConnectionClosed:
        print("Клиент отключился")
    finally:
        clients.remove(websocket)

async def main():
    print("Сигналинг-сервер запущен на ws://localhost:8765")
    async with websockets.serve(handler, "localhost", 8765):
        await asyncio.Future()  # бесконечное ожидание

if __name__ == "__main__":
    asyncio.run(main())
