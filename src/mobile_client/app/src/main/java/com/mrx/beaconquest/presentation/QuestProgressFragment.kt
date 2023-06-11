package com.mrx.beaconquest.presentation

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.MutableLiveData
import com.mrx.beaconquest.R
import com.mrx.beaconquest.data.retrofit.models.BeaconsList
import com.mrx.beaconquest.databinding.FragmentQuestProgressBinding
import com.mrx.indoorservice.api.IndoorService
import com.mrx.indoorservice.domain.model.WiFiBeaconsEnvironmentInfo
import org.json.JSONObject

class QuestProgressFragment(private val _context: Context, private val _beaconsList: BeaconsList) : Fragment() {

    companion object {
        fun newInstance(__context: Context, __beaconsList: BeaconsList) = QuestProgressFragment(_context = __context, _beaconsList = __beaconsList)
    }

    private lateinit var binding: FragmentQuestProgressBinding

    private val webSocket by lazy { WebServicesProvider.getInstance() }

    private val indoorService by lazy { IndoorService.getInstance(context = _context) }
    private var nowBeacon: MutableLiveData<String?> = MutableLiveData(null)
    private val remainingBeacons = _beaconsList.BeaconsList

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        binding = FragmentQuestProgressBinding.inflate(layoutInflater)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // create sockets and connect
        WebServicesProvider.getLiveResponse().observe(this as LifecycleOwner, webSocketObserver)
        webSocket.send("{\"intent\" : \"getColor\"}")

        indoorService.WiFiBeaconsEnvironment.setScanInterval(2200L)
        indoorService.WiFiBeaconsEnvironment.addSSIDFilterForSpecificScan(this._beaconsList.BeaconsList)
        indoorService.WiFiBeaconsEnvironment.getSpecificScanViewModel().observe(this as LifecycleOwner, wifiScanObserver)
        indoorService.WiFiBeaconsEnvironment.startSpecificScanning()

        nowBeacon.observe(this as LifecycleOwner, changeNowBeaconObserver)

        binding.btnCommitBeacon.setOnClickListener(this.btnCommitBeaconObserver)
    }

    override fun onDestroyView() {
        super.onDestroyView()

        indoorService.WiFiBeaconsEnvironment.stopSpecificScanning()
        webSocket.close(0, null)
    }

    private val webSocketObserver: ((String) -> Unit) = { data: String ->

        Log.d(CONSTANTS.TAG, "Receive data -> $data")

        if (data.contains("status")) {
            val obj = JSONObject(data)
            if (obj.getString("status") == "ERROR") {
                Toast.makeText(context, "Error request on server -> ${obj.getString("description")}", Toast.LENGTH_LONG).show()
            } else {
                Toast.makeText(context, "Request successful", Toast.LENGTH_LONG).show()
            }
        }
        else if (data.contains("color")) {
            Log.d(CONSTANTS.TAG, "Get color $data")
            // toDo show user color
//            val obj = JSONObject(data).getJSONObject("color")
//            binding.userColorBtn.setBackgroundColor(color)
        }
        else {
            Toast.makeText(context, "Unknown response!", Toast.LENGTH_LONG).show()
            Log.d(CONSTANTS.TAG, "Unknown response!")
        }

    }

    private val wifiScanObserver: ((Collection<WiFiBeaconsEnvironmentInfo>) -> Unit) = { _newData: Collection<WiFiBeaconsEnvironmentInfo> ->

        Log.d(CONSTANTS.TAG, "wifiScanObserver -> data size = ${_newData.size}")

        val newData = _newData.map { WiFiBeaconsEnvironmentInfo(it.ssid, -it.rssi) }    // invert rssi

        if (newData.isNotEmpty()) {

            Log.d(CONSTANTS.TAG, "newData isNotEmpty!")

            val elem = (newData.filter { remainingBeacons.contains(it.ssid) })
                .minBy { it.rssi }

            Log.d(CONSTANTS.TAG, "elem = ${elem.ssid} -> ${elem.rssi}!")
//            Toast.makeText(context, "elem = ${elem.ssid} -> ${elem.rssi}!", Toast.LENGTH_SHORT).show()

            if (nowBeacon.value != null) {
                if (elem.rssi <= CONSTANTS.BEACONS.MIN_RSSI && nowBeacon.value != elem.ssid) {
                    Log.d(CONSTANTS.TAG, "beacon replaced 1")
                    webSocket.send("{\"intent\" : \"deactivateBeacon\", \"beaconId\" : \"${nowBeacon.value}\"}")
                    nowBeacon.value = elem.ssid
                    webSocket.send("{\"intent\" : \"activateBeacon\", \"beaconId\" : \"${nowBeacon.value}\"}")
                }
                else if ((newData.find { it.ssid == nowBeacon.value }?.rssi ?: CONSTANTS.BEACONS.MAX_RSSI) >= CONSTANTS.BEACONS.MAX_RSSI) {
                    Log.d(CONSTANTS.TAG, "beacon deactivated 2")
                    webSocket.send("{\"intent\" : \"deactivateBeacon\", \"beaconId\" : \"${nowBeacon.value}\"}")
                    nowBeacon.value = null
                }
            }
            else {
                if (elem.rssi <= CONSTANTS.BEACONS.MIN_RSSI) {
                    Log.d(CONSTANTS.TAG, "beacon activated 3")
                    nowBeacon.value = elem.ssid
                    webSocket.send("{\"intent\" : \"activateBeacon\", \"beaconId\" : \"${nowBeacon.value}\"}")
                }
            }
        }
        else if (nowBeacon.value != null) {
            Log.d(CONSTANTS.TAG, "beacon deactivated 4")
            webSocket.send("{\"intent\" : \"deactivateBeacon\", \"beaconId\" : \"${nowBeacon.value}\"}")
            nowBeacon.value = null
        }

    }

    @SuppressLint("SetTextI18n")
    private val changeNowBeaconObserver: ((String?) -> Unit) = {
        binding.viewNowActiveBeacon.text = resources.getString(R.string.viewNowBeaconActiveText) + " ${nowBeacon.value}"
    }

    private val btnCommitBeaconObserver = { _: View ->
        // не забыть удалить маячок из списка пройденных маячков
        // а лучше просто убирать маячок из маски библиотеки которая отбираеть маячки
    }

}