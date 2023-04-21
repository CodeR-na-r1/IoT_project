# python 3.10+ fastAPIserver

# start server -> uvicorn server:app --host 192.168.0.15

# for connect to server as espBeacon -> var ws = new WebSocket(`ws://192.168.0.15:8000/esp/YOUR_SSID_ID_ESP`);

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from BeaconConnectionManager import BeaconConnectionManager, BeaconConnectionManagerException
from Console_colors import bcolors


app = FastAPI()


beacon_manager = BeaconConnectionManager()


@app.websocket("/esp/{WiFiSSID}")
async def websocket_endpoint(websocket: WebSocket, WiFiSSID: str):
    try:
        await beacon_manager.connect(websocket, WiFiSSID)

        print(f"{bcolors.OKGREEN}Connected: {beacon_manager.clientsCount} beacons{bcolors.ENDC}")

        while True:
            data = await websocket.receive_text()   # просто поддерживаем подключение, общение в EspConnectionManager
            print(f"{WiFiSSID} -> {data}")
    except BeaconConnectionManagerException as e:
        print(f"{bcolors.FAIL}{e}{bcolors.ENDC}")
    except WebSocketDisconnect:
        await beacon_manager.disconnect(WiFiSSID)
        print(f"{bcolors.OKGREEN}Connected: {beacon_manager.clientsCount} beacons{bcolors.ENDC}")