package com.mrx.questwithbeacons.retrofit.interfaces

import com.mrx.questwithbeacons.CONSTANTS
import com.mrx.questwithbeacons.models.BeaconsCount
import com.mrx.questwithbeacons.models.BeaconsList
import retrofit2.http.GET

interface RetrofitServices {

    @GET(CONSTANTS.URLS.PATH_TO_COUNT_BEACONS)
    suspend fun getBeaconsCount() : BeaconsCount

    @GET(CONSTANTS.URLS.PATH_TO_LIST_BEACONS)
    suspend fun getBeaconsList() : BeaconsList

}