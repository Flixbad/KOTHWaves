static const string KOTHWAVES_WAVE_PRESET_FILE = "$profile:KOTHWaves\\WavePreset.json";

class KOTHWavesWavePresetConfig
{
	string Name;
	ref array<ref KOTHWavesEnemyConfig> Enemies;
	ref array<ref KOTHWavesWaveConfig> Waves;
	
	void KOTHWavesWavePresetConfig()
	{
		Name = "";
		Enemies = new array<ref KOTHWavesEnemyConfig>;
		Waves = new array<ref KOTHWavesWaveConfig>;
	}
}

class KOTHWavesWavePresets
{
	string Default_Preset;
	ref array<ref KOTHWavesWavePresetConfig> Presets;
	
	void KOTHWavesWavePresets()
	{
		Default_Preset = "military_zombies";
		Presets = new array<ref KOTHWavesWavePresetConfig>;
	}
	
	static void EnsureProfileFile()
	{
		if (!FileExist(KOTHWAVES_PROFILE_DIR))
		{
			MakeDirectory(KOTHWAVES_PROFILE_DIR);
		}
		
		if (FileExist(KOTHWAVES_WAVE_PRESET_FILE))
		{
			return;
		}
		
		KOTHWavesWavePresets presets = DefaultPresets();
		JsonFileLoader<KOTHWavesWavePresets>.JsonSaveFile(KOTHWAVES_WAVE_PRESET_FILE, presets);
		Print("[KOTHWaves] Default WavePreset.json created at " + KOTHWAVES_WAVE_PRESET_FILE);
	}
	
	static KOTHWavesWavePresets Load()
	{
		EnsureProfileFile();
		
		KOTHWavesWavePresets presets = new KOTHWavesWavePresets;
		JsonFileLoader<KOTHWavesWavePresets>.JsonLoadFile(KOTHWAVES_WAVE_PRESET_FILE, presets);
		
		if (presets)
		{
			presets.Validate();
			return presets;
		}
		
		presets = DefaultPresets();
		JsonFileLoader<KOTHWavesWavePresets>.JsonSaveFile(KOTHWAVES_WAVE_PRESET_FILE, presets);
		Print("[KOTHWaves] WavePreset.json was invalid — regenerated at " + KOTHWAVES_WAVE_PRESET_FILE);
		return presets;
	}
	
	void Validate()
	{
		if (!Presets)
		{
			Presets = new array<ref KOTHWavesWavePresetConfig>;
		}
		
		if (Default_Preset == "")
		{
			Default_Preset = "military_zombies";
		}
		
		if (Presets.Count() == 0)
		{
			AddDefaultPresetsTo(this);
			return;
		}
		
		foreach (KOTHWavesWavePresetConfig preset: Presets)
		{
			if (!preset)
			{
				continue;
			}
			
			if (!preset.Enemies)
			{
				preset.Enemies = new array<ref KOTHWavesEnemyConfig>;
			}
			
			if (!preset.Waves)
			{
				preset.Waves = new array<ref KOTHWavesWaveConfig>;
			}
		}
	}
	
	KOTHWavesWavePresetConfig GetPresetByName(string presetName)
	{
		if (!Presets || presetName == "")
		{
			return null;
		}
		
		for (int i = 0; i < Presets.Count(); i++)
		{
			KOTHWavesWavePresetConfig preset = Presets.Get(i);
			
			if (!preset || preset.Name == "")
			{
				continue;
			}
			
			if (preset.Name == presetName)
			{
				return preset;
			}
		}
		
		return null;
	}
	
	string ResolvePresetName(KOTHWavesKothConfig koth)
	{
		string presetName = "";
		
		if (koth)
		{
			presetName = KOTHWavesPresetPicker.PickPresetName(koth.Wave_Preset_Chances, koth.Wave_Preset, Default_Preset);
		}
		
		if (presetName == "")
		{
			presetName = Default_Preset;
		}
		
		return presetName;
	}
	
	array<ref KOTHWavesWaveConfig> GetWavesForKoth(KOTHWavesKothConfig koth, array<ref KOTHWavesWaveConfig> fallbackWaves)
	{
		if (koth && koth.Waves && koth.Waves.Count() > 0)
		{
			return koth.Waves;
		}
		
		string presetName = ResolvePresetName(koth);
		KOTHWavesWavePresetConfig preset = GetPresetByName(presetName);
		
		if (preset && preset.Waves && preset.Waves.Count() > 0)
		{
			return preset.Waves;
		}
		
		return fallbackWaves;
	}
	
	array<ref KOTHWavesEnemyConfig> GetEnemiesForPresetName(string presetName, array<ref KOTHWavesEnemyConfig> fallbackEnemies)
	{
		if (presetName == "")
		{
			presetName = Default_Preset;
		}
		
		KOTHWavesWavePresetConfig preset = GetPresetByName(presetName);
		
		if (preset && preset.Enemies && preset.Enemies.Count() > 0)
		{
			return preset.Enemies;
		}
		
		return fallbackEnemies;
	}
	
	static KOTHWavesWavePresets DefaultPresets()
	{
		KOTHWavesWavePresets presets = new KOTHWavesWavePresets;
		AddDefaultPresetsTo(presets);
		return presets;
	}
	
	static void AddDefaultPresetsTo(KOTHWavesWavePresets presets)
	{
		if (!presets)
		{
			return;
		}
		
		presets.Presets.Clear();
		
		KOTHWavesWavePresetConfig military = new KOTHWavesWavePresetConfig;
		military.Name = "military_zombies";
		KOTHWavesConfig.AddDefaultWaveMilitaryEnemies(military.Enemies);
		presets.Presets.Insert(military);
		
		KOTHWavesWavePresetConfig infected = new KOTHWavesWavePresetConfig;
		infected.Name = "city_infected";
		KOTHWavesConfig.AddDefaultCityInfectedEnemies(infected.Enemies);
		presets.Presets.Insert(infected);
		
		KOTHWavesWavePresetConfig mutants = new KOTHWavesWavePresetConfig;
		mutants.Name = "mutants";
		KOTHWavesConfig.AddDefaultMutantEnemies(mutants.Enemies);
		presets.Presets.Insert(mutants);
		
		KOTHWavesWavePresetConfig wildlife = new KOTHWavesWavePresetConfig;
		wildlife.Name = "wildlife";
		KOTHWavesConfig.AddDefaultWildlifePredatorEnemies(wildlife.Enemies);
		presets.Presets.Insert(wildlife);
		
		KOTHWavesWavePresetConfig boss = new KOTHWavesWavePresetConfig;
		boss.Name = "boss";
		KOTHWavesConfig.AddDefaultBossEnemies(boss.Enemies);
		presets.Presets.Insert(boss);
		
		KOTHWavesWavePresetConfig legacyWildlife = new KOTHWavesWavePresetConfig;
		legacyWildlife.Name = "mutant_wildlife";
		KOTHWavesConfig.AddDefaultWildlifeEnemies(legacyWildlife.Enemies);
		presets.Presets.Insert(legacyWildlife);
		
		presets.Default_Preset = "military_zombies";
	}
}
