package com.mrx.questwithbeacons.domain.useCases

import com.mrx.questwithbeacons.domain.interfaces.GetBeaconsCountInterface

class GetBeaconsCountUseCase(private val beaconsCountManager: GetBeaconsCountInterface) {

    fun exec() : Int {
        return beaconsCountManager.getBeaconsCount()
    }

}