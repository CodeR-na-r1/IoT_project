package com.mrx.questwithbeacons

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.mrx.indoorservice.api.IndoorService
import com.mrx.questwithbeacons.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private val binding by lazy { ActivityMainBinding.inflate(layoutInflater) }

    private val indoorService by lazy { IndoorService.getInstance(applicationContext) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.setContentView(binding.root)


    }
}