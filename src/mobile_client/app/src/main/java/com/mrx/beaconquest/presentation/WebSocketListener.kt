package com.mrx.beaconquest.presentation

import android.content.Context
import android.os.Looper
import android.util.Log
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import okhttp3.Response
import okhttp3.WebSocket
import okhttp3.WebSocketListener

class WebSocketListener(): WebSocketListener() {

    private val liveResponse = MutableLiveData<String>()

    fun getLiveResponse() : LiveData<String> = this.liveResponse

    override fun onOpen(webSocket: WebSocket, response: Response) {
        super.onOpen(webSocket, response)

        Log.d(CONSTANTS.TAG, "Websockets connected")
    }

    override fun onClosing(webSocket: WebSocket, code: Int, reason: String) {
        super.onClosing(webSocket, code, reason)

        webSocket.close(code = 0, reason = null)

        Log.d(CONSTANTS.TAG, "Websockets closed")
    }

    override fun onFailure(webSocket: WebSocket, t: Throwable, response: Response?) {
        super.onFailure(webSocket, t, response)

        Log.d(CONSTANTS.TAG, "Websockets failure! -> ${t.message}")
    }

    override fun onMessage(webSocket: WebSocket, text: String) {
        super.onMessage(webSocket, text)

        liveResponse.postValue(text)
    }
}