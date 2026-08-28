static const string KOTHWAVES_ITEM_PRESET_FILE = "$profile:KOTHWaves\\ItemPreset.json";

class KOTHWavesLootPresetConfig
{
	string Name;
	ref array<ref KOTHWavesLootConfig> Loot;
	
	void KOTHWavesLootPresetConfig()
	{
		Name = "";
		Loot = new array<ref KOTHWavesLootConfig>;
	}
}

class KOTHWavesItemPresets
{
	string Default_Preset;
	ref array<ref KOTHWavesLootPresetConfig> Presets;
	
	void KOTHWavesItemPresets()
	{
		Default_Preset = "military";
		Presets = new array<ref KOTHWavesLootPresetConfig>;
	}
	
	static void EnsureProfileFile()
	{
		if (!FileExist(KOTHWAVES_PROFILE_DIR))
		{
			MakeDirectory(KOTHWAVES_PROFILE_DIR);
		}
		
		if (FileExist(KOTHWAVES_ITEM_PRESET_FILE))
		{
			return;
		}
		
		KOTHWavesItemPresets presets = DefaultPresets();
		JsonFileLoader<KOTHWavesItemPresets>.JsonSaveFile(KOTHWAVES_ITEM_PRESET_FILE, presets);
		Print("[KOTHWaves] Default ItemPreset.json created at " + KOTHWAVES_ITEM_PRESET_FILE);
	}
	
	static KOTHWavesItemPresets Load()
	{
		EnsureProfileFile();
		
		KOTHWavesItemPresets presets = new KOTHWavesItemPresets;
		JsonFileLoader<KOTHWavesItemPresets>.JsonLoadFile(KOTHWAVES_ITEM_PRESET_FILE, presets);
		
		if (presets)
		{
			presets.Validate();
			return presets;
		}
		
		presets = DefaultPresets();
		JsonFileLoader<KOTHWavesItemPresets>.JsonSaveFile(KOTHWAVES_ITEM_PRESET_FILE, presets);
		Print("[KOTHWaves] ItemPreset.json was invalid — regenerated at " + KOTHWAVES_ITEM_PRESET_FILE);
		return presets;
	}
	
	void Validate()
	{
		if (!Presets)
		{
			Presets = new array<ref KOTHWavesLootPresetConfig>;
		}
		
		if (Default_Preset == "")
		{
			Default_Preset = "military";
		}
		
		if (Presets.Count() == 0)
		{
			AddDefaultPresetsTo(this);
			return;
		}
		
		foreach (KOTHWavesLootPresetConfig preset: Presets)
		{
			if (!preset)
			{
				continue;
			}
			
			if (!preset.Loot)
			{
				preset.Loot = new array<ref KOTHWavesLootConfig>;
			}
			
			KOTHWavesConfig.ValidateLootArray(preset.Loot);
		}
	}
	
	array<ref KOTHWavesLootConfig> GetLootByPresetName(string presetName)
	{
		if (!Presets || presetName == "")
		{
			return null;
		}
		
		for (int i = 0; i < Presets.Count(); i++)
		{
			KOTHWavesLootPresetConfig preset = Presets.Get(i);
			
			if (!preset || preset.Name == "")
			{
				continue;
			}
			
			if (preset.Name == presetName)
			{
				return preset.Loot;
			}
		}
		
		return null;
	}
	
	string ResolveLootPresetName(KOTHWavesKothConfig koth)
	{
		string presetName = "";
		
		if (koth)
		{
			presetName = KOTHWavesPresetPicker.PickPresetName(koth.Loot_Preset_Chances, koth.Loot_Preset, Default_Preset);
		}
		
		if (presetName == "")
		{
			presetName = Default_Preset;
		}
		
		return presetName;
	}
	
	array<ref KOTHWavesLootConfig> ResolveLootForKoth(KOTHWavesKothConfig koth, array<ref KOTHWavesLootConfig> fallbackLoot)
	{
		string presetName = ResolveLootPresetName(koth);
		array<ref KOTHWavesLootConfig> loot = GetLootByPresetName(presetName);
		
		if (loot && loot.Count() > 0)
		{
			return loot;
		}
		
		return fallbackLoot;
	}
	
	static KOTHWavesItemPresets DefaultPresets()
	{
		KOTHWavesItemPresets presets = new KOTHWavesItemPresets;
		AddDefaultPresetsTo(presets);
		return presets;
	}
	
	static void AddDefaultPresetsTo(KOTHWavesItemPresets presets)
	{
		if (!presets)
		{
			return;
		}
		
		presets.Presets.Clear();
		
		KOTHWavesLootPresetConfig military = new KOTHWavesLootPresetConfig;
		military.Name = "military";
		KOTHWavesConfig.AddDefaultMilitaryLoot(military.Loot);
		presets.Presets.Insert(military);
		
		KOTHWavesLootPresetConfig medical = new KOTHWavesLootPresetConfig;
		medical.Name = "medical";
		KOTHWavesConfig.AddDefaultMedicalLoot(medical.Loot);
		presets.Presets.Insert(medical);
		
		KOTHWavesLootPresetConfig basebuilding = new KOTHWavesLootPresetConfig;
		basebuilding.Name = "basebuilding";
		KOTHWavesConfig.AddDefaultBaseBuildingLoot(basebuilding.Loot);
		presets.Presets.Insert(basebuilding);
		
		KOTHWavesLootPresetConfig boss = new KOTHWavesLootPresetConfig;
		boss.Name = "boss";
		KOTHWavesConfig.AddDefaultBossLoot(boss.Loot);
		presets.Presets.Insert(boss);
		
		presets.Default_Preset = "military";
	}
}
