package com.mrx.beaconquest.data.retrofit.interfaces

import com.mrx.beaconquest.presentation.CONSTANTS
import com.mrx.beaconquest.data.retrofit.models.BeaconsCount
import com.mrx.beaconquest.data.retrofit.models.BeaconsList
import retrofit2.http.GET

interface RetrofitServices {

    @GET(CONSTANTS.URLS.PATH_TO_COUNT_BEACONS)
    suspend fun getBeaconsCount() : BeaconsCount

    @GET(CONSTANTS.URLS.PATH_TO_LIST_BEACONS)
    suspend fun getBeaconsList() : BeaconsList

}