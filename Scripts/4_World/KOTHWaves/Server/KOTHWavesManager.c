class KOTHWavesManager
{
	protected static ref KOTHWavesManager m_Instance;
	
	protected ref KOTHWavesConfig m_Config;
	protected ref KOTHWavesItemPresets m_ItemPresets;
	protected ref KOTHWavesWavePresets m_WavePresets;
	protected ref KOTHWavesEvent m_ActiveEvent;
	protected bool m_Started;
	protected int m_LastKothIndex;
	protected EntityAI m_ScheduledRewardDespawn;
	protected ref KOTHWavesEvent m_ScheduledRewardOwnerEvent;
	
	void KOTHWavesManager()
	{
		m_LastKothIndex = -1;
	}
	
	static KOTHWavesManager GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new KOTHWavesManager;
		}
		
		return m_Instance;
	}
	
	void Start()
	{
		if (m_Started || !GetGame().IsServer())
		{
			return;
		}
		
		m_Started = true;
		m_Config = KOTHWavesConfig.Load();
		m_ItemPresets = KOTHWavesItemPresets.Load();
		m_WavePresets = KOTHWavesWavePresets.Load();
		Log("Config loaded from " + KOTHWAVES_CONFIG_FILE);
		Log("Item presets loaded from " + KOTHWAVES_ITEM_PRESET_FILE);
		Log("Wave presets loaded from " + KOTHWAVES_WAVE_PRESET_FILE);
		CleanupOrphanKothObjectsAtConfiguredSites();
		ScheduleNext(m_Config.Settings.Start_Delay);
	}
	
	protected void CleanupOrphanKothObjectsAtConfiguredSites()
	{
		if (!m_Config || !m_Config.KOTHs || m_Config.KOTHs.Count() == 0)
		{
			return;
		}
		
		float r = m_Config.Settings.Cleanup_Radius;
		
		if (r < 15.0)
		{
			r = 15.0;
		}
		
		for (int i = 0; i < m_Config.KOTHs.Count(); i++)
		{
			KOTHWavesKothConfig k = m_Config.KOTHs.Get(i);
			
			if (!k)
			{
				continue;
			}
			
			int n = KOTHWavesEvent.DeleteOrphanKothFlagsNear(k.GetPosition(), r);
			int crates = KOTHWavesEvent.DeleteOrphanRewardCratesNear(k.GetPosition(), r, m_Config);
			
			if (n > 0 || crates > 0)
			{
				Log("KOTH pre-start: " + n.ToString() + " drapeau(x)/fumée, " + crates.ToString() + " caisse(s) retirée(s) près de \"" + k.Name + "\".");
			}
		}
	}
	
	void Stop()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartNextKoth);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExecuteScheduledRewardDespawn);
		
		EntityAI pendingReward = m_ScheduledRewardDespawn;
		KOTHWavesEvent pendingOwner = m_ScheduledRewardOwnerEvent;
		m_ScheduledRewardDespawn = null;
		m_ScheduledRewardOwnerEvent = null;
		
		if (pendingOwner)
		{
			pendingOwner.NotifyRewardAutoDespawned();
		}
		
		if (pendingReward)
		{
			KOTHWavesDeletion.DeleteRewardContainerHard(pendingReward);
		}
		
		if (m_ActiveEvent)
		{
			m_ActiveEvent.ForceCleanup();
			m_ActiveEvent = null;
		}
		
		m_Started = false;
	}
	
	void ScheduleNext(int delaySeconds)
	{
		if (!m_Config)
		{
			return;
		}
		
		int delayMs = delaySeconds * 1000;
		
		if (m_Config.Settings.Debug_QuickMode == 1)
		{
			if (delayMs > 10000)
			{
				delayMs = 10000;
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartNextKoth);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartNextKoth, delayMs, false);
		Log("Next KOTH scheduled in " + (delayMs / 1000).ToString() + " seconds");
	}
	
	void StartNextKoth()
	{
		if (!m_Config)
		{
			m_Config = KOTHWavesConfig.Load();
		}
		
		if (m_ActiveEvent)
		{
			Log("Start skipped because a KOTH is already active");
			return;
		}
		
		if (m_Config.KOTHs.Count() == 0)
		{
			Log("Start failed because no KOTH zones are configured");
			ScheduleNext(m_Config.Settings.Starting_Failed_Retry_Delay);
			return;
		}
		
		if (GetPlayerCount() < m_Config.Settings.Minimum_Players)
		{
			Log("Start failed because the server has fewer than Minimum_Players");
			ScheduleNext(m_Config.Settings.Starting_Failed_Retry_Delay);
			return;
		}
		
		int startIndex = m_LastKothIndex + 1;
		
		if (startIndex >= m_Config.KOTHs.Count())
		{
			startIndex = 0;
		}
		
		KOTHWavesKothConfig selectedKoth = null;
		int selectedIndex = -1;
		
		for (int i = 0; i < m_Config.KOTHs.Count(); i++)
		{
			int index = (startIndex + i) % m_Config.KOTHs.Count();
			KOTHWavesKothConfig candidate = m_Config.KOTHs.Get(index);
			
			if (!IsAnyPlayerNear(candidate.GetPosition(), m_Config.Settings.Players_In_Radius_Check_Range))
			{
				selectedKoth = candidate;
				selectedIndex = index;
				break;
			}
		}
		
		if (!selectedKoth)
		{
			Log("Start failed because all configured KOTH zones have players nearby");
			ScheduleNext(m_Config.Settings.Starting_Failed_Retry_Delay);
			return;
		}
		
		m_ActiveEvent = new KOTHWavesEvent(this, m_Config, selectedKoth, selectedIndex);
		m_LastKothIndex = selectedIndex;
		Log("Starting KOTH zone #" + selectedIndex.ToString() + ": " + selectedKoth.Name);
		m_ActiveEvent.Start();
	}
	
	void OnEventCleanupFinished(KOTHWavesEvent eventRef)
	{
		if (m_ActiveEvent == eventRef)
		{
			m_ActiveEvent = null;
		}
		
		ScheduleNext(m_Config.Settings.Time_Between);
	}
	
	bool IsAnyPlayerNear(vector position, float radius)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		
		foreach (Man man: players)
		{
			if (!man)
			{
				continue;
			}
			
			if (vector.Distance(man.GetPosition(), position) <= radius)
			{
				return true;
			}
		}
		
		return false;
	}
	
	int GetPlayerCount()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		return players.Count();
	}
	
	void ScheduleRewardDespawn(KOTHWavesEvent ownerEvent, EntityAI reward, int delaySeconds)
	{
		CancelRewardDespawn();
		
		if (!reward || delaySeconds <= 0)
		{
			return;
		}
		
		m_ScheduledRewardDespawn = reward;
		m_ScheduledRewardOwnerEvent = ownerEvent;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExecuteScheduledRewardDespawn, delaySeconds * 1000, false);
		Log("Reward despawn scheduled in " + delaySeconds.ToString() + " seconds");
	}
	
	void CancelRewardDespawn()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExecuteScheduledRewardDespawn);
		m_ScheduledRewardDespawn = null;
		m_ScheduledRewardOwnerEvent = null;
	}
	
	protected void ExecuteScheduledRewardDespawn()
	{
		EntityAI reward = m_ScheduledRewardDespawn;
		KOTHWavesEvent owner = m_ScheduledRewardOwnerEvent;
		m_ScheduledRewardDespawn = null;
		m_ScheduledRewardOwnerEvent = null;
		
		if (owner)
		{
			owner.NotifyRewardAutoDespawned();
		}
		
		if (reward)
		{
			KOTHWavesDeletion.DeleteRewardContainerHard(reward);
		}
		
		Log("Scheduled reward crate despawn executed");
	}
	
	protected void SendNotificationToIdentity(PlayerIdentity identity, string title, string body, float durationSeconds)
	{
		if (!identity)
		{
			return;
		}
		
#ifdef EXPANSIONMOD
		ExpansionNotification(title, body).Info(identity);
#else
		NotificationSystem.SendNotificationToPlayerIdentityExtended(identity, durationSeconds, title, body, "set:dayz_gui image:icon_mission");
#endif
	}
	
	void NotifyAllPlayers(string title, string body, float durationSeconds)
	{
		Log(title + " — " + body);
		
		if (!m_Config || m_Config.Settings.Notification_Enabled != 1)
		{
			return;
		}
		
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		
		foreach (Man man: players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			
			if (player && player.GetIdentity())
			{
				SendNotificationToIdentity(player.GetIdentity(), title, body, durationSeconds);
			}
		}
	}
	
	void NotifyPlayersInZone(vector center, float radius, string title, string body, float durationSeconds)
	{
		Log(title + " (zone) — " + body);
		
		if (!m_Config || m_Config.Settings.Notification_Enabled != 1)
		{
			return;
		}
		
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		
		foreach (Man man: players)
		{
			if (!man || !man.IsAlive())
			{
				continue;
			}
			
			if (vector.Distance(man.GetPosition(), center) > radius)
			{
				continue;
			}
			
			PlayerBase player = PlayerBase.Cast(man);
			
			if (player && player.GetIdentity())
			{
				SendNotificationToIdentity(player.GetIdentity(), title, body, durationSeconds);
			}
		}
	}
	
	void Log(string message)
	{
		if (!m_Config || !m_Config.Settings || m_Config.Settings.Logging_Enabled == 1)
		{
			Print("[KOTHWaves] " + message);
		}
	}
	
	KOTHWavesItemPresets GetItemPresets()
	{
		return m_ItemPresets;
	}
	
	KOTHWavesWavePresets GetWavePresets()
	{
		return m_WavePresets;
	}
	
	string ResolveLootPresetName(KOTHWavesKothConfig koth)
	{
		if (!m_ItemPresets)
		{
			return "";
		}
		
		return m_ItemPresets.ResolveLootPresetName(koth);
	}
	
	array<ref KOTHWavesLootConfig> ResolveLootPool(KOTHWavesKothConfig koth)
	{
		return ResolveLootPoolByPresetName(ResolveLootPresetName(koth), koth);
	}
	
	array<ref KOTHWavesLootConfig> ResolveVictoryLootPool(KOTHWavesKothConfig koth)
	{
		string presetName = "boss";
		
		if (koth && koth.Victory_Loot_Preset != "")
		{
			presetName = koth.Victory_Loot_Preset;
		}
		
		return ResolveLootPoolByPresetName(presetName, koth);
	}
	
	array<ref KOTHWavesLootConfig> ResolveLootPoolByPresetName(string presetName, KOTHWavesKothConfig koth)
	{
		array<ref KOTHWavesLootConfig> fallback;
		
		if (m_Config && m_Config.Loot)
		{
			fallback = m_Config.Loot;
		}
		
		if (!m_ItemPresets || presetName == "")
		{
			return fallback;
		}
		
		array<ref KOTHWavesLootConfig> resolved = m_ItemPresets.GetLootByPresetName(presetName);
		
		if (resolved && resolved.Count() > 0)
		{
			return resolved;
		}
		
		return fallback;
	}
	
	string ResolveVictoryLootPresetName(KOTHWavesKothConfig koth)
	{
		if (koth && koth.Victory_Loot_Preset != "")
		{
			return koth.Victory_Loot_Preset;
		}
		
		return "boss";
	}
	
	string ResolveWavePresetName(KOTHWavesKothConfig koth)
	{
		if (!m_WavePresets)
		{
			return "";
		}
		
		return m_WavePresets.ResolvePresetName(koth);
	}
	
	array<ref KOTHWavesWaveConfig> GetWavesForKoth(KOTHWavesKothConfig koth)
	{
		array<ref KOTHWavesWaveConfig> fallback;
		
		if (m_Config && m_Config.Default_Waves)
		{
			fallback = m_Config.Default_Waves;
		}
		
		if (!m_WavePresets)
		{
			return fallback;
		}
		
		return m_WavePresets.GetWavesForKoth(koth, fallback);
	}
	
	array<ref KOTHWavesEnemyConfig> GetEnemiesForWavePreset(string presetName)
	{
		array<ref KOTHWavesEnemyConfig> fallback;
		
		if (m_Config && m_Config.Enemies)
		{
			fallback = m_Config.Enemies;
		}
		
		if (!m_WavePresets)
		{
			return fallback;
		}
		
		return m_WavePresets.GetEnemiesForPresetName(presetName, fallback);
	}
}
