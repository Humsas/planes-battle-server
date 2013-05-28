#ifndef FPS_LIMITER_H_
#define FPS_LIMITER_H_

#include <Windows.h>

class FPSLimiter
{
private:
	int fps;

	DWORD frame_start_time; ///++++

	int frame_times_array_len;			// frame times array len
	int frame_times_array_write_pos;	// frame times array write pos
	DWORD *frame_times;					// frame times array
	
	
	void addFrameToFrameTimesArray(DWORD frameTime)
	{
		if(frame_times_array_write_pos >= frame_times_array_len)
		{
			frame_times_array_write_pos = 0;
		}

		frame_times[frame_times_array_write_pos] = (frameTime+getAverageFrameTime())/2;
		frame_times_array_write_pos++;
	}


	float getAverageFrameTime()
	{
		int count = 0;
		DWORD frame_times_sum = 0;
		//std::cout << "---------------\n";
		for(int i = 0; i < frame_times_array_len; i++)
		{
			if(frame_times[i] != 0)
			{
				//std::cout << frame_times[i] << std::endl;
				frame_times_sum += frame_times[i];
				count++;
			}
		}

		//std::cout << "---------------\n";
		if(count == 0)
		{
			return 0.0f;
		}

		return frame_times_sum / (float)count;
	}




public:
	FPSLimiter(int fps = 60)
	{
		this->fps = fps;
		frame_start_time = timeGetTime();
		frame_times_array_len = 10;
		frame_times_array_write_pos = 0;
		frame_times = new DWORD[frame_times_array_len];

		for(int i = 0; i < frame_times_array_len; i++)
		{
			frame_times[i] = 0;
		}

	}

	~FPSLimiter()
	{
		delete frame_times;
	}


	void doSleep()
	{
		addFrameToFrameTimesArray(timeGetTime() - frame_start_time);

		float sleep_time = (1000.0 / fps) - getAverageFrameTime();
		if(sleep_time > 0 )
		{
			Sleep(sleep_time);
		}
		frame_start_time = timeGetTime();

	}


};

#endif