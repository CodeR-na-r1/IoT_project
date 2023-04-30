package com.mrx.questwithbeacons.presentation

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Toast
import com.mrx.indoorservice.api.IndoorService
import com.mrx.questwithbeacons.CONSTANTS
import com.mrx.questwithbeacons.databinding.ActivityMainBinding
import com.mrx.questwithbeacons.data.retrofit.RetrofitClient
import com.mrx.questwithbeacons.data.retrofit.interfaces.RetrofitServices
import com.mrx.questwithbeacons.data.retrofit.models.BeaconsCount
import com.mrx.questwithbeacons.data.retrofit.models.BeaconsList
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private val binding by lazy { ActivityMainBinding.inflate(layoutInflater) }

    private val indoorService by lazy { IndoorService.getInstance(applicationContext) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.setContentView(binding.root)

        updateBeaconsData()

        binding.buttonStart.setOnClickListener(listener)
    }

    private val listener = { view: View ->
        // toDo
    }

    private fun updateBeaconsData() {
        val retrofit = RetrofitClient.getClient(CONSTANTS.URLS.BASE_URL)
        val retrofitAPI = retrofit.create(RetrofitServices::class.java)

        var bCount: BeaconsCount
        var bList: BeaconsList

        GlobalScope.launch(handler) {
            bCount = retrofitAPI.getBeaconsCount()
            bList = retrofitAPI.getBeaconsList()

            runOnUiThread {
                binding.beaconsCount.text = "${bCount.BeaconsCount} ${bList.BeaconsList}"
            }
        }
    }

    private val handler = CoroutineExceptionHandler { _, exception ->
        Toast.makeText(applicationContext, "Exception: ${exception}", Toast.LENGTH_LONG).show()
    }
}