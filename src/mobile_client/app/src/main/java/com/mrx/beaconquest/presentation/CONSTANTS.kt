package com.mrx.beaconquest.presentation

object CONSTANTS {

    const val TAG = "myTag"

    object BEACONS {

        const val MIN_RSSI = 65
        const val MAX_RSSI = 75

    }

    object URLS {

        const val BASE_URL = "http://192.168.0.13:8000"

        const val PATH_TO_COUNT_BEACONS = "BeaconsCount"
        const val PATH_TO_LIST_BEACONS = "BeaconsList"

        const val PATH_TO_WEBSOCKET_ANDROID = "/android"

    }

}