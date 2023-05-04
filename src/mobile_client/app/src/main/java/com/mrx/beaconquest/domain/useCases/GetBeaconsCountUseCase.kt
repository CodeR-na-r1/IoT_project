package com.mrx.beaconquest.domain.useCases

import com.mrx.beaconquest.domain.interfaces.GetBeaconsCountInterface

class GetBeaconsCountUseCase(private val beaconsCountManager: GetBeaconsCountInterface) {

    fun exec() : Int {
        return beaconsCountManager.getBeaconsCount()
    }

}