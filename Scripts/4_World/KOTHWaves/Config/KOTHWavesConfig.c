static const string KOTHWAVES_PROFILE_DIR = "$profile:KOTHWaves";
static const string KOTHWAVES_CONFIG_FILE = "$profile:KOTHWaves\\KOTHWavesConfig.json";

class KOTHWavesSettings
{
	float Config_Version;
	int Minimum_Players;
	int Cleanup_Radius;
	int Cleanup_Time_After_KOTH_Finished;
	int Reward_Delete_On_Cleanup;
	int Players_In_Radius_Check_Range;
	int Max_Simultaneous;
	int Start_Delay;
	int Starting_Failed_Retry_Delay;
	int Time_Between;
	int Time_Limit;
	int Spawn_Start_Delay;
	int Capture_Time;
	float Capture_Speed_Bonus_PerPlayer_PerSec;
	float Capture_Speed_Bonus_PerPlayer_PerSec_Max;
	int Flag_Lower_If_Abandoned;
	int Map_Marker_Enabled;
	float LB_Marker_Radius;
	int Perspective_Marker_Enabled;
	int Notification_Enabled;
	int Logging_Enabled;
	int Debug_QuickMode;
	int Spawn_Outdoor_Only;
	ref array<string> Flags;
	string Flag_ClassName;
	string Reward_Fallback_ClassName;
	int Reward_Despawn_Seconds;
	int Fireworks_Enabled;
	int Flag_Smoke_Enabled;
	string Fireworks_ClassName;
	string Marker_Color_Empty;
	string Marker_Color_Capturing;
	string Marker_Color_Finished;
	string Marker_Icon;
	int Marker_Display_3D;
	int Marker_Display_Map;
	int Marker_Display_GPS;
	string Notif_Spawn_Title;
	string Notif_Spawn_Body;
	string Notif_Wave_Title;
	string Notif_Wave_Body;
	string Notif_Progress_Title;
	string Notif_Progress_Body;
	string Notif_Victory_Title;
	string Notif_Victory_Body;
	string Notif_Defeat_Title;
	string Notif_Defeat_Body;
	string Notif_Cleanup_Title;
	string Notif_Cleanup_Body;
	string Notif_Flag_Error_Title;
	string Notif_Flag_Error_Body;
	string Notif_Reward_Error_Title;
	string Notif_Reward_Error_Body;
	float Notif_Duration_Spawn;
	float Notif_Duration_Wave;
	float Notif_Duration_Progress;
	float Notif_Duration_Victory;
	float Notif_Duration_Defeat;
	float Notif_Duration_Cleanup;
	float Notif_Duration_Error;
	
	void KOTHWavesSettings()
	{
		Config_Version = 1.0;
		Minimum_Players = 0;
		Cleanup_Radius = 5;
		Cleanup_Time_After_KOTH_Finished = 300;
		Reward_Delete_On_Cleanup = 1;
		Players_In_Radius_Check_Range = 200;
		Max_Simultaneous = 1;
		Start_Delay = 600;
		Starting_Failed_Retry_Delay = 120;
		Time_Between = 1800;
		Time_Limit = 1200;
		Spawn_Start_Delay = 0;
		Capture_Time = 900;
		Capture_Speed_Bonus_PerPlayer_PerSec = 0.25;
		Capture_Speed_Bonus_PerPlayer_PerSec_Max = 2.0;
		Flag_Lower_If_Abandoned = 1;
		Map_Marker_Enabled = 1;
		LB_Marker_Radius = 150.0;
		Perspective_Marker_Enabled = 0;
		Notification_Enabled = 1;
		Logging_Enabled = 1;
		Debug_QuickMode = 0;
		Spawn_Outdoor_Only = 1;
		Flag_ClassName = "KOTHWavesTerritoryFlag";
		Reward_Fallback_ClassName = "KOTHWavesRewardCrate";
		Reward_Despawn_Seconds = 600;
		Fireworks_Enabled = 1;
		Flag_Smoke_Enabled = 1;
		Fireworks_ClassName = "FireworksLauncher";
		Marker_Color_Empty = "Yellow";
		Marker_Color_Capturing = "White";
		Marker_Color_Finished = "Black";
		Marker_Icon = "King";
		Marker_Display_3D = 0;
		Marker_Display_Map = 1;
		Marker_Display_GPS = 1;
		ApplyDefaultNotificationTexts(false);
		ApplyDefaultNotificationDurations(false);
	}
	
	void ApplyDefaultNotificationTexts(bool onlyMissing)
	{
		if (!onlyMissing || Notif_Spawn_Title == "")
		{
			Notif_Spawn_Title = "Roi de la colline";
		}
		
		if (!onlyMissing || Notif_Spawn_Body == "")
		{
			Notif_Spawn_Body = "KOTH actif à %1. Prenez le drapeau, survivez aux vagues — c'est parti !";
		}
		
		if (!onlyMissing || Notif_Wave_Title == "")
		{
			Notif_Wave_Title = "Vague incoming";
		}
		
		if (!onlyMissing || Notif_Wave_Body == "")
		{
			Notif_Wave_Body = "%2 sur %1 : %3 hostiles détectés. Tenez la ligne !";
		}
		
		if (!onlyMissing || Notif_Progress_Title == "")
		{
			Notif_Progress_Title = "Capture en cours";
		}
		
		if (!onlyMissing || Notif_Progress_Body == "")
		{
			Notif_Progress_Body = "%1 — %2%% capturé. Ne lâchez rien !";
		}
		
		if (!onlyMissing || Notif_Victory_Title == "")
		{
			Notif_Victory_Title = "Victoire !";
		}
		
		if (!onlyMissing || Notif_Victory_Body == "")
		{
			Notif_Victory_Body = "%2 remporte %1 ! Loot au sol — récupérez-le avant %3.";
		}
		
		if (!onlyMissing || Notif_Defeat_Title == "")
		{
			Notif_Defeat_Title = "Temps écoulé — %1";
		}
		
		if (!onlyMissing || Notif_Defeat_Body == "")
		{
			Notif_Defeat_Body = "Plus de temps à %1. Drapeau perdu. Prochain round bientôt.";
		}
		
		if (!onlyMissing || Notif_Cleanup_Title == "")
		{
			Notif_Cleanup_Title = "KOTH terminé";
		}
		
		if (!onlyMissing || Notif_Cleanup_Body == "")
		{
			Notif_Cleanup_Body = "%1 fermé. Merci pour le show !";
		}
		
		if (!onlyMissing || Notif_Flag_Error_Title == "")
		{
			Notif_Flag_Error_Title = "Erreur KOTH";
		}
		
		if (!onlyMissing || Notif_Flag_Error_Body == "")
		{
			Notif_Flag_Error_Body = "Drapeau introuvable à %1 (classe : %2). Vérifiez Flag_ClassName.";
		}
		
		if (!onlyMissing || Notif_Reward_Error_Title == "")
		{
			Notif_Reward_Error_Title = "Récompense — %1";
		}
		
		if (!onlyMissing || Notif_Reward_Error_Body == "")
		{
			Notif_Reward_Error_Body = "Caisse de loot impossible à spawner. Vérifiez les classnames conteneur.";
		}
	}
	
	void ApplyDefaultNotificationDurations(bool onlyMissing)
	{
		if (!onlyMissing || Notif_Duration_Spawn <= 0.0)
		{
			Notif_Duration_Spawn = 12.0;
		}
		
		if (!onlyMissing || Notif_Duration_Wave <= 0.0)
		{
			Notif_Duration_Wave = 10.0;
		}
		
		if (!onlyMissing || Notif_Duration_Progress <= 0.0)
		{
			Notif_Duration_Progress = 8.0;
		}
		
		if (!onlyMissing || Notif_Duration_Victory <= 0.0)
		{
			Notif_Duration_Victory = 14.0;
		}
		
		if (!onlyMissing || Notif_Duration_Defeat <= 0.0)
		{
			Notif_Duration_Defeat = 12.0;
		}
		
		if (!onlyMissing || Notif_Duration_Cleanup <= 0.0)
		{
			Notif_Duration_Cleanup = 8.0;
		}
		
		if (!onlyMissing || Notif_Duration_Error <= 0.0)
		{
			Notif_Duration_Error = 12.0;
		}
	}
}

class KOTHWavesNotifFormat
{
	static string Msg(string template, string p1 = "", string p2 = "", string p3 = "", string p4 = "")
	{
		if (!template || template.Length() == 0)
		{
			return "";
		}
		
		if (template.IndexOf("%") < 0)
		{
			return template;
		}
		
		return string.Format(template, p1, p2, p3, p4);
	}
}

class KOTHWavesEnemyConfig
{
	string ClassName;
	float SpawnRarity;
	int Difficulty;
	
	void KOTHWavesEnemyConfig()
	{
		ClassName = "";
		SpawnRarity = 0;
		Difficulty = 1;
	}
}

class KOTHWavesWaveSpawnGroupConfig
{
	string Preset;
	int Count;
	int CountPerType;
	int DifficultyMin;
	int DifficultyMax;
	
	void KOTHWavesWaveSpawnGroupConfig()
	{
		Preset = "";
		Count = 0;
		CountPerType = 0;
		DifficultyMin = 0;
		DifficultyMax = 0;
	}
	
	bool UsesCountPerType()
	{
		return CountPerType > 0;
	}
	
	bool HasDifficultyFilter()
	{
		return DifficultyMin > 0 || DifficultyMax > 0;
	}
	
	int GetDifficultyMin(KOTHWavesWaveConfig wave)
	{
		if (DifficultyMin > 0)
		{
			return DifficultyMin;
		}
		
		if (wave)
		{
			return wave.DifficultyMin;
		}
		
		return 1;
	}
	
	int GetDifficultyMax(KOTHWavesWaveConfig wave)
	{
		if (DifficultyMax > 0)
		{
			return DifficultyMax;
		}
		
		if (wave)
		{
			return wave.DifficultyMax;
		}
		
		return 5;
	}
}

class KOTHWavesWaveConfig
{
	string Name;
	int DelaySeconds;
	string Enemy_Preset;
	int ZombieCount;
	int DifficultyMin;
	int DifficultyMax;
	float SpawnRadiusMin;
	float SpawnRadiusMax;
	ref array<ref KOTHWavesWaveSpawnGroupConfig> Spawns;
	
	void KOTHWavesWaveConfig()
	{
		Name = "";
		DelaySeconds = 0;
		Enemy_Preset = "";
		ZombieCount = 10;
		DifficultyMin = 1;
		DifficultyMax = 5;
		SpawnRadiusMin = 18.0;
		SpawnRadiusMax = 45.0;
		Spawns = new array<ref KOTHWavesWaveSpawnGroupConfig>;
	}
	
	bool HasCustomSpawns()
	{
		return Spawns && Spawns.Count() > 0;
	}
	
	bool HasEnemyPreset()
	{
		return Enemy_Preset != "";
	}
}

class KOTHWavesStaticObjectConfig
{
	string ClassName;
	float Scale;
	ref array<float> Position;
	ref array<float> Rotation;
	
	void KOTHWavesStaticObjectConfig()
	{
		ClassName = "";
		Scale = 1.0;
		Position = new array<float>;
		Rotation = new array<float>;
	}
}

class KOTHWavesRewardContainerConfig
{
	string ClassName;
	int Is_Scripted_Reward;
	
	void KOTHWavesRewardContainerConfig()
	{
		ClassName = "SeaChest";
		Is_Scripted_Reward = 0;
	}
}

class KOTHWavesLootConfig
{
	string ClassName;
	string Magazine_ClassName;
	float Spawn_Chance;
	int Max_Spawnable;
	int Quantity;
	ref array<string> Attach;
	ref array<string> Extra_Items;
	ref array<string> Alternatives;
	
	void KOTHWavesLootConfig()
	{
		ClassName = "";
		Magazine_ClassName = "";
		Spawn_Chance = 100;
		Max_Spawnable = 1;
		Quantity = -1;
		Attach = new array<string>;
		Extra_Items = new array<string>;
		Alternatives = new array<string>;
	}
}

class KOTHWavesKothConfig
{
	string Name;
	ref array<float> Reward_Position;
	ref array<float> Reward_Orientation;
	int Capture_Radius;
	int Reward_Loot_Count;
	string Loot_Preset;
	ref array<ref KOTHWavesPresetChanceConfig> Loot_Preset_Chances;
	string Wave_Preset;
	ref array<ref KOTHWavesPresetChanceConfig> Wave_Preset_Chances;
	string Victory_Loot_Preset;
	ref array<string> Reward_Container_Classnames;
	ref array<ref KOTHWavesStaticObjectConfig> Static_Objects;
	ref array<ref KOTHWavesWaveConfig> Waves;
	
	void KOTHWavesKothConfig()
	{
		Name = "";
		Reward_Position = new array<float>;
		Reward_Orientation = new array<float>;
		Capture_Radius = 40;
		Reward_Loot_Count = 6;
		Loot_Preset = "";
		Loot_Preset_Chances = new array<ref KOTHWavesPresetChanceConfig>;
		Wave_Preset = "";
		Wave_Preset_Chances = new array<ref KOTHWavesPresetChanceConfig>;
		Victory_Loot_Preset = "boss";
		Reward_Container_Classnames = new array<string>;
		Static_Objects = new array<ref KOTHWavesStaticObjectConfig>;
		Waves = new array<ref KOTHWavesWaveConfig>;
	}
	
	vector GetPosition()
	{
		return KOTHWavesConfig.ArrayToVector(Reward_Position);
	}
	
	vector GetOrientation()
	{
		return KOTHWavesConfig.ArrayToVector(Reward_Orientation);
	}
}

class KOTHWavesConfig
{
	ref KOTHWavesSettings Settings;
	ref array<ref KOTHWavesEnemyConfig> Enemies;
	ref array<ref KOTHWavesWaveConfig> Default_Waves;
	ref array<ref KOTHWavesKothConfig> KOTHs;
	ref array<ref KOTHWavesRewardContainerConfig> Reward_Containers;
	ref array<ref KOTHWavesLootConfig> Loot;
	
	void KOTHWavesConfig()
	{
		Settings = new KOTHWavesSettings;
		Enemies = new array<ref KOTHWavesEnemyConfig>;
		Default_Waves = new array<ref KOTHWavesWaveConfig>;
		KOTHs = new array<ref KOTHWavesKothConfig>;
		Reward_Containers = new array<ref KOTHWavesRewardContainerConfig>;
		Loot = new array<ref KOTHWavesLootConfig>;
	}
	
	static KOTHWavesConfig Load()
	{
		KOTHWavesConfig config;
		
		if (!FileExist(KOTHWAVES_PROFILE_DIR))
		{
			MakeDirectory(KOTHWAVES_PROFILE_DIR);
		}
		
		if (FileExist(KOTHWAVES_CONFIG_FILE))
		{
			config = new KOTHWavesConfig;
			JsonFileLoader<KOTHWavesConfig>.JsonLoadFile(KOTHWAVES_CONFIG_FILE, config);
			
			if (config)
			{
				config.Validate();
				KOTHWavesItemPresets.EnsureProfileFile();
				KOTHWavesWavePresets.EnsureProfileFile();
				return config;
			}
		}
		
		config = DefaultConfig();
		config.StripLegacyJsonFields();
		JsonFileLoader<KOTHWavesConfig>.JsonSaveFile(KOTHWAVES_CONFIG_FILE, config);
		Print("[KOTHWaves] Default KOTHWavesConfig.json created at " + KOTHWAVES_CONFIG_FILE);
		config.Validate();
		KOTHWavesItemPresets.EnsureProfileFile();
		KOTHWavesWavePresets.EnsureProfileFile();
		return config;
	}
	
	void Validate()
	{
		if (!Settings)
		{
			Settings = new KOTHWavesSettings;
		}
		
		if (!Enemies)
		{
			Enemies = new array<ref KOTHWavesEnemyConfig>;
		}
		
		if (!Default_Waves)
		{
			Default_Waves = new array<ref KOTHWavesWaveConfig>;
		}
		
		if (!KOTHs)
		{
			KOTHs = new array<ref KOTHWavesKothConfig>;
		}
		
		if (!Reward_Containers)
		{
			Reward_Containers = new array<ref KOTHWavesRewardContainerConfig>;
		}
		
		if (!Loot)
		{
			Loot = new array<ref KOTHWavesLootConfig>;
		}
		
		if (Settings.Flag_ClassName == "" || Settings.Flag_ClassName == "TerritoryFlag" || Settings.Flag_ClassName == "Flag_Bohemia" || Settings.Flag_ClassName == "Flag_Chernarus" || Settings.Flag_ClassName == "Flag_Pirates")
		{
			Settings.Flag_ClassName = "KOTHWavesTerritoryFlag";
		}
		
		if (Settings.Reward_Fallback_ClassName == "")
		{
			Settings.Reward_Fallback_ClassName = "KOTHWavesRewardCrate";
		}
		
		if (Settings.Reward_Fallback_ClassName == "WoodenCrate")
		{
			Settings.Reward_Fallback_ClassName = "KOTHWavesRewardCrate";
		}
		
		if (Settings.Reward_Despawn_Seconds <= 0)
		{
			Settings.Reward_Despawn_Seconds = 600;
		}
		
		if (Settings.Fireworks_ClassName == "")
		{
			Settings.Fireworks_ClassName = "FireworksLauncher";
		}
		
		if (Settings.Flag_Smoke_Enabled != 0 && Settings.Flag_Smoke_Enabled != 1)
		{
			Settings.Flag_Smoke_Enabled = 1;
		}
		
		if (!Settings.Flags)
		{
			Settings.Flags = new array<string>;
		}
		
		if (Settings.Flags.Count() == 0)
		{
			AddDefaultFlags(Settings.Flags);
		}
		
		if (Settings.Spawn_Start_Delay < 0)
		{
			Settings.Spawn_Start_Delay = 0;
		}
		
		if (Settings.Spawn_Outdoor_Only != 0 && Settings.Spawn_Outdoor_Only != 1)
		{
			Settings.Spawn_Outdoor_Only = 1;
		}
		
		if (Settings.Reward_Delete_On_Cleanup != 0 && Settings.Reward_Delete_On_Cleanup != 1)
		{
			Settings.Reward_Delete_On_Cleanup = 1;
		}
		
		Settings.ApplyDefaultNotificationTexts(true);
		Settings.ApplyDefaultNotificationDurations(true);
		
		foreach (KOTHWavesKothConfig koth: KOTHs)
		{
			if (!koth)
			{
				continue;
			}
			
			if (!koth.Reward_Position)
			{
				koth.Reward_Position = new array<float>;
			}
			
			if (!koth.Reward_Orientation)
			{
				koth.Reward_Orientation = new array<float>;
			}
			
			if (!koth.Reward_Container_Classnames)
			{
				koth.Reward_Container_Classnames = new array<string>;
			}
			
			for (int containerIndex = 0; containerIndex < koth.Reward_Container_Classnames.Count(); containerIndex++)
			{
				if (koth.Reward_Container_Classnames.Get(containerIndex) == "WoodenCrate")
				{
					koth.Reward_Container_Classnames.Set(containerIndex, "KOTHWavesRewardCrate");
				}
			}
			
			if (!koth.Static_Objects)
			{
				koth.Static_Objects = new array<ref KOTHWavesStaticObjectConfig>;
			}
			
			if (!koth.Waves)
			{
				koth.Waves = new array<ref KOTHWavesWaveConfig>;
			}
			
			ValidateWaveArray(koth.Waves);
			
			if (koth.Waves.Count() == 0)
			{
				AddDefaultProgressiveWaves(koth.Waves);
			}
			
			if (!koth.Loot_Preset_Chances)
			{
				koth.Loot_Preset_Chances = new array<ref KOTHWavesPresetChanceConfig>;
			}
			
			if (!koth.Wave_Preset_Chances)
			{
				koth.Wave_Preset_Chances = new array<ref KOTHWavesPresetChanceConfig>;
			}
			
			if (koth.Victory_Loot_Preset == "")
			{
				koth.Victory_Loot_Preset = "boss";
			}
			
			ValidatePresetChanceArray(koth.Loot_Preset_Chances);
			ValidatePresetChanceArray(koth.Wave_Preset_Chances);
			
			foreach (KOTHWavesStaticObjectConfig staticObject: koth.Static_Objects)
			{
				if (!staticObject)
				{
					continue;
				}
				
				if (!staticObject.Position)
				{
					staticObject.Position = new array<float>;
				}
				
				if (!staticObject.Rotation)
				{
					staticObject.Rotation = new array<float>;
				}
			}
		}
		
		ValidateLootArray(Loot);
	}
	
	static void ValidateWaveArray(array<ref KOTHWavesWaveConfig> waveArray)
	{
		if (!waveArray)
		{
			return;
		}
		
		foreach (KOTHWavesWaveConfig wave: waveArray)
		{
			if (!wave)
			{
				continue;
			}
			
			if (!wave.Spawns)
			{
				wave.Spawns = new array<ref KOTHWavesWaveSpawnGroupConfig>;
			}
			
			foreach (KOTHWavesWaveSpawnGroupConfig spawnGroup: wave.Spawns)
			{
				if (!spawnGroup)
				{
					continue;
				}
				
				if (spawnGroup.Count < 0)
				{
					spawnGroup.Count = 0;
				}
				
				if (spawnGroup.CountPerType < 0)
				{
					spawnGroup.CountPerType = 0;
				}
			}
		}
	}
	
	static void ValidatePresetChanceArray(array<ref KOTHWavesPresetChanceConfig> chanceArray)
	{
		if (!chanceArray)
		{
			return;
		}
		
		for (int i = chanceArray.Count() - 1; i >= 0; i--)
		{
			KOTHWavesPresetChanceConfig entry = chanceArray.Get(i);
			
			if (!entry)
			{
				chanceArray.Remove(i);
				continue;
			}
			
			if (entry.Preset == "")
			{
				chanceArray.Remove(i);
				continue;
			}
			
			if (entry.Chance < 0.0)
			{
				entry.Chance = 0.0;
			}
		}
	}
	
	static void ValidateLootArray(array<ref KOTHWavesLootConfig> lootArray)
	{
		if (!lootArray)
		{
			return;
		}
		
		foreach (KOTHWavesLootConfig loot: lootArray)
		{
			if (!loot)
			{
				continue;
			}
			
			if (!loot.Attach)
			{
				loot.Attach = new array<string>;
			}
			
			if (!loot.Extra_Items)
			{
				loot.Extra_Items = new array<string>;
			}
			
			if (!loot.Alternatives)
			{
				loot.Alternatives = new array<string>;
			}
		}
	}
	
	static vector ArrayToVector(array<float> values)
	{
		if (!values || values.Count() < 3)
		{
			return Vector(0, 0, 0);
		}
		
		return Vector(values.Get(0), values.Get(1), values.Get(2));
	}
	
	void StripLegacyJsonFields()
	{
		Enemies = null;
		Default_Waves = null;
		Loot = null;
	}
	
	static KOTHWavesConfig DefaultConfig()
	{
		KOTHWavesConfig config = new KOTHWavesConfig;
		AddDefaultFlags(config.Settings.Flags);
		AddDefaultKoths(config.KOTHs);
		AddDefaultContainers(config.Reward_Containers);
		return config;
	}
	
	static void AddDefaultFlags(array<string> flags)
	{
		if (!flags)
		{
			return;
		}
		
		flags.Clear();
		flags.Insert("Flag_Bohemia");
		flags.Insert("Flag_Chernarus");
		flags.Insert("Flag_Pirates");
	}
	
	static void AddDefaultWaves(array<ref KOTHWavesWaveConfig> waves)
	{
		KOTHWavesWaveConfig wave1 = new KOTHWavesWaveConfig;
		wave1.Name = "Wave 1";
		wave1.DelaySeconds = 0;
		wave1.ZombieCount = 10;
		wave1.DifficultyMin = 1;
		wave1.DifficultyMax = 2;
		wave1.SpawnRadiusMin = 18.0;
		wave1.SpawnRadiusMax = 35.0;
		waves.Insert(wave1);
		
		KOTHWavesWaveConfig wave2 = new KOTHWavesWaveConfig;
		wave2.Name = "Wave 2";
		wave2.DelaySeconds = 225;
		wave2.ZombieCount = 15;
		wave2.DifficultyMin = 1;
		wave2.DifficultyMax = 3;
		wave2.SpawnRadiusMin = 20.0;
		wave2.SpawnRadiusMax = 40.0;
		waves.Insert(wave2);
		
		KOTHWavesWaveConfig wave3 = new KOTHWavesWaveConfig;
		wave3.Name = "Wave 3";
		wave3.DelaySeconds = 450;
		wave3.ZombieCount = 20;
		wave3.DifficultyMin = 2;
		wave3.DifficultyMax = 4;
		wave3.SpawnRadiusMin = 22.0;
		wave3.SpawnRadiusMax = 45.0;
		waves.Insert(wave3);
		
		KOTHWavesWaveConfig wave4 = new KOTHWavesWaveConfig;
		wave4.Name = "Wave 4";
		wave4.DelaySeconds = 675;
		wave4.ZombieCount = 25;
		wave4.DifficultyMin = 3;
		wave4.DifficultyMax = 5;
		wave4.SpawnRadiusMin = 25.0;
		wave4.SpawnRadiusMax = 50.0;
		waves.Insert(wave4);
		
		KOTHWavesWaveConfig wave5 = new KOTHWavesWaveConfig;
		wave5.Name = "Wave 5";
		wave5.DelaySeconds = 900;
		wave5.ZombieCount = 30;
		wave5.DifficultyMin = 4;
		wave5.DifficultyMax = 5;
		wave5.SpawnRadiusMin = 28.0;
		wave5.SpawnRadiusMax = 55.0;
		waves.Insert(wave5);
	}
	
	static void AddDefaultEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddDefaultMilitaryZombieEnemies(enemies);
	}
	
	static void AddDefaultWaveMilitaryEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "ZmbM_PatrolNormal_Autumn", 0, 3);
		AddEnemy(enemies, "ZmbM_PatrolNormal_Summer", 0, 3);
		AddEnemy(enemies, "ZmbM_SoldierNormal", 0, 4);
		AddEnemy(enemies, "ZmbM_usSoldier_normal_Woodland", 0, 5);
		AddEnemy(enemies, "ZmbM_usSoldier_Officer_Desert", 0, 5);
	}
	
	static void AddDefaultMilitaryZombieEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "ZmbF_CitizenANormal_Beige", 0, 1);
		AddEnemy(enemies, "ZmbF_CitizenANormal_Blue", 0, 1);
		AddEnemy(enemies, "ZmbF_CitizenANormal_Brown", 0, 1);
		AddEnemy(enemies, "ZmbF_CitizenBSkinny", 0, 1);
		AddEnemy(enemies, "ZmbF_Clerk_Normal_Blue", 0, 1);
		AddEnemy(enemies, "ZmbF_Clerk_Normal_Green", 0, 1);
		AddEnemy(enemies, "ZmbF_Clerk_Normal_Red", 0, 1);
		AddEnemy(enemies, "ZmbF_Clerk_Normal_White", 0, 1);
		AddEnemy(enemies, "ZmbF_DoctorSkinny", 0, 2);
		AddEnemy(enemies, "ZmbF_JoggerSkinny_Blue", 0, 2);
		AddEnemy(enemies, "ZmbF_JoggerSkinny_Brown", 0, 2);
		AddEnemy(enemies, "ZmbF_JoggerSkinny_Green", 0, 2);
		AddEnemy(enemies, "ZmbF_JoggerSkinny_Red", 0, 2);
		AddEnemy(enemies, "ZmbF_JournalistNormal_Blue", 0, 1);
		AddEnemy(enemies, "ZmbF_JournalistNormal_Green", 0, 1);
		AddEnemy(enemies, "ZmbF_JournalistNormal_Red", 0, 1);
		AddEnemy(enemies, "ZmbF_JournalistNormal_White", 0, 1);
		AddEnemy(enemies, "ZmbF_NurseFat", 0, 2);
		AddEnemy(enemies, "ZmbF_ParamedicNormal_Blue", 0, 2);
		AddEnemy(enemies, "ZmbF_ParamedicNormal_Green", 0, 2);
		AddEnemy(enemies, "ZmbF_ParamedicNormal_Red", 0, 2);
		AddEnemy(enemies, "ZmbF_PatientOld", 0, 1);
		AddEnemy(enemies, "ZmbF_PoliceWomanNormal", 0, 3);
		AddEnemy(enemies, "ZmbF_ShortSkirt_beige", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_black", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_brown", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_checks", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_green", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_grey", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_red", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_stripes", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_white", 0, 1);
		AddEnemy(enemies, "ZmbF_ShortSkirt_yellow", 0, 1);
		AddEnemy(enemies, "ZmbF_SkaterYoung_Brown", 0, 2);
		AddEnemy(enemies, "ZmbF_SkaterYoung_Striped", 0, 2);
		AddEnemy(enemies, "ZmbF_SkaterYoung_Violet", 0, 2);
		AddEnemy(enemies, "ZmbM_CitizenASkinny_Blue", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenASkinny_Brown", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenASkinny_Grey", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenASkinny_Red", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenBFat_Blue", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenBFat_Green", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenBFat_Red", 0, 1);
		AddEnemy(enemies, "ZmbM_ClerkFat_Brown", 0, 1);
		AddEnemy(enemies, "ZmbM_ClerkFat_Grey", 0, 1);
		AddEnemy(enemies, "ZmbM_ClerkFat_Khaki", 0, 1);
		AddEnemy(enemies, "ZmbM_ClerkFat_White", 0, 1);
		AddEnemy(enemies, "ZmbM_CommercialPilotOld_Blue", 0, 2);
		AddEnemy(enemies, "ZmbM_CommercialPilotOld_Brown", 0, 2);
		AddEnemy(enemies, "ZmbM_CommercialPilotOld_Grey", 0, 2);
		AddEnemy(enemies, "ZmbM_CommercialPilotOld_Olive", 0, 2);
		AddEnemy(enemies, "ZmbM_DoctorFat", 0, 2);
		AddEnemy(enemies, "ZmbM_JacketNormal_greenchecks", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_beige", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_black", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_blue", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_bluechecks", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_brown", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_greenchecks", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_grey", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_khaki", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_magenta", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_stripes", 0, 1);
		AddEnemy(enemies, "ZmbM_JoggerSkinny_Blue", 0, 2);
		AddEnemy(enemies, "ZmbM_JoggerSkinny_Green", 0, 2);
		AddEnemy(enemies, "ZmbM_JoggerSkinny_Red", 0, 2);
		AddEnemy(enemies, "ZmbM_JournalistSkinny", 0, 1);
		AddEnemy(enemies, "ZmbM_ParamedicNormal_Black", 0, 2);
		AddEnemy(enemies, "ZmbM_ParamedicNormal_Blue", 0, 2);
		AddEnemy(enemies, "ZmbM_ParamedicNormal_Green", 0, 2);
		AddEnemy(enemies, "ZmbM_ParamedicNormal_Red", 0, 2);
		AddEnemy(enemies, "ZmbM_PatientSkinny", 0, 1);
		AddEnemy(enemies, "ZmbM_PatrolNormal_Autumn", 0, 3);
		AddEnemy(enemies, "ZmbM_PatrolNormal_Flat", 0, 3);
		AddEnemy(enemies, "ZmbM_PatrolNormal_PautRev", 0, 3);
		AddEnemy(enemies, "ZmbM_PatrolNormal_Summer", 0, 3);
		AddEnemy(enemies, "ZmbM_TerritoryNormal", 0, 4);
		AddEnemy(enemies, "ZmbM_PolicemanFat", 0, 4);
		AddEnemy(enemies, "ZmbM_PolicemanSpecForce", 0, 5);
		AddEnemy(enemies, "ZmbM_PrisonerSkinny", 0, 2);
		AddEnemy(enemies, "ZmbM_SkaterYoung_Blue", 0, 2);
		AddEnemy(enemies, "ZmbM_SkaterYoung_Brown", 0, 2);
		AddEnemy(enemies, "ZmbM_SkaterYoung_Green", 0, 2);
		AddEnemy(enemies, "ZmbM_SkaterYoung_Grey", 0, 2);
	}
	
	static void AddDefaultCityInfectedEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "ZmbF_CitizenANormal_Beige", 0, 1);
		AddEnemy(enemies, "ZmbF_CitizenANormal_Blue", 0, 1);
		AddEnemy(enemies, "ZmbM_CitizenASkinny_Blue", 0, 1);
		AddEnemy(enemies, "ZmbM_Jacket_black", 0, 2);
		AddEnemy(enemies, "ZmbM_JoggerSkinny_Blue", 0, 2);
		AddEnemy(enemies, "ZmbF_JoggerSkinny_Blue", 0, 2);
		AddEnemy(enemies, "ZmbM_ParamedicNormal_Blue", 0, 3);
		AddEnemy(enemies, "ZmbF_PoliceWomanNormal", 0, 3);
		AddEnemy(enemies, "ZmbM_PolicemanFat", 0, 4);
	}
	
	static void AddDefaultWildlifeEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "Animal_CanisLupus_Grey", 0, 2);
		AddEnemy(enemies, "Animal_CanisLupus_White", 0, 2);
		AddEnemy(enemies, "Animal_UrsusArctos", 0, 4);
		AddEnemy(enemies, "Animal_CapraHircusF", 0, 1);
		AddEnemy(enemies, "Animal_CapreolusCapreolus", 0, 1);
		AddEnemy(enemies, "Animal_SusScrofa", 0, 2);
		AddEnemy(enemies, "Animal_GallusGallusDomesticus", 0, 1);
	}
	
	static void AddDefaultWildlifePredatorEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "Animal_CanisLupus_Grey", 0, 3);
		AddEnemy(enemies, "Animal_CanisLupus_White", 0, 3);
		AddEnemy(enemies, "Animal_UrsusArctos", 0, 5);
	}
	
	static void AddDefaultMutantEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "Animal_UrsusArctos", 0, 4);
		AddEnemy(enemies, "Animal_CanisLupus_Grey", 0, 3);
		AddEnemy(enemies, "Animal_SusScrofa", 0, 3);
	}
	
	static void AddDefaultBossEnemies(array<ref KOTHWavesEnemyConfig> enemies)
	{
		AddEnemy(enemies, "Animal_UrsusArctos", 0, 5);
	}
	
	static void AddEnemy(array<ref KOTHWavesEnemyConfig> enemies, string className, float rarity, int difficulty)
	{
		KOTHWavesEnemyConfig enemy = new KOTHWavesEnemyConfig;
		enemy.ClassName = className;
		enemy.SpawnRarity = rarity;
		enemy.Difficulty = difficulty;
		enemies.Insert(enemy);
	}
	
	static void AddDefaultKoths(array<ref KOTHWavesKothConfig> koths)
	{
		KOTHWavesKothConfig zelen = NewKoth("Zelenogorsk Military", 2562.699951, 193.175003, 5117.337891, 0.0, 0.0, 0.0, "KOTHWavesRewardCrate", "military", "military_zombies");
		AddPresetChance(zelen.Loot_Preset_Chances, "military", 70);
		AddPresetChance(zelen.Loot_Preset_Chances, "medical", 30);
		AddDefaultProgressiveWaves(zelen.Waves);
		koths.Insert(zelen);
		
		KOTHWavesKothConfig elektro = NewKoth("Elektro Police Station", 10461.144531, 5.957000, 2258.447021, 0.0, 0.0, 0.0, "KOTHWavesRewardCrate", "medical", "military_zombies");
		AddPresetChance(elektro.Loot_Preset_Chances, "medical", 60);
		AddPresetChance(elektro.Loot_Preset_Chances, "military", 40);
		AddDefaultProgressiveWaves(elektro.Waves);
		koths.Insert(elektro);
		
		KOTHWavesKothConfig krasno = NewKoth("Krasnostav Airfield", 11833.782227, 341.458710, 12502.089844, 0.0, 0.0, 0.0, "KOTHWavesRewardCrate", "military", "military_zombies");
		AddPresetChance(krasno.Loot_Preset_Chances, "military", 70);
		AddPresetChance(krasno.Loot_Preset_Chances, "medical", 30);
		AddDefaultProgressiveWaves(krasno.Waves);
		koths.Insert(krasno);
	}
	
	static void AddDefaultProgressiveWaves(array<ref KOTHWavesWaveConfig> waves)
	{
		if (!waves)
		{
			return;
		}
		
		waves.Clear();
		
		KOTHWavesWaveConfig wave1 = new KOTHWavesWaveConfig;
		wave1.Name = "Vague 1 — Militaires";
		wave1.DelaySeconds = 0;
		wave1.Enemy_Preset = "military_zombies";
		wave1.ZombieCount = 25;
		wave1.DifficultyMin = 1;
		wave1.DifficultyMax = 4;
		wave1.SpawnRadiusMin = 18.0;
		wave1.SpawnRadiusMax = 35.0;
		waves.Insert(wave1);
		
		KOTHWavesWaveConfig wave2 = new KOTHWavesWaveConfig;
		wave2.Name = "Vague 2 — Civils";
		wave2.DelaySeconds = 225;
		wave2.Enemy_Preset = "city_infected";
		wave2.ZombieCount = 20;
		wave2.DifficultyMin = 1;
		wave2.DifficultyMax = 4;
		wave2.SpawnRadiusMin = 20.0;
		wave2.SpawnRadiusMax = 40.0;
		waves.Insert(wave2);
		
		KOTHWavesWaveConfig wave3 = new KOTHWavesWaveConfig;
		wave3.Name = "Vague 3 — Mutants";
		wave3.DelaySeconds = 450;
		wave3.SpawnRadiusMin = 22.0;
		wave3.SpawnRadiusMax = 45.0;
		AddWaveSpawnGroup(wave3, "mutants", 0, 2);
		waves.Insert(wave3);
		
		KOTHWavesWaveConfig wave4 = new KOTHWavesWaveConfig;
		wave4.Name = "Vague 4 — Faune";
		wave4.DelaySeconds = 675;
		wave4.SpawnRadiusMin = 25.0;
		wave4.SpawnRadiusMax = 50.0;
		AddWaveSpawnGroup(wave4, "wildlife", 0, 1);
		waves.Insert(wave4);
		
		KOTHWavesWaveConfig wave5 = new KOTHWavesWaveConfig;
		wave5.Name = "Vague 5 — Boss";
		wave5.DelaySeconds = 750;
		wave5.SpawnRadiusMin = 28.0;
		wave5.SpawnRadiusMax = 55.0;
		AddWaveSpawnGroup(wave5, "boss", 1, 0);
		AddWaveSpawnGroup(wave5, "military_zombies", 10, 0);
		waves.Insert(wave5);
	}
	
	static void AddWaveSpawnGroup(KOTHWavesWaveConfig wave, string presetName, int count, int countPerType)
	{
		if (!wave || presetName == "")
		{
			return;
		}
		
		KOTHWavesWaveSpawnGroupConfig group = new KOTHWavesWaveSpawnGroupConfig;
		group.Preset = presetName;
		group.Count = count;
		group.CountPerType = countPerType;
		wave.Spawns.Insert(group);
	}
	
	static KOTHWavesKothConfig NewKoth(string name, float x, float y, float z, float yaw, float pitch, float roll, string containerClassName, string lootPreset, string wavePreset)
	{
		KOTHWavesKothConfig koth = new KOTHWavesKothConfig;
		koth.Name = name;
		AddVector(koth.Reward_Position, x, y, z);
		AddVector(koth.Reward_Orientation, yaw, pitch, roll);
		koth.Capture_Radius = 40;
		koth.Reward_Loot_Count = 6;
		koth.Loot_Preset = lootPreset;
		koth.Wave_Preset = wavePreset;
		koth.Victory_Loot_Preset = "boss";
		koth.Reward_Container_Classnames.Insert(containerClassName);
		return koth;
	}
	
	static void AddPresetChance(array<ref KOTHWavesPresetChanceConfig> chances, string presetName, float chance)
	{
		if (!chances || presetName == "" || chance <= 0.0)
		{
			return;
		}
		
		KOTHWavesPresetChanceConfig entry = new KOTHWavesPresetChanceConfig;
		entry.Preset = presetName;
		entry.Chance = chance;
		chances.Insert(entry);
	}
	
	static void AddDefaultContainers(array<ref KOTHWavesRewardContainerConfig> containers)
	{
		AddContainer(containers, "KOTHWavesRewardCrate", 0);
		AddContainer(containers, "SeaChest", 0);
		AddContainer(containers, "Barrel_Green", 0);
	}
	
	static void AddContainer(array<ref KOTHWavesRewardContainerConfig> containers, string className, int scripted)
	{
		KOTHWavesRewardContainerConfig container = new KOTHWavesRewardContainerConfig;
		container.ClassName = className;
		container.Is_Scripted_Reward = scripted;
		containers.Insert(container);
	}
	
	static void AddDefaultBossLoot(array<ref KOTHWavesLootConfig> loot)
	{
		KOTHWavesLootConfig svd = AddLoot(loot, "SVD", "Mag_SVD_10Rnd", 100, 2, -1);
		svd.Attach.Insert("PSO1Optic");
		svd.Extra_Items.Insert("Mag_SVD_10Rnd");
		svd.Extra_Items.Insert("Mag_SVD_10Rnd");
		
		KOTHWavesLootConfig fal = AddLoot(loot, "FAL", "Mag_FAL_20Rnd", 100, 2, -1);
		fal.Attach.Insert("Fal_OeBttsck");
		fal.Attach.Insert("ACOGOptic");
		fal.Extra_Items.Insert("Mag_FAL_20Rnd");
		fal.Extra_Items.Insert("Mag_FAL_20Rnd");
		
		KOTHWavesLootConfig vest = AddLoot(loot, "PlateCarrierVest", "", 100, 2, -1);
		vest.Alternatives.Insert("PlateCarrierVest_Black");
		vest.Alternatives.Insert("PlateCarrierVest_Camo");
		
		KOTHWavesLootConfig helmet = AddLoot(loot, "BallisticHelmet_Green", "", 100, 2, -1);
		helmet.Alternatives.Insert("BallisticHelmet_Black");
		helmet.Alternatives.Insert("BallisticHelmet_Woodland");
		
		AddLoot(loot, "NVGoggles", "", 100, 1, -1);
		AddLoot(loot, "Morphine", "", 100, 3, -1);
		AddLoot(loot, "BandageDressing", "", 100, 4, -1);
	}
	
	static void AddDefaultLoot(array<ref KOTHWavesLootConfig> loot)
	{
		AddDefaultMilitaryLoot(loot);
		AddDefaultMedicalLoot(loot);
		AddDefaultBaseBuildingLoot(loot);
	}
	
	static void AddDefaultMilitaryLoot(array<ref KOTHWavesLootConfig> loot)
	{
		KOTHWavesLootConfig fal = AddLoot(loot, "FAL", "Mag_FAL_20Rnd", 100, 2, -1);
		fal.Attach.Insert("Fal_OeBttsck");
		fal.Attach.Insert("ACOGOptic");
		fal.Extra_Items.Insert("Mag_FAL_20Rnd");
		fal.Extra_Items.Insert("Mag_FAL_20Rnd");
		
		KOTHWavesLootConfig svd = AddLoot(loot, "SVD", "Mag_SVD_10Rnd", 100, 2, -1);
		svd.Attach.Insert("PSO1Optic");
		svd.Extra_Items.Insert("Mag_SVD_10Rnd");
		svd.Extra_Items.Insert("Mag_SVD_10Rnd");
		
		KOTHWavesLootConfig vest = AddLoot(loot, "PlateCarrierVest", "", 100, 2, -1);
		vest.Alternatives.Insert("PlateCarrierVest_Black");
		vest.Alternatives.Insert("PlateCarrierVest_Camo");
		
		KOTHWavesLootConfig helmet = AddLoot(loot, "BallisticHelmet_Green", "", 100, 2, -1);
		helmet.Alternatives.Insert("BallisticHelmet_Black");
		helmet.Alternatives.Insert("BallisticHelmet_Woodland");
		
		KOTHWavesLootConfig akm = AddLoot(loot, "AKM", "Mag_AKM_30Rnd", 100, 2, -1);
		akm.Extra_Items.Insert("Mag_AKM_30Rnd");
		akm.Extra_Items.Insert("Mag_AKM_30Rnd");
	}
	
	static void AddDefaultMedicalLoot(array<ref KOTHWavesLootConfig> loot)
	{
		AddLoot(loot, "BandageDressing", "", 100, 4, -1);
		AddLoot(loot, "Morphine", "", 100, 3, -1);
		AddLoot(loot, "Epinephrine", "", 100, 2, -1);
		AddLoot(loot, "SalineBag", "", 100, 2, -1);
		AddLoot(loot, "BloodBagIV", "", 100, 2, -1);
		AddLoot(loot, "PainkillerTablets", "", 100, 3, -1);
		AddLoot(loot, "TetracyclineAntibiotics", "", 100, 2, -1);
		AddLoot(loot, "VitaminBottle", "", 100, 2, -1);
	}
	
	static void AddDefaultBaseBuildingLoot(array<ref KOTHWavesLootConfig> loot)
	{
		AddLoot(loot, "NailBox", "", 100, 4, -1);
		AddLoot(loot, "MetalPlate", "", 100, 3, -1);
		AddLoot(loot, "WoodenPlank", "", 100, 4, -1);
		AddLoot(loot, "Hatchet", "", 100, 2, -1);
		AddLoot(loot, "SledgeHammer", "", 100, 2, -1);
		AddLoot(loot, "Shovel", "", 100, 2, -1);
		AddLoot(loot, "Pliers", "", 100, 2, -1);
		AddLoot(loot, "CombinationLock4", "", 100, 1, -1);
	}
	
	static KOTHWavesLootConfig AddLoot(array<ref KOTHWavesLootConfig> loot, string className, string magazineClassName, float spawnChance, int maxSpawnable, int quantity)
	{
		KOTHWavesLootConfig item = new KOTHWavesLootConfig;
		item.ClassName = className;
		item.Magazine_ClassName = magazineClassName;
		item.Spawn_Chance = spawnChance;
		item.Max_Spawnable = maxSpawnable;
		item.Quantity = quantity;
		loot.Insert(item);
		return item;
	}
	
	static void AddVector(array<float> values, float x, float y, float z)
	{
		values.Insert(x);
		values.Insert(y);
		values.Insert(z);
	}
	
	static void CollectRewardContainerClassNames(KOTHWavesConfig config, array<string> outNames)
	{
		if (!config || !outNames)
		{
			return;
		}
		
		AddUniqueClassName(outNames, "KOTHWavesRewardCrate");
		AddUniqueClassName(outNames, config.Settings.Reward_Fallback_ClassName);
		
		if (config.Reward_Containers)
		{
			foreach (KOTHWavesRewardContainerConfig container: config.Reward_Containers)
			{
				if (container)
				{
					AddUniqueClassName(outNames, container.ClassName);
				}
			}
		}
		
		if (config.KOTHs)
		{
			foreach (KOTHWavesKothConfig koth: config.KOTHs)
			{
				if (!koth || !koth.Reward_Container_Classnames)
				{
					continue;
				}
				
				foreach (string cn: koth.Reward_Container_Classnames)
				{
					AddUniqueClassName(outNames, cn);
				}
			}
		}
	}
	
	static void AddUniqueClassName(array<string> outNames, string className)
	{
		if (className == "")
		{
			return;
		}
		
		for (int i = 0; i < outNames.Count(); i++)
		{
			if (outNames.Get(i) == className)
			{
				return;
			}
		}
		
		outNames.Insert(className);
	}
}
