package com.tam.fittimetable.data

import android.app.Activity
import android.content.Context
import android.os.AsyncTask
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.tam.fittimetable.apicient.ApiEvent
import com.tam.fittimetable.backend.core.data.Strings
import java.lang.Thread.sleep

interface EventsApi {
    fun fetchEvents(onSuccess: (List<ApiEvent>) -> Unit)
}

class FakeEventsApi(
    private val context: Context
) : EventsApi {

    private val responseType = object : TypeToken<List<ApiEvent>>() {}.type

    override fun fetchEvents(
        onSuccess: (List<ApiEvent>) -> Unit
    ) {
        AsyncTask.execute {
            sleep(2_000)
          val inputStream = context.openFileInput(Strings.FILE_NAME)
          //  val inputStream = context.assets.open("subjects.json")
            val json = inputStream.reader().readText()
            //get()

            val activity = context as Activity
            activity.runOnUiThread {
                onSuccess(Gson().fromJson(json, responseType))
            }
        }
    }
}
