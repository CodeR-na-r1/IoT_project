package com.mrx.beaconquest.presentation

import android.annotation.SuppressLint
import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.MutableLiveData
import com.mrx.beaconquest.R
import com.mrx.beaconquest.data.retrofit.RetrofitClient
import com.mrx.beaconquest.data.retrofit.interfaces.RetrofitServices
import com.mrx.beaconquest.data.retrofit.models.BeaconsCount
import com.mrx.beaconquest.data.retrofit.models.BeaconsList
import com.mrx.beaconquest.databinding.FragmentStartBinding
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class StartFragment(private val _context: Context) : Fragment() {

    companion object {
        fun newInstance(__context: Context) = StartFragment(__context)

        private const val TIME_UPDATE_DATA = 3000L

        private const val SERVER_OK = 0
        private const val SERVER_WAITING = 1
        private const val SERVER_ERROR = -1
    }

    private lateinit var binding: FragmentStartBinding

    private val handler = Handler(Looper.getMainLooper())

    private val statusServerLive = MutableLiveData<Int>(SERVER_WAITING)
    private var bCountLive = MutableLiveData<BeaconsCount>(BeaconsCount())
    private var bListLive = MutableLiveData<BeaconsList>(BeaconsList(listOf()))

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        binding = FragmentStartBinding.inflate(layoutInflater)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        this.statusServerLive.observe(this as LifecycleOwner, serverStatusObserver)
        this.bCountLive.observe(this as LifecycleOwner, beaconCountObserver)
        this.bListLive.observe(this as LifecycleOwner, beaconListObserver)

        binding.startButton.setOnClickListener(buttonObserver)

        handler.postDelayed(updateDataRunnable, 100L)
    }

    override fun onDestroyView() {
        super.onDestroyView()

        handler.removeCallbacks(updateDataRunnable)
    }

    private val buttonObserver: (View) -> Unit = { _: View ->
        if (this.bCountLive.value?.BeaconsCount!!.toInt() > 0 && this.statusServerLive.value == SERVER_OK) {
            handler.removeCallbacks(updateDataRunnable)
            parentFragmentManager.beginTransaction().replace(R.id.frameLayout, QuestProgressFragment.newInstance()).commit()
        }
        else {
            if (this.statusServerLive.value != SERVER_OK) {
                Toast.makeText(_context, "Server is not available!", Toast.LENGTH_LONG).show()
            }
            else {
                Toast.makeText(_context, "Beacons not found!", Toast.LENGTH_LONG).show()
            }
        }
    }

    private val serverStatusObserver = { newValue: Int ->
        when (newValue) {
            SERVER_OK -> {
                binding.serverStatusView.text = resources.getString(R.string.serverSuccess)
                binding.serverStatusView.setTextColor(resources.getColor(R.color.server_ok))
            }
            SERVER_WAITING -> {
                binding.serverStatusView.text = resources.getString(R.string.serverCheck)
                binding.serverStatusView.setTextColor(resources.getColor(R.color.server_waiting))
            }
            SERVER_ERROR -> {
                binding.serverStatusView.text = resources.getString(R.string.serverFail)
                binding.serverStatusView.setTextColor(resources.getColor(R.color.server_fail))
            }
        }
    }

    @SuppressLint("SetTextI18n")
    private val beaconCountObserver = { newValue: BeaconsCount ->
        binding.beaconsQuantityView.text = resources.getString(R.string.viewBeaconsQuantityText) + " " + newValue.BeaconsCount
    }

    private val beaconListObserver = { newValue: BeaconsList ->
        var resValues = resources.getString(R.string.viewBeaconsIdText) + " "

        var flag = false
        newValue.BeaconsList.forEach {
            if (flag) { resValues += ", " }

            flag = true

            resValues += it
        }

        if (!flag) { resValues += "[]" }

        binding.beaconsIDView.text = resValues
    }

    private fun updateDataSchedule() {
        handler.postDelayed(updateDataRunnable, TIME_UPDATE_DATA)
    }

    private val updateDataRunnable = Runnable {
        this.statusServerLive.value = SERVER_WAITING

        updateBeaconsData()

        if (this.statusServerLive.value != SERVER_ERROR) {
            this.statusServerLive.value = SERVER_OK
        }

        Log.d("myTag", "data updated")

        updateDataSchedule()
    }

    private val coroutineExceptionHandler = CoroutineExceptionHandler { _, exception ->
        activity?.runOnUiThread {
            Log.d("myTag", "exception -> $exception")
            this.statusServerLive.value = SERVER_ERROR
        }
    }

    @OptIn(DelicateCoroutinesApi::class)
    private fun updateBeaconsData() {
        val retrofit = RetrofitClient.getClient(CONSTANTS.URLS.BASE_URL)
        val retrofitAPI = retrofit.create(RetrofitServices::class.java)

        GlobalScope.launch(coroutineExceptionHandler) {
            val tempCount = retrofitAPI.getBeaconsCount()
            val tempList = retrofitAPI.getBeaconsList()

            activity?.runOnUiThread {
                bCountLive.value = tempCount
                bListLive.value = tempList
            }
        }
    }
}