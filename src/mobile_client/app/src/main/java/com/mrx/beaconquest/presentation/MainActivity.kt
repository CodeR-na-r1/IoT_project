package com.mrx.beaconquest.presentation

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.mrx.beaconquest.R
import com.mrx.beaconquest.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private val binding by lazy { ActivityMainBinding.inflate(layoutInflater) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.setContentView(binding.root)

        supportFragmentManager.beginTransaction().replace(R.id.frameLayout, StartFragment.newInstance(applicationContext)).commit()
    }
}