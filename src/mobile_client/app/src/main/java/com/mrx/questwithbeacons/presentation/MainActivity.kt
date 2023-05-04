package com.mrx.questwithbeacons.presentation

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import com.mrx.questwithbeacons.R
import com.mrx.questwithbeacons.databinding.ActivityMainBinding
import kotlinx.coroutines.CoroutineExceptionHandler

class MainActivity : AppCompatActivity() {

    private val binding by lazy { ActivityMainBinding.inflate(layoutInflater) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        this.setContentView(binding.root)

//        supportFragmentManager.beginTransaction().replace(R.id.frameLayout, StartFragment.newInstance(applicationContext)).commit()
    }
}