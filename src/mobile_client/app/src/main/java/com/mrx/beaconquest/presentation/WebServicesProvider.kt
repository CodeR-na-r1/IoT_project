package com.mrx.beaconquest.presentation

import androidx.lifecycle.LiveData
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.WebSocket

object WebServicesProvider {

    private val client by lazy { OkHttpClient() }

    private val request by lazy { Request
        .Builder()
        .url(CONSTANTS.URLS.BASE_URL + CONSTANTS.URLS.PATH_TO_WEBSOCKET_ANDROID)
        .build()
    }

    private val listener = WebSocketListener()

    private var webSocket: WebSocket? = null

    fun getInstance(): WebSocket {

        if (webSocket == null) {
            webSocket = client.newWebSocket(request = request, listener = listener)
        }

        return webSocket!!
    }

    fun getLiveResponse() : LiveData<String> = listener.getLiveResponse()

}