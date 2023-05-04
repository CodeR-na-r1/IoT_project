package com.mrx.beaconquest.domain.useCases

import com.mrx.beaconquest.domain.interfaces.GetBeaconsListInterface

class GetBeaconsListUseCase(private val beaconsListManager: GetBeaconsListInterface) {

    fun exec() : Collection<String> {
        return beaconsListManager.getBeaconsList()
    }

}