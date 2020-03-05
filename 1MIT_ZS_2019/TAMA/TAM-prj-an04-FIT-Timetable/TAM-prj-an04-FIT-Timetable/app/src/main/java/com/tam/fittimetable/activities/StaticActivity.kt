package com.tam.fittimetable.activities


import android.app.AlertDialog
import android.app.ProgressDialog
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.Observer
import com.alamkanak.weekview.DateTimeInterpreter
import com.alamkanak.weekview.OnRangeChangeListener
import com.alamkanak.weekview.WeekView
import com.tam.fittimetable.R
import com.tam.fittimetable.apicient.ApiEvent
import com.tam.fittimetable.backend.core.data.SubjectManager
import com.tam.fittimetable.data.EventsApi
import com.tam.fittimetable.data.EventsDatabase
import com.tam.fittimetable.data.FakeEventsApi
import com.tam.fittimetable.data.FakeEventsDatabase
import com.tam.fittimetable.util.*
import kotlinx.android.synthetic.main.activity_static.*
import kotlinx.android.synthetic.main.view_toolbar.*
import java.text.DateFormat
import java.text.SimpleDateFormat
import java.util.*

private data class AsyncViewState(
        val events: List<ApiEvent> = emptyList(),
        val isLoading: Boolean = false
)

private class AsyncViewModel(
        private val eventsApi: EventsApi
) {
    val viewState = MutableLiveData<AsyncViewState>()

    init {
        viewState.value = AsyncViewState(isLoading = true)
        fetchEvents()
    }

    fun fetchEvents() = eventsApi.fetchEvents {
        viewState.value = AsyncViewState(it)
    }

    fun remove(event: ApiEvent) {

        val allEvents = viewState.value?.events ?: return
        viewState.value = AsyncViewState(events = allEvents.minus(event))
    }
}


class StaticActivity : AppCompatActivity() {

    private val weekView: WeekView<ApiEvent> by lazyView(R.id.weekView)

    private val viewModel: AsyncViewModel by lazy {
        AsyncViewModel(FakeEventsApi(this))
    }

    @Suppress("DEPRECATION")
    private val progressDialog: ProgressDialog by lazy {
        ProgressDialog(this).apply {
            setCancelable(false)
            setMessage(getString(R.string.loading_courses))
        }
    }

    //private val weekView: WeekView<Event> by lazyView(R.id.weekView)

    private val database: EventsDatabase by lazy { FakeEventsDatabase(this) }
    private val dateFormatter = SimpleDateFormat.getDateInstance(DateFormat.MEDIUM, Locale.GERMANY)


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_static)

        toolbar.setupWithWeekView(weekView)

        viewModel.viewState.observe(this, Observer { viewState ->
            if (viewState.isLoading) {
                progressDialog.show()
            } else {
                progressDialog.dismiss()
            }

            weekView.submit(viewState.events)
        })

        weekView.setOnEventClickListener { event, _ ->
            showToast("${event.title}\n" +
                    getString(R.string.room)+": ${event.place}\n" +
                    getString(R.string.mess_id) +": ${event.id}")
        }

        weekView.setOnEventLongClickListener { event, _ ->
            val builder = AlertDialog.Builder(this)
            builder.setMessage(getString(R.string.message_delete_course))
            builder.setPositiveButton(getString(R.string.yes)) { dialog, which ->
                var success = SubjectManager.removeSubject(this, event.id.toString())
                if ( success){
                    viewModel.remove(event)
                    showToast( getString(R.string.message_course_deleted))
                } else {
                    showToast( getString(R.string.message_course_non_deleted))
                }


            }

            builder.setNegativeButton(getString(R.string.no), null)
            builder.show()
        }

        weekView.setOnEmptyViewLongClickListener { time ->
            val sdf = SimpleDateFormat.getDateTimeInstance()
            showToast("Empty view clicked at ${sdf.format(time.time)}")
        }


        weekView.onRangeChangeListener = object : OnRangeChangeListener {
            override fun onRangeChanged(
                    firstVisibleDate: Calendar,
                    lastVisibleDate: Calendar
            ) = updateDateText(firstVisibleDate, lastVisibleDate)
        }
        previousWeekButton.setOnClickListener {
            val cal = checkNotNull(weekView.firstVisibleDate)
            cal.add(Calendar.DATE, -7)
            weekView.goToDate(cal)
        }

        nextWeekButton.setOnClickListener {
            val cal = checkNotNull(weekView.firstVisibleDate)
            cal.add(Calendar.DATE, 7)
            weekView.goToDate(cal)
        }

        setupDateTimeInterpreter(false)
    }
    /**
     * Set up a date time interpreter which will show short date values when in week view and long
     * date values otherwise.
     * @param shortDate True if the date values should be short.
     */
    private fun setupDateTimeInterpreter(shortDate: Boolean) {
        weekView.dateTimeInterpreter = object : DateTimeInterpreter {
            override fun interpretDate(date: Calendar): String {
                val cal = Calendar.getInstance()
                val loc = Locale("cs", "CZ")
                val weekdayNameFormat = SimpleDateFormat("EEE", loc)
                var weekday = weekdayNameFormat.format(date.time)
                val format = SimpleDateFormat(" d.M", Locale.getDefault())

                // All android api level do not have a standard way of getting the first letter of
                // the week day name. Hence we get the first char programmatically.
                // Details: http://stackoverflow.com/questions/16959502/get-one-letter-abbreviation-of-week-day-of-a-date-in-java#answer-16959657
                if (shortDate)
                    weekday = weekday[0].toString()
                return weekday.toUpperCase() + format.format(date.time)
            }

            override fun interpretTime(hour: Int): String {
                //if (hour >= 8 && hour <=20){
                return "$hour:00"
                //}

            }
        }
    }
    override fun onBackPressed() {
        moveTaskToBack(true)
    }


   /* override fun onMonthChange(
            startDate: Calendar,
            endDate: Calendar
    ) = database.getEventsInRange(startDate, endDate)

    override fun onEventClick(event: Event, eventRect: RectF) {
        showToast("Clicked ${event.title}")
    }

    override fun onEventLongClick(event: Event, eventRect: RectF) {
        showToast("Long-clicked ${event.title}")
        Toast.makeText(this, "Long pressed event: " + event.title, Toast.LENGTH_SHORT).show()
    }

    override fun onEmptyViewLongClick(time: Calendar) {
        val sdf = SimpleDateFormat.getDateTimeInstance()
        showToast("Empty view clicked at ${sdf.format(time.time)}")
    }*/

    internal fun updateDateText(firstVisibleDate: Calendar, lastVisibleDate: Calendar) {
        val formattedFirstDay = dateFormatter.format(firstVisibleDate.time)
        val formattedLastDay = dateFormatter.format(lastVisibleDate.time)
        dateRangeTextView.text = getString(R.string.date_infos, formattedFirstDay, formattedLastDay)
    }
}
