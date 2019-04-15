#include <stdio.h>
#include <windows.h>

int main()
{
	int present_time;
	int time_for_alarm;
	int hours;
	int minutes;
	int seconds;

	printf("Enter the your starting time HHMMSS \n ");
	scanf("%d", &present_time);

	printf("Enter the time for Alarm time HHMMSS \n");
	scanf("%d", &time_for_alarm);

	hours = present_time / 10000;
	minutes = ((present_time - (hours * 10000))) / 100;
	seconds = (present_time - (hours * 10000)) % 100;

	while (present_time <= time_for_alarm || present_time > time_for_alarm)
	{
		printf("Present time is: %02d:%02d:%02d \n", hours, minutes, seconds);
		Sleep(1000);
		present_time++;

		hours = present_time / 10000;
		minutes = ((present_time - (hours * 10000))) / 100;
		seconds = (present_time - (hours * 10000)) % 100;

		if (seconds >= 60)
		{
			minutes++;
			seconds = 0;
		}
		if (minutes >= 60)
		{
			hours++;
			minutes = 0;
		}
		if (hours >= 24)
		{
			hours = 0;
		}

		present_time = (hours * 10000) + (minutes * 100) + (seconds);


		if (time_for_alarm == present_time)
		{
			printf(&hours);
			printf("ALARM .. ALARM .. ALARM \n");
			break;
		}

	}

	return 0;
}