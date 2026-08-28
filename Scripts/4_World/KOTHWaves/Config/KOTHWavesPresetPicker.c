class KOTHWavesPresetChanceConfig
{
	string Preset;
	float Chance;
	
	void KOTHWavesPresetChanceConfig()
	{
		Preset = "";
		Chance = 0.0;
	}
}

class KOTHWavesPresetPicker
{
	static string PickPresetName(array<ref KOTHWavesPresetChanceConfig> chances, string singlePreset, string defaultPreset)
	{
		if (chances && chances.Count() > 0)
		{
			float totalWeight = 0.0;
			
			for (int i = 0; i < chances.Count(); i++)
			{
				KOTHWavesPresetChanceConfig entry = chances.Get(i);
				
				if (!entry || entry.Preset == "" || entry.Chance <= 0.0)
				{
					continue;
				}
				
				totalWeight = totalWeight + entry.Chance;
			}
			
			if (totalWeight > 0.0)
			{
				float roll = Math.RandomFloatInclusive(0.0, totalWeight);
				float cursor = 0.0;
				
				for (int j = 0; j < chances.Count(); j++)
				{
					KOTHWavesPresetChanceConfig pickEntry = chances.Get(j);
					
					if (!pickEntry || pickEntry.Preset == "" || pickEntry.Chance <= 0.0)
					{
						continue;
					}
					
					cursor = cursor + pickEntry.Chance;
					
					if (roll <= cursor)
					{
						return pickEntry.Preset;
					}
				}
			}
		}
		
		if (singlePreset != "")
		{
			return singlePreset;
		}
		
		return defaultPreset;
	}
}
