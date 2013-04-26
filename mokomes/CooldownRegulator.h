#pragma once

class CooldownRegulator
{

private:
	double		m_TimeLeft;
	int			m_CooldownTimeInMS;

public:

	CooldownRegulator()
	{
	}

	CooldownRegulator(int cooldown)
	{
		m_CooldownTimeInMS = cooldown;
		m_TimeLeft = (double)m_CooldownTimeInMS/1000;
	}

	int GetCooldownTime()
	{
		return m_CooldownTimeInMS;
	}

	void SetCooldownTime(int cooldown)
	{
		m_CooldownTimeInMS = cooldown;
		m_TimeLeft = (double)m_CooldownTimeInMS/1000;
	}

	void UpdateCooldowns(double deltaTime)
	{
		if(m_TimeLeft < 0.0f)
			return;

		m_TimeLeft -= deltaTime;
	}

	bool isReady()
	{
		if(m_TimeLeft <= 0.0f)
		{
			m_TimeLeft = (double)m_CooldownTimeInMS/1000;
			return true;
		}

		return false;
	}

};