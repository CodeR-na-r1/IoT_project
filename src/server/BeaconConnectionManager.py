from fastapi import FastAPI, WebSocket, WebSocketDisconnect

class BeaconConnectionManagerException(Exception):
    ...

class BeaconConnectionManager:
    
    from enum import Enum
    class MODES(Enum):
        ACTIVATE = "1"
        DEACTIVATE = "2"


    def __init__(self):
        self.active_connections: dict[str, WebSocket] = {}  # ~? annotation

    async def connect(self, websocket: WebSocket, wifi_SSID: str):
        if wifi_SSID in self.active_connections:
            raise BeaconConnectionManagerException(f"Beacon {wifi_SSID} already connected!")
        await websocket.accept()
        self.active_connections[wifi_SSID] = websocket

    async def disconnect(self, wifi_SSID: str):
        del self.active_connections[wifi_SSID]

    async def notifyBeacon(self, wifi_SSID: str, color: str, isActivateIntent: bool):
        data_mode: str = (self.MODES.ACTIVATE if isActivateIntent else self.MODES.DEACTIVATE).value

        data = {"mode" : data_mode, "color" : color}

        try:
            await self.active_connections[wifi_SSID].send_json(data)
        except KeyError:
            raise BeaconConnectionManagerException(f"WiFi beacon with {wifi_SSID} not found!")
        except Exception as e:
            raise BeaconConnectionManagerException(f"error sending data to {wifi_SSID} ({e})")

    @property
    def clientsCount(self):
        return len(self.active_connections)