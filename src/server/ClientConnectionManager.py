from fastapi import FastAPI, WebSocket, WebSocketDisconnect

class ClientConnectionManagerException(Exception):
    ...

class ClientConnectionManager:

    def __init__(self):
        self.active_connections: dict[str, WebSocket] = {}  # ~? annotation

    async def connect(self, websocket: WebSocket, color: str):
        if color in self.active_connections:
            raise ClientConnectionManagerException(f"Client with color -> {color} already connected!")
        
        await websocket.accept()

        self.active_connections[color] = websocket

    async def disconnect(self, color: str):
        del self.active_connections[color]

    @property
    def clientsCount(self):
        return len(self.active_connections)