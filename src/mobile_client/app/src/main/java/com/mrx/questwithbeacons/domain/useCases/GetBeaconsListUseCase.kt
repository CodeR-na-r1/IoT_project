package com.mrx.questwithbeacons.domain.useCases

import com.mrx.questwithbeacons.domain.interfaces.GetBeaconsListInterface

class GetBeaconsListUseCase(private val beaconsListManager: GetBeaconsListInterface) {

    fun exec() : Collection<String> {
        return beaconsListManager.getBeaconsList()
    }

}