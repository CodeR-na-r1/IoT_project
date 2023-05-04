package com.mrx.questwithbeacons.presentation

import android.content.Context
import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import com.mrx.questwithbeacons.CONSTANTS
import com.mrx.questwithbeacons.R
import com.mrx.questwithbeacons.data.retrofit.RetrofitClient
import com.mrx.questwithbeacons.data.retrofit.interfaces.RetrofitServices
import com.mrx.questwithbeacons.data.retrofit.models.BeaconsCount
import com.mrx.questwithbeacons.data.retrofit.models.BeaconsList
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class StartFragment(private val _context: Context) : Fragment() {

    companion object {
        fun newInstance(__context: Context) = StartFragment(__context)
    }

    private val statusServerLive = MutableLiveData<Int>()
    private var bCountLive = MutableLiveData<BeaconsCount>()
    private var bListLive = MutableLiveData<BeaconsList>()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_start, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        this.statusServerLive.observeForever {
            Toast.makeText(_context, "update live", Toast.LENGTH_LONG).show()
        }

//        this.statusServerLive.observe( this as LifecycleOwner) {
//
//        }
    }


    private val coroutineExceptionHandler = CoroutineExceptionHandler { _, exception ->
        activity?.runOnUiThread {
            this.statusServerLive.value = -1
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    private fun updateBeaconsData() {
        val retrofit = RetrofitClient.getClient(CONSTANTS.URLS.BASE_URL)
        val retrofitAPI = retrofit.create(RetrofitServices::class.java)

        GlobalScope.launch(coroutineExceptionHandler) {
            bCountLive.value = retrofitAPI.getBeaconsCount()
            bListLive.value = retrofitAPI.getBeaconsList()
        }
    }
}