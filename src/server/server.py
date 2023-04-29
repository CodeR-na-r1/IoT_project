# python 3.10+ fastAPIserver

# start server -> uvicorn server:app --host 192.168.0.10
# start server -> uvicorn server:app --host 192.168.0.10 --ws-ping-interval 500

# fastapi ping pong -> https://codematcher.com/questions/fastapi-websocket-ping-pong-timeout
# esp library WebSockets not supported ping pong

# for connect to server as espBeacon -> var ws = new WebSocket(`ws://192.168.0.15:8000/esp/YOUR_SSID_ID_ESP`);

from fastapi import FastAPI, WebSocket, WebSocketDisconnect

from BeaconConnectionManager import BeaconConnectionManager, BeaconConnectionManagerException
from ClientConnectionManager import ClientConnectionManager, ClientConnectionManagerException

from Console_colors import bcolors

from random import randrange
from time import sleep

app = FastAPI()

beacon_manager = BeaconConnectionManager()
client_manager = ClientConnectionManager()

userColors = [
                { "color1" : { "r" : "48", "g" : "213", "b" : "200" } }, # Бирюзовый
                { "color2" : { "r" : "255", "g" : "77", "b" : "0" } },  # Киноварь (orange)
                { "color3" : { "r" : "0", "g" : "255", "b" : "0" } },  # Лайм (green)
                { "color4" : { "r" : "0", "g" : "127", "b" : "255" } },  # Лазурный, Азур (blue)
             ]

# get requests

@app.get("/BeaconsCount")
async def beaconsCount():
    return {"BeaconsCount": f"{beacon_manager.clientsCount}"}

@app.get("/BeaconsList")
async def beaconsList():
    return {"BeaconsList": [SSID for SSID in beacon_manager.active_connections.keys()]}

# WebSocket endpoints

# for beacon (esp)

@app.websocket("/esp/{WiFiSSID}")
async def websocket_endpoint(websocket: WebSocket, WiFiSSID: str):
    try:
        await beacon_manager.connect(websocket, WiFiSSID)

        print(f"{bcolors.OKGREEN}Connected: {beacon_manager.clientsCount} beacons{bcolors.ENDC}")

        while True: 
            #await websocket.send_json({"mode" : "1", "color" : {"r" : "12", "g" : "255", "b" : "123"}})
            data = await websocket.receive_text()   # просто поддерживаем подключение, общение в BeaconConnectionManager
            print(f"{WiFiSSID} -> {data}")
    except BeaconConnectionManagerException as e:
        print(f"{bcolors.FAIL}{e}{bcolors.ENDC}")
    except WebSocketDisconnect:
        await beacon_manager.disconnect(WiFiSSID)
        print(f"{bcolors.OKGREEN}Connected: {beacon_manager.clientsCount} beacons{bcolors.ENDC}")

# for client (android)

@app.websocket("/android")
async def websocket_endpoint(websocket: WebSocket):
    try:
        colorDict : dict = userColors.pop(randrange(len(userColors)))
        color : str = str(colorDict)

        await client_manager.connect(websocket, color)

        print(f"{bcolors.OKGREEN}Connected: {client_manager.clientsCount} clients{bcolors.ENDC}")

        while True:
            data = await websocket.receive_json()

            if "intent" in data:
                if data["intent"] == "getColor":
                    await websocket.send_json({"color" : colorDict})
                elif data["intent"] == "activateBeacon":
                    try:
                        await beacon_manager.notifyBeacon(data["beaconId"], colorDict[list(colorDict.keys())[0]], True)
                        await websocket.send_json({"status" : "OK"})
                    except Exception as e:
                        await websocket.send_json({"status" : "ERROR", "description" : f"{e}"})
                elif data["intent"] == "deactivateBeacon":
                    try:
                        await beacon_manager.notifyBeacon(data["beaconId"], colorDict[list(colorDict.keys())[0]], False)
                        await websocket.send_json({"status" : "OK"})
                    except Exception as e:
                        await websocket.send_json({"status" : "ERROR", "description" : f"{e}"})

    except ValueError:
        await websocket.accept()

        sleep(3)

        await websocket.send_json({"status" : "ERROR", "description" : "too many clients connected"})

        print(f"{bcolors.FAIL}canceled connected, too many clients connected{bcolors.ENDC}")

    except ClientConnectionManagerException as e:
        print(f"{bcolors.FAIL}{e}{bcolors.ENDC}")

    except WebSocketDisconnect:
        await client_manager.disconnect(color)
        userColors.append(colorDict)

        print(f"{bcolors.OKGREEN}Connected: {client_manager.clientsCount} clients{bcolors.ENDC}")