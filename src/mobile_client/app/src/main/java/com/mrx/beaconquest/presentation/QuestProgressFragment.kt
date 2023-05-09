package com.mrx.beaconquest.presentation

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.lifecycle.LifecycleOwner
import com.mrx.beaconquest.R
import com.mrx.beaconquest.data.retrofit.models.BeaconsList
import com.mrx.beaconquest.databinding.FragmentQuestProgressBinding
import com.mrx.indoorservice.api.IndoorService
import com.mrx.indoorservice.domain.model.WiFiBeaconsEnvironmentInfo

class QuestProgressFragment(private val _context: Context, private val _beaconsList: BeaconsList) : Fragment() {

    companion object {
        fun newInstance(__context: Context, __beaconsList: BeaconsList) = QuestProgressFragment(_context = __context, _beaconsList = __beaconsList)
    }

    lateinit var binding: FragmentQuestProgressBinding

    private val indoorService by lazy { IndoorService.getInstance(context = _context) }
    private var nowBeacon: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        binding = FragmentQuestProgressBinding.inflate(layoutInflater)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

//        toDo create sockets and connect

        indoorService.WiFiBeaconsEnvironment.setScanInterval(1700L)
        indoorService.WiFiBeaconsEnvironment.addSSIDFilterForSpecificScan(this._beaconsList.BeaconsList)
        indoorService.WiFiBeaconsEnvironment.getSpecificScanViewModel().observe(this as LifecycleOwner, wifiScanObserver)
        indoorService.WiFiBeaconsEnvironment.startSpecificScanning()

        binding.btnCommitBeacon.setOnClickListener(this.btnCommitBeaconObserver)
    }

    override fun onDestroyView() {
        super.onDestroyView()

        indoorService.WiFiBeaconsEnvironment.stopSpecificScanning()
    }

    @SuppressLint("SetTextI18n")
    private val wifiScanObserver = { newData: Collection<WiFiBeaconsEnvironmentInfo> ->

        if (newData.isNotEmpty()) {

            val elem = newData.minBy { it.rssi }
            if (nowBeacon != null) {
                if (elem.rssi <= CONSTANTS.BEACONS.MIN_RSSI && nowBeacon != elem.ssid) {
                    // toDo deactive old beacon
                    nowBeacon = elem.ssid
                    // toDo activate new beacon
                }
                else if ((newData.find { it.ssid == nowBeacon }?.rssi ?: CONSTANTS.BEACONS.MAX_RSSI) >= CONSTANTS.BEACONS.MAX_RSSI) {
                    // toDo deactivate beacon
                    nowBeacon = null
                }
            }
            else {
                true
            }

            if (elem.rssi < CONSTANTS.BEACONS.MIN_RSSI) {
                if (elem.ssid != nowBeacon) {
                    // deactivate beacon
                }
            }
        }
        else {
            nowBeacon = null
            binding.viewNowActiveBeacon.text = resources.getString(R.string.viewNowBeaconActiveText) + " None"
        }


    }

    private val btnCommitBeaconObserver = { _: View ->

    }

}