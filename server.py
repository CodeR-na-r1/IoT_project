# python 3.10+ fastAPIserver

# start server -> uvicorn server:app --host 192.168.0.15

from fastapi import FastAPI, WebSocket, WebSocketDisconnect


app = FastAPI()

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
        await websocket.accept()
        self.active_connections[wifi_SSID] = websocket

    def disconnect(self, wifi_SSID: str):
        del self.active_connections[wifi_SSID]

    async def notifyBeacon(self, wifi_SSID: str, color: str, isActivateIntent: bool):
        data_mode: str = (self.MODES.ACTIVATE if isActivateIntent else self.MODES.DEACTIVATE).value

        data = {"mode" : data_mode, "color" : color}

        try:
            await self.active_connections[wifi_SSID].send_json(data)
        except KeyError:
            BeaconConnectionManagerException(f"WiFi beacon with {wifi_SSID} not found!")
        except Exception:
            BeaconConnectionManagerException(f"error sending data to {wifi_SSID}")


beacon_manager = BeaconConnectionManager()


@app.websocket("/esp/{WiFiSSID}")
async def websocket_endpoint(websocket: WebSocket, WiFi_SSID: str):
    await beacon_manager.connect(websocket, WiFi_SSID)
    try:
        while True:
            data = await websocket.receive_text()   # просто поддерживаем подключение, общение в EspConnectionManager
            print(f"{WiFi_SSID} -> {data}")
    except WebSocketDisconnect:
        beacon_manager.disconnect(websocket)