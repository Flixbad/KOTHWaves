class KOTHWavesEvent
{
	protected KOTHWavesManager m_Manager;
	protected KOTHWavesConfig m_Config;
	protected KOTHWavesKothConfig m_Koth;
	protected int m_KothIndex;
	protected bool m_Active;
	protected bool m_Finished;
	protected float m_CaptureProgress;
	protected int m_StartTime;
	protected ref array<Object> m_TrackedObjects;
	protected ref array<Object> m_Zombies;
	protected TerritoryFlag m_Flag;
	protected EntityAI m_Reward;
	protected FireworksLauncher m_Fireworks;
	protected int m_LastProgressNotice;
	protected int m_LastMarkerState;
#ifdef LBmaster_Groups
	protected LBServerMarker m_LBMarker;
#endif
	protected Object m_FlagSmokeProxy;
	protected int m_FlagSmokeMode;
	protected bool m_FlagSmokeSpawnFailLogged;
	protected bool m_LastRewardContainerScripted;
	protected string m_ResolvedWavePresetName;
	protected string m_ResolvedLootPresetName;
	protected ref array<ref KOTHWavesEnemyConfig> m_ActiveEnemyPool;
	protected bool m_WavesScheduled;
	protected ref array<int> m_SpawnedWaveIndices;
	
	void KOTHWavesEvent(KOTHWavesManager manager, KOTHWavesConfig config, KOTHWavesKothConfig koth, int kothIndex)
	{
		m_Manager = manager;
		m_Config = config;
		m_Koth = koth;
		m_KothIndex = kothIndex;
		m_TrackedObjects = new array<Object>;
		m_Zombies = new array<Object>;
		m_SpawnedWaveIndices = new array<int>;
		m_LastMarkerState = -1;
		m_FlagSmokeMode = -1;
	}
	
	void Start()
	{
		m_Active = true;
		m_StartTime = GetGame().GetTime();
		
		SpawnStaticObjects();
		SpawnFlag();
		UpdateFlagProgress();
		UpdateFlagSmokeForCapture(false);
		UpdateMarkerState(0);
		m_WavesScheduled = false;
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, 1000, true);
		m_Manager.Log("KOTH started: " + m_Koth.Name + " @ " + FormatPosition(m_Koth.GetPosition()));
		m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Spawn_Title, m_Koth.Name), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Spawn_Body, m_Koth.Name), m_Config.Settings.Notif_Duration_Spawn);
	}
	
	void ForceCleanup()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartWave);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CleanupAndNotifyManager);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(IgniteFireworks);
		RemoveMarker();
		CleanupRewardsAtSite(true);
		Cleanup();
	}
	
	protected void TryScheduleWavesOnPlayerEntry(int playersInRadius)
	{
		if (m_WavesScheduled || playersInRadius <= 0)
		{
			return;
		}
		
		m_WavesScheduled = true;
		m_ResolvedWavePresetName = m_Manager.ResolveWavePresetName(m_Koth);
		m_ActiveEnemyPool = m_Manager.GetEnemiesForWavePreset(m_ResolvedWavePresetName);
		ScheduleWaves();
		m_Manager.Log("Vagues programmées pour « " + m_Koth.Name + " » — preset fallback « " + m_ResolvedWavePresetName + " », premier joueur dans la zone (rayon " + m_Koth.Capture_Radius.ToString() + " m).");
	}
	
	protected void ScheduleWaves()
	{
		array<ref KOTHWavesWaveConfig> waves = GetWaves();
		
		for (int i = 0; i < waves.Count(); i++)
		{
			KOTHWavesWaveConfig wave = waves.Get(i);
			int delayMs = wave.DelaySeconds * 1000;
			
			if (m_Config.Settings.Debug_QuickMode == 1)
			{
				int quickDelayMs = (i + 1) * 10000;
				
				if (delayMs > quickDelayMs)
				{
					delayMs = quickDelayMs;
				}
			}
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartWave, delayMs, false, i);
		}
	}
	
	protected array<ref KOTHWavesWaveConfig> GetWaves()
	{
		return m_Manager.GetWavesForKoth(m_Koth);
	}
	
	void StartWave(int waveIndex)
	{
		if (!m_Active || m_Finished)
		{
			return;
		}
		
		array<ref KOTHWavesWaveConfig> waves = GetWaves();
		
		if (waveIndex < 0 || waveIndex >= waves.Count())
		{
			return;
		}
		
		KOTHWavesWaveConfig wave = waves.Get(waveIndex);
		int spawnedCount = ExecuteWaveSpawns(wave);
		MarkWaveSpawned(waveIndex);
		
		m_Manager.Log("Wave " + wave.Name + " @ " + m_Koth.Name + " — " + spawnedCount.ToString() + " hostiles spawnés");
		m_Manager.NotifyPlayersInZone(m_Koth.GetPosition(), m_Koth.Capture_Radius, KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Wave_Title, m_Koth.Name, wave.Name, spawnedCount.ToString()), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Wave_Body, m_Koth.Name, wave.Name, spawnedCount.ToString()), m_Config.Settings.Notif_Duration_Wave);
	}
	
	protected int ExecuteWaveSpawns(KOTHWavesWaveConfig wave)
	{
		if (!wave)
		{
			return 0;
		}
		
		int totalSpawned = 0;
		
		if (wave.HasCustomSpawns())
		{
			for (int i = 0; i < wave.Spawns.Count(); i++)
			{
				totalSpawned += ExecuteSpawnGroup(wave, wave.Spawns.Get(i));
			}
			
			return totalSpawned;
		}
		
		string presetName = wave.Enemy_Preset;
		
		if (presetName == "")
		{
			presetName = m_ResolvedWavePresetName;
		}
		
		int spawnCount = wave.ZombieCount;
		
		if (spawnCount <= 0)
		{
			spawnCount = 10;
		}
		
		KOTHWavesWaveSpawnGroupConfig legacyGroup = new KOTHWavesWaveSpawnGroupConfig;
		legacyGroup.Preset = presetName;
		legacyGroup.Count = spawnCount;
		
		return ExecuteSpawnGroup(wave, legacyGroup);
	}
	
	protected int ExecuteSpawnGroup(KOTHWavesWaveConfig wave, KOTHWavesWaveSpawnGroupConfig group)
	{
		if (!wave || !group || group.Preset == "")
		{
			return 0;
		}
		
		array<ref KOTHWavesEnemyConfig> enemyPool = m_Manager.GetEnemiesForWavePreset(group.Preset);
		
		if (!enemyPool || enemyPool.Count() == 0)
		{
			m_Manager.Log("Wave « " + wave.Name + " » @ " + m_Koth.Name + " — preset « " + group.Preset + " » introuvable ou sans ennemis.");
			return 0;
		}
		
		if (group.UsesCountPerType())
		{
			return SpawnCountPerType(wave, group, enemyPool);
		}
		
		int spawnCount = group.Count;
		
		if (spawnCount <= 0)
		{
			spawnCount = wave.ZombieCount;
		}
		
		if (spawnCount <= 0)
		{
			spawnCount = 10;
		}
		
		int spawned = 0;
		
		for (int i = 0; i < spawnCount; i++)
		{
			if (SpawnEnemyFromPool(wave, group, enemyPool))
			{
				spawned++;
			}
		}
		
		return spawned;
	}
	
	protected int SpawnCountPerType(KOTHWavesWaveConfig wave, KOTHWavesWaveSpawnGroupConfig group, array<ref KOTHWavesEnemyConfig> enemyPool)
	{
		int diffMin = group.GetDifficultyMin(wave);
		int diffMax = group.GetDifficultyMax(wave);
		int spawned = 0;
		
		foreach (KOTHWavesEnemyConfig enemy: enemyPool)
		{
			if (!enemy || enemy.ClassName == "")
			{
				continue;
			}
			
			if (enemy.Difficulty < diffMin || enemy.Difficulty > diffMax)
			{
				continue;
			}
			
			if (enemy.SpawnRarity > 0 && Math.RandomFloatInclusive(0.0, 100.0) < enemy.SpawnRarity)
			{
				continue;
			}
			
			for (int i = 0; i < group.CountPerType; i++)
			{
				if (SpawnEnemyEntity(wave, enemy))
				{
					spawned++;
				}
			}
		}
		
		return spawned;
	}
	
	protected bool SpawnEnemyFromPool(KOTHWavesWaveConfig wave, KOTHWavesWaveSpawnGroupConfig group, array<ref KOTHWavesEnemyConfig> enemyPool)
	{
		KOTHWavesEnemyConfig enemy = PickEnemyFromPool(enemyPool, group.GetDifficultyMin(wave), group.GetDifficultyMax(wave));
		
		if (!enemy)
		{
			return false;
		}
		
		return SpawnEnemyEntity(wave, enemy);
	}
	
	protected KOTHWavesEnemyConfig PickEnemyFromPool(array<ref KOTHWavesEnemyConfig> enemyPool, int difficultyMin, int difficultyMax)
	{
		if (!enemyPool || enemyPool.Count() == 0)
		{
			enemyPool = m_ActiveEnemyPool;
		}
		
		if (!enemyPool || enemyPool.Count() == 0)
		{
			if (m_Config && m_Config.Enemies)
			{
				enemyPool = m_Config.Enemies;
			}
		}
		
		if (!enemyPool || enemyPool.Count() == 0)
		{
			return null;
		}
		
		for (int attempt = 0; attempt < 40; attempt++)
		{
			KOTHWavesEnemyConfig enemy = enemyPool.Get(Math.RandomInt(0, enemyPool.Count()));
			
			if (!enemy)
			{
				continue;
			}
			
			if (enemy.Difficulty < difficultyMin || enemy.Difficulty > difficultyMax)
			{
				continue;
			}
			
			if (enemy.SpawnRarity > 0 && Math.RandomFloatInclusive(0.0, 100.0) < enemy.SpawnRarity)
			{
				continue;
			}
			
			return enemy;
		}
		
		return enemyPool.Get(Math.RandomInt(0, enemyPool.Count()));
	}
	
	protected bool SpawnEnemyEntity(KOTHWavesWaveConfig wave, KOTHWavesEnemyConfig enemy)
	{
		if (!enemy || enemy.ClassName == "")
		{
			return false;
		}
		
		vector position = GetRandomSpawnPosition(wave);
		Object entity = CreateHostileEntity(enemy.ClassName, position);
		
		if (entity)
		{
			m_Zombies.Insert(entity);
			m_TrackedObjects.Insert(entity);
			return true;
		}
		
		m_Manager.Log("Spawn échoué pour « " + enemy.ClassName + " » @ " + m_Koth.Name + " — vérifiez le classname (mod client+serveur).");
		return false;
	}
	
	protected Object CreateHostileEntity(string className, vector position)
	{
		Object entity = GetGame().CreateObjectEx(className, position, ECE_PLACE_ON_SURFACE | ECE_INITAI);
		
		if (!entity)
		{
			entity = GetGame().CreateObjectEx(className, position, ECE_PLACE_ON_SURFACE);
		}
		
		if (!entity)
		{
			return null;
		}
		
		return entity;
	}
	
	protected float GetCleanupRadius()
	{
		float staleRadius = m_Config.Settings.Cleanup_Radius;
		
		if (staleRadius < 15.0)
		{
			staleRadius = 15.0;
		}
		
		return staleRadius;
	}
	
	protected void SpawnFlag()
	{
		float staleRadius = GetCleanupRadius();
		int removedCrates = DeleteOrphanRewardCratesNear(GetSurfacePosition(m_Koth.GetPosition()), staleRadius, m_Config);
		
		if (removedCrates > 0)
		{
			m_Manager.Log("KOTH \"" + m_Koth.Name + "\": " + removedCrates.ToString() + " ancienne(s) caisse(s) de récompense retirée(s) avant spawn.");
		}
		
		int removed = KOTHWavesEvent.DeleteOrphanKothFlagsNear(GetSurfacePosition(m_Koth.GetPosition()), staleRadius);
		
		if (removed > 0)
		{
			m_Manager.Log("KOTH \"" + m_Koth.Name + "\": " + removed.ToString() + " ancien(s) drapeau(x) / fumée orphelin(s) retiré(s) avant spawn (persistant ou doublon).");
		}
		
		string flagClassName = m_Config.Settings.Flag_ClassName;
		
		if (flagClassName == "")
		{
			flagClassName = "TerritoryFlag";
		}
		
		m_Flag = TerritoryFlag.Cast(GetGame().CreateObjectEx(flagClassName, GetSurfacePosition(m_Koth.GetPosition()), ECE_PLACE_ON_SURFACE));
		
		string flagSkin = PickRandomFlagSkin();
		
		if (m_Flag)
		{
			m_Flag.SetOrientation(m_Koth.GetOrientation());
			m_TrackedObjects.Insert(m_Flag);
			
			KOTHWavesTerritoryFlag kothFlag = KOTHWavesTerritoryFlag.Cast(m_Flag);
			
			if (kothFlag)
			{
				kothFlag.PrepareForKOTH(flagSkin);
			}
			else
			{
				m_Flag.GetInventory().CreateAttachment(flagSkin);
				m_Flag.AnimateFlag(1.0);
			}
		}
		else
		{
			m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Flag_Error_Title, m_Koth.Name, flagClassName), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Flag_Error_Body, m_Koth.Name, flagClassName), m_Config.Settings.Notif_Duration_Error);
		}
	}
	
	protected void SpawnStaticObjects()
	{
		if (!m_Koth.Static_Objects)
		{
			return;
		}
		
		foreach (KOTHWavesStaticObjectConfig staticObject: m_Koth.Static_Objects)
		{
			if (!staticObject || staticObject.ClassName == "")
			{
				continue;
			}
			
			Object obj = GetGame().CreateObjectEx(staticObject.ClassName, GetSurfacePosition(KOTHWavesConfig.ArrayToVector(staticObject.Position)), ECE_PLACE_ON_SURFACE);
			
			if (obj)
			{
				obj.SetOrientation(KOTHWavesConfig.ArrayToVector(staticObject.Rotation));
				obj.SetScale(staticObject.Scale);
				m_TrackedObjects.Insert(obj);
			}
		}
	}
	
	protected void MarkWaveSpawned(int waveIndex)
	{
		if (!m_SpawnedWaveIndices)
		{
			m_SpawnedWaveIndices = new array<int>;
		}
		
		if (m_SpawnedWaveIndices.Find(waveIndex) == -1)
		{
			m_SpawnedWaveIndices.Insert(waveIndex);
		}
	}
	
	protected bool WasWaveSpawned(int waveIndex)
	{
		return m_SpawnedWaveIndices && m_SpawnedWaveIndices.Find(waveIndex) != -1;
	}
	
	protected void SpawnFinalWaveIfNeeded()
	{
		array<ref KOTHWavesWaveConfig> waves = GetWaves();
		
		if (!waves || waves.Count() == 0)
		{
			return;
		}
		
		int finalIndex = waves.Count() - 1;
		
		if (WasWaveSpawned(finalIndex))
		{
			return;
		}
		
		KOTHWavesWaveConfig wave = waves.Get(finalIndex);
		int spawnedCount = ExecuteWaveSpawns(wave);
		MarkWaveSpawned(finalIndex);
		m_Manager.Log("Vague finale boss @ " + m_Koth.Name + " — " + spawnedCount.ToString() + " hostiles spawnés à la victoire");
		m_Manager.NotifyPlayersInZone(m_Koth.GetPosition(), m_Koth.Capture_Radius, KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Wave_Title, m_Koth.Name, wave.Name, spawnedCount.ToString()), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Wave_Body, m_Koth.Name, wave.Name, spawnedCount.ToString()), m_Config.Settings.Notif_Duration_Wave);
	}
	
	protected vector GetRandomSpawnPosition(KOTHWavesWaveConfig wave)
	{
		vector position;
		bool outdoorOnly = m_Config.Settings.Spawn_Outdoor_Only == 1;
		KOTHWavesSpawnUtil.TryGetSpawnPosition(m_Koth.GetPosition(), wave.SpawnRadiusMin, wave.SpawnRadiusMax, outdoorOnly, position);
		return position;
	}
	
	protected void Tick()
	{
		if (!m_Active || m_Finished)
		{
			return;
		}
		
		int elapsedSeconds = (GetGame().GetTime() - m_StartTime) / 1000;
		bool canCapture = (elapsedSeconds >= m_Config.Settings.Spawn_Start_Delay);
		
		if (elapsedSeconds >= m_Config.Settings.Time_Limit)
		{
			Finish(false);
			return;
		}
		
		int playersInRadius = CountPlayersInRadius();
		TryScheduleWavesOnPlayerEntry(playersInRadius);
		
		if (canCapture && playersInRadius > 0)
		{
			UpdateMarkerState(1);
			float bonus = (playersInRadius - 1) * m_Config.Settings.Capture_Speed_Bonus_PerPlayer_PerSec;
			
			if (bonus > m_Config.Settings.Capture_Speed_Bonus_PerPlayer_PerSec_Max)
			{
				bonus = m_Config.Settings.Capture_Speed_Bonus_PerPlayer_PerSec_Max;
			}
			
			m_CaptureProgress = m_CaptureProgress + 1.0 + bonus;
			BroadcastProgressIfNeeded();
		}
		else if (m_Config.Settings.Flag_Lower_If_Abandoned == 1)
		{
			UpdateMarkerState(0);
			m_CaptureProgress = m_CaptureProgress - 1.0;
			
			if (m_CaptureProgress < 0.0)
			{
				m_CaptureProgress = 0.0;
			}
		}
		
		UpdateFlagProgress();
		UpdateFlagSmokeForCapture(canCapture && playersInRadius > 0);
		
		if (m_CaptureProgress >= m_Config.Settings.Capture_Time)
		{
			Finish(true);
		}
	}
	
	protected int CountPlayersInRadius()
	{
		int count = 0;
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		vector center = m_Koth.GetPosition();
		
		foreach (Man man: players)
		{
			if (!man || !man.IsAlive())
			{
				continue;
			}
			
			if (vector.Distance(man.GetPosition(), center) <= m_Koth.Capture_Radius)
			{
				count++;
			}
		}
		
		return count;
	}
	
	protected string GetCapturerDisplayName()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		vector center = m_Koth.GetPosition();
		PlayerBase best = null;
		float bestDist = -1.0;
		
		foreach (Man man: players)
		{
			if (!man || !man.IsAlive())
			{
				continue;
			}
			
			PlayerBase pb = PlayerBase.Cast(man);
			
			if (!pb || !pb.GetIdentity())
			{
				continue;
			}
			
			float dist = vector.Distance(pb.GetPosition(), center);
			
			if (dist > m_Koth.Capture_Radius)
			{
				continue;
			}
			
			if (!best || dist < bestDist)
			{
				best = pb;
				bestDist = dist;
			}
		}
		
		if (best && best.GetIdentity())
		{
			return best.GetIdentity().GetName();
		}
		
		return "Un survivant";
	}
	
	protected void Finish(bool won)
	{
		if (m_Finished)
		{
			return;
		}
		
		if (won)
		{
			SpawnFinalWaveIfNeeded();
		}
		
		m_Finished = true;
		m_Active = false;
		StopFlagSmoke();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartWave);
		
		if (won)
		{
			m_CaptureProgress = m_Config.Settings.Capture_Time;
			UpdateFlagProgress();
			UpdateMarkerState(2);
			DeleteFlag();
			SpawnReward();
			SpawnFireworks();
			int rewardMinutes = m_Config.Settings.Reward_Despawn_Seconds / 60;
			
			if (rewardMinutes < 1)
			{
				rewardMinutes = 1;
			}
			
			string rewardTimeText = rewardMinutes.ToString() + " minutes";
			
			if (rewardMinutes == 1)
			{
				rewardTimeText = "environ une minute";
			}
			
			string capturerName = GetCapturerDisplayName();
			m_Manager.Log("KOTH captured at " + m_Koth.Name + " by " + capturerName);
			m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Victory_Title, m_Koth.Name, capturerName, rewardTimeText), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Victory_Body, m_Koth.Name, capturerName, rewardTimeText), m_Config.Settings.Notif_Duration_Victory);
		}
		else
		{
			RemoveMarker();
			Cleanup();
			m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Defeat_Title, m_Koth.Name), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Defeat_Body, m_Koth.Name), m_Config.Settings.Notif_Duration_Defeat);
		}
		
		int cleanupDelayMs = m_Config.Settings.Cleanup_Time_After_KOTH_Finished * 1000;
		
		if (!won)
		{
			if (cleanupDelayMs > 5000)
			{
				cleanupDelayMs = 5000;
			}
			
			if (cleanupDelayMs < 500)
			{
				cleanupDelayMs = 500;
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupAndNotifyManager, cleanupDelayMs, false);
	}
	
	protected void SpawnReward()
	{
		string className = ResolveRewardContainer();
		bool scripted = m_LastRewardContainerScripted;
		
		if (className == "")
		{
			className = m_Config.Settings.Reward_Fallback_ClassName;
		}
		
		vector rewardPosition = m_Koth.GetPosition();
		rewardPosition[0] = rewardPosition[0] + 0.8;
		rewardPosition[2] = rewardPosition[2] + 0.8;
		rewardPosition = GetSurfacePosition(rewardPosition);
		m_Reward = EntityAI.Cast(GetGame().CreateObjectEx(className, rewardPosition, ECE_PLACE_ON_SURFACE));
		
		if (!m_Reward && m_Config.Settings.Reward_Fallback_ClassName != "" && className != m_Config.Settings.Reward_Fallback_ClassName)
		{
			m_Manager.Log("Reward container '" + className + "' failed, trying fallback '" + m_Config.Settings.Reward_Fallback_ClassName + "'");
			m_Reward = EntityAI.Cast(GetGame().CreateObjectEx(m_Config.Settings.Reward_Fallback_ClassName, rewardPosition, ECE_PLACE_ON_SURFACE));
		}
		
		if (!m_Reward)
		{
			m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Reward_Error_Title, m_Koth.Name), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Reward_Error_Body, m_Koth.Name), m_Config.Settings.Notif_Duration_Error);
			return;
		}
		
		m_Reward.SetOrientation(m_Koth.GetOrientation());
		
		m_ResolvedLootPresetName = m_Manager.ResolveVictoryLootPresetName(m_Koth);
		
		if (!scripted)
		{
			FillReward(m_Reward);
		}
		else
		{
			m_Manager.Log("Scripted reward container (no auto-loot): " + m_Koth.Name + " class " + className);
			OnScriptedRewardReady(m_Reward);
		}
		
		m_Manager.ScheduleRewardDespawn(this, m_Reward, m_Config.Settings.Reward_Despawn_Seconds);
		if (m_ResolvedLootPresetName == "")
		{
			m_ResolvedLootPresetName = "(default)";
		}
		
		m_Manager.Log("Reward container spawned for " + m_Koth.Name + " — loot preset '" + m_ResolvedLootPresetName + "', " + m_Koth.Reward_Loot_Count.ToString() + " item rolls");
	}
	
	protected string ResolveRewardContainer()
	{
		m_LastRewardContainerScripted = false;
		
		if (m_Koth.Reward_Container_Classnames && m_Koth.Reward_Container_Classnames.Count() > 0)
		{
			string cn = NormalizeRewardContainerClassName(m_Koth.Reward_Container_Classnames.Get(Math.RandomInt(0, m_Koth.Reward_Container_Classnames.Count())));
			m_LastRewardContainerScripted = IsScriptedRewardContainer(cn);
			return cn;
		}
		
		if (m_Config.Reward_Containers && m_Config.Reward_Containers.Count() > 0)
		{
			KOTHWavesRewardContainerConfig container = m_Config.Reward_Containers.Get(Math.RandomInt(0, m_Config.Reward_Containers.Count()));
			
			if (container)
			{
				string picked = NormalizeRewardContainerClassName(container.ClassName);
				m_LastRewardContainerScripted = (container.Is_Scripted_Reward == 1);
				return picked;
			}
		}
		
		string fb = m_Config.Settings.Reward_Fallback_ClassName;
		m_LastRewardContainerScripted = IsScriptedRewardContainer(fb);
		return fb;
	}
	
	protected bool IsScriptedRewardContainer(string className)
	{
		if (!m_Config || !m_Config.Reward_Containers || className == "")
		{
			return false;
		}
		
		for (int i = 0; i < m_Config.Reward_Containers.Count(); i++)
		{
			KOTHWavesRewardContainerConfig c = m_Config.Reward_Containers.Get(i);
			
			if (!c || c.ClassName != className)
			{
				continue;
			}
			
			return c.Is_Scripted_Reward == 1;
		}
		
		return false;
	}
	
	protected void OnScriptedRewardReady(EntityAI reward)
	{
	}
	
	protected string PickRandomFlagSkin()
	{
		if (m_Config.Settings.Flags && m_Config.Settings.Flags.Count() > 0)
		{
			return m_Config.Settings.Flags.Get(Math.RandomInt(0, m_Config.Settings.Flags.Count()));
		}
		
		return "Flag_DayZ";
	}
	
	protected string NormalizeRewardContainerClassName(string className)
	{
		if (className == "" || className == "WoodenCrate")
		{
			return m_Config.Settings.Reward_Fallback_ClassName;
		}
		
		return className;
	}
	
	protected void SpawnFireworks()
	{
		if (m_Config.Settings.Fireworks_Enabled != 1 || m_Config.Settings.Fireworks_ClassName == "")
		{
			return;
		}
		
		vector fireworksPosition = m_Koth.GetPosition();
		fireworksPosition[0] = fireworksPosition[0] - 1.2;
		fireworksPosition[2] = fireworksPosition[2] + 1.2;
		fireworksPosition = GetSurfacePosition(fireworksPosition);
		
		m_Fireworks = FireworksLauncher.Cast(GetGame().CreateObjectEx(m_Config.Settings.Fireworks_ClassName, fireworksPosition, ECE_PLACE_ON_SURFACE));
		
		if (!m_Fireworks)
		{
			m_Manager.Log("Fireworks failed to spawn. Check Fireworks_ClassName: " + m_Config.Settings.Fireworks_ClassName);
			return;
		}
		
		m_Fireworks.SetOrientation(m_Koth.GetOrientation());
		m_TrackedObjects.Insert(m_Fireworks);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(IgniteFireworks, 1000, false);
	}
	
	protected void IgniteFireworks()
	{
		if (!m_Fireworks)
		{
			return;
		}
		
		m_Fireworks.OnIgnitedThis(null);
	}
	
	protected void DeleteFlag()
	{
		if (!m_Flag)
		{
			return;
		}
		
		m_TrackedObjects.RemoveItem(m_Flag);
		GetGame().ObjectDelete(m_Flag);
		m_Flag = null;
	}
	
	void NotifyRewardAutoDespawned()
	{
		m_Reward = null;
	}
	
	protected void DeleteReward()
	{
		m_Manager.CancelRewardDespawn();
		
		if (!m_Reward)
		{
			return;
		}
		
		KOTHWavesDeletion.DeleteRewardContainerHard(m_Reward);
		m_Reward = null;
	}
	
	protected void CleanupRewardsAtSite(bool forceDelete)
	{
		if (!forceDelete && m_Config.Settings.Reward_Delete_On_Cleanup != 1)
		{
			return;
		}
		
		DeleteReward();
		
		array<string> rewardClassNames = new array<string>;
		KOTHWavesConfig.CollectRewardContainerClassNames(m_Config, rewardClassNames);
		int removed = KOTHWavesDeletion.DeleteRewardCratesNear(m_Koth.GetPosition(), GetCleanupRadius(), rewardClassNames);
		
		if (removed > 0)
		{
			m_Manager.Log("KOTH \"" + m_Koth.Name + "\": " + removed.ToString() + " caisse(s) de récompense retirée(s) à la réinitialisation.");
		}
	}
	
	static int DeleteOrphanRewardCratesNear(vector center, float radius, KOTHWavesConfig config)
	{
		if (!config)
		{
			return 0;
		}
		
		array<string> rewardClassNames = new array<string>;
		KOTHWavesConfig.CollectRewardContainerClassNames(config, rewardClassNames);
		return KOTHWavesDeletion.DeleteRewardCratesNear(center, radius, rewardClassNames);
	}
	
	protected vector GetFlagSmokeWorldPosition()
	{
		vector p;
		
		if (m_Flag)
		{
			p = m_Flag.GetPosition();
		}
		else
		{
			p = m_Koth.GetPosition();
		}
		
		p = GetSurfacePosition(p);
		p[1] = p[1] + 1.35;
		return p;
	}
	
	protected void StopFlagSmoke()
	{
		if (m_FlagSmokeProxy)
		{
			GetGame().ObjectDelete(m_FlagSmokeProxy);
			m_FlagSmokeProxy = null;
		}
		
		m_FlagSmokeMode = -1;
		m_FlagSmokeSpawnFailLogged = false;
	}
	
	protected void UpdateFlagSmokeForCapture(bool capturing)
	{
		if (m_Config.Settings.Flag_Smoke_Enabled != 1)
		{
			StopFlagSmoke();
			return;
		}
		
		if (!m_Active || m_Finished)
		{
			return;
		}
		
		int wantMode = 0;
		
		if (capturing)
		{
			wantMode = 1;
		}
		
		if (wantMode != m_FlagSmokeMode)
		{
			m_FlagSmokeSpawnFailLogged = false;
		}
		
		if (m_FlagSmokeProxy)
		{
			KOTHWavesSmokeProxy sp = KOTHWavesSmokeProxy.Cast(m_FlagSmokeProxy);
			
			if (sp)
			{
				sp.SetDisplayModeServer(wantMode);
				m_FlagSmokeMode = wantMode;
				m_FlagSmokeSpawnFailLogged = false;
				return;
			}
		}
		
		if (wantMode == m_FlagSmokeMode && !m_FlagSmokeProxy && m_FlagSmokeSpawnFailLogged)
		{
			return;
		}
		
		StopFlagSmoke();
		m_FlagSmokeMode = wantMode;
		
		vector smokePos = GetFlagSmokeWorldPosition();
		Object proxy = GetGame().CreateObjectEx("KOTHWavesSmokeProxy", smokePos, ECE_PLACE_ON_SURFACE);
		
		if (!proxy)
		{
			proxy = GetGame().CreateObjectEx("KOTHWavesSmokeProxy", smokePos, ECE_NONE);
		}
		
		if (!proxy)
		{
			proxy = GetGame().CreateObjectEx("KOTHWavesSmokeProxy", smokePos, ECE_PLACE_ON_SURFACE | ECE_CREATEPHYSICS);
		}
		
		if (proxy)
		{
			m_FlagSmokeProxy = proxy;
			m_FlagSmokeSpawnFailLogged = false;
			
			KOTHWavesSmokeProxy sp2 = KOTHWavesSmokeProxy.Cast(proxy);
			
			if (sp2)
			{
				sp2.SetDisplayModeServer(wantMode);
			}
		}
		else
		{
			m_FlagSmokeSpawnFailLogged = true;
			m_Manager.Log("KOTH smoke proxy failed to spawn: KOTHWavesSmokeProxy (verifiez CfgPatches units[] et -mod client+serveur).");
		}
	}
	
	protected void UpdateFlagProgress()
	{
		if (!m_Flag || m_Config.Settings.Capture_Time <= 0)
		{
			return;
		}
		
		float progress01 = m_CaptureProgress / m_Config.Settings.Capture_Time;
		
		if (progress01 < 0.0)
		{
			progress01 = 0.0;
		}
		
		if (progress01 > 1.0)
		{
			progress01 = 1.0;
		}
		
		m_Flag.AnimateFlag(1.0 - progress01);
	}
	
	protected void UpdateMarkerState(int state)
	{
		if (m_Config.Settings.Map_Marker_Enabled != 1)
		{
			return;
		}
		
		if (state == m_LastMarkerState)
		{
			return;
		}
		
		m_LastMarkerState = state;
		
#ifdef LBmaster_Groups
		string markerName = "KOTH - " + m_Koth.Name;
		vector markerPosition = m_Koth.GetPosition();
		string markerIcon = m_Config.Settings.Marker_Icon;
		
		if (markerIcon == "")
		{
			markerIcon = "King";
		}
		
		if (!m_LBMarker)
		{
			m_LBMarker = LBStaticMarkerManager.Get.AddTempServerMarker(markerName, markerPosition, markerIcon, GetMarkerColor(state), true, m_Config.Settings.Marker_Display_3D == 1, m_Config.Settings.Marker_Display_Map == 1, m_Config.Settings.Marker_Display_GPS == 1);
			
			if (m_LBMarker)
			{
				SetLBMarkerRadius(state);
			}
			
			return;
		}
		
		m_LBMarker.SetName(markerName);
		m_LBMarker.SetPosition(markerPosition);
		m_LBMarker.SetIcon(markerIcon);
		SetLBMarkerColor(state);
		SetLBMarkerRadius(state);
#endif
	}
	
	protected int GetMarkerColor(int state)
	{
		if (state == 1)
		{
			return ARGB(255, 255, 255, 255);
		}
		
		if (state == 2)
		{
			return ARGB(255, 0, 0, 0);
		}
		
		return ARGB(255, 255, 230, 0);
	}
	
#ifdef LBmaster_Groups
	protected void SetLBMarkerColor(int state)
	{
		if (!m_LBMarker)
		{
			return;
		}
		
		if (state == 1)
		{
			m_LBMarker.SetColorARGB(255, 255, 255, 255);
			return;
		}
		
		if (state == 2)
		{
			m_LBMarker.SetColorARGB(255, 0, 0, 0);
			return;
		}
		
		m_LBMarker.SetColorARGB(255, 255, 230, 0);
	}
	
	protected void SetLBMarkerRadius(int state)
	{
		if (!m_LBMarker)
		{
			return;
		}
		
		float radius = m_Config.Settings.LB_Marker_Radius;
		
		if (radius < 0)
		{
			radius = m_Koth.Capture_Radius;
		}
		
		if (radius == 0)
		{
			return;
		}
		
		if (state == 1)
		{
			m_LBMarker.SetRadius(radius, 255, 255, 255, 255, false);
			return;
		}
		
		if (state == 2)
		{
			m_LBMarker.SetRadius(radius, 255, 0, 0, 0, false);
			return;
		}
		
		m_LBMarker.SetRadius(radius, 255, 255, 230, 0, false);
	}
#endif
	
	protected void RemoveMarker()
	{
#ifdef LBmaster_Groups
		if (m_LBMarker)
		{
			LBStaticMarkerManager.Get.RemoveServerMarker(m_LBMarker);
			m_LBMarker = null;
		}
#endif
	}
	
	protected void FillReward(EntityAI container)
	{
		array<ref KOTHWavesLootConfig> lootPool = m_Manager.ResolveVictoryLootPool(m_Koth);
		
		if (!container || !lootPool || lootPool.Count() == 0)
		{
			if (m_ResolvedLootPresetName == "")
			{
				m_ResolvedLootPresetName = m_Manager.ResolveVictoryLootPresetName(m_Koth);
			}
			
			if (m_ResolvedLootPresetName == "")
			{
				m_ResolvedLootPresetName = "(default)";
			}
			
			m_Manager.Log("Reward loot failed for " + m_Koth.Name + " — preset '" + m_ResolvedLootPresetName + "' is empty or missing in ItemPreset.json");
			return;
		}
		
		map<string, int> spawnedCounts = new map<string, int>;
		int spawned = 0;
		int attempts = 0;
		
		while (spawned < m_Koth.Reward_Loot_Count && attempts < 250)
		{
			attempts++;
			
			KOTHWavesLootConfig loot = lootPool.Get(Math.RandomInt(0, lootPool.Count()));
			
			if (!loot || loot.ClassName == "")
			{
				continue;
			}
			
			if (Math.RandomFloatInclusive(0.0, 100.0) > loot.Spawn_Chance)
			{
				continue;
			}
			
			int currentCount = 0;
			
			if (spawnedCounts.Contains(loot.ClassName))
			{
				currentCount = spawnedCounts.Get(loot.ClassName);
			}
			
			if (loot.Max_Spawnable > 0 && currentCount >= loot.Max_Spawnable)
			{
				continue;
			}
			
			EntityAI item = SpawnLootItem(container, loot);
			
			if (item)
			{
				spawnedCounts.Set(loot.ClassName, currentCount + 1);
				spawned++;
			}
		}
	}
	
	protected EntityAI SpawnLootItem(EntityAI container, KOTHWavesLootConfig loot)
	{
		string className = PickLootClassName(loot);
		EntityAI item = EntityAI.Cast(container.GetInventory().CreateInInventory(className));
		
		if (!item)
		{
			item = EntityAI.Cast(GetGame().CreateObjectEx(className, m_Koth.GetPosition(), ECE_PLACE_ON_SURFACE));
		}
		
		if (!item)
		{
			return null;
		}
		
		ItemBase itemBase = ItemBase.Cast(item);
		
		if (itemBase && loot.Quantity >= 0)
		{
			itemBase.SetQuantity(loot.Quantity);
		}
		
		foreach (string attachmentClassName: loot.Attach)
		{
			if (attachmentClassName != "")
			{
				item.GetInventory().CreateAttachment(attachmentClassName);
			}
		}
		
		if (loot.Magazine_ClassName != "")
		{
			container.GetInventory().CreateInInventory(loot.Magazine_ClassName);
		}
		
		foreach (string extraClassName: loot.Extra_Items)
		{
			if (extraClassName != "")
			{
				container.GetInventory().CreateInInventory(extraClassName);
			}
		}
		
		return item;
	}
	
	protected string PickLootClassName(KOTHWavesLootConfig loot)
	{
		if (!loot.Alternatives || loot.Alternatives.Count() == 0)
		{
			return loot.ClassName;
		}
		
		int pick = Math.RandomInt(0, loot.Alternatives.Count() + 1);
		
		if (pick == 0)
		{
			return loot.ClassName;
		}
		
		return loot.Alternatives.Get(pick - 1);
	}
	
	protected void BroadcastProgressIfNeeded()
	{
		if (m_Config.Settings.Capture_Time <= 0)
		{
			return;
		}
		
		float progressPercent = (m_CaptureProgress / m_Config.Settings.Capture_Time) * 100.0;
		int roundedProgress = 0;
		
		if (progressPercent >= 75.0)
		{
			roundedProgress = 75;
		}
		else if (progressPercent >= 50.0)
		{
			roundedProgress = 50;
		}
		else if (progressPercent >= 25.0)
		{
			roundedProgress = 25;
		}
		
		if (roundedProgress > 0 && roundedProgress < 100 && roundedProgress != m_LastProgressNotice)
		{
			m_LastProgressNotice = roundedProgress;
			m_Manager.NotifyPlayersInZone(m_Koth.GetPosition(), m_Koth.Capture_Radius, KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Progress_Title, m_Koth.Name, roundedProgress.ToString()), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Progress_Body, m_Koth.Name, roundedProgress.ToString()), m_Config.Settings.Notif_Duration_Progress);
		}
	}
	
	protected vector GetSurfacePosition(vector position)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		return position;
	}
	
	protected string FormatPosition(vector position)
	{
		float x = position[0];
		float z = position[2];
		return "X:" + x.ToString() + " Z:" + z.ToString();
	}
	
	static int DeleteOrphanKothFlagsNear(vector center, float radius)
	{
		array<Object> near = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(center, radius, near, proxyCargos);
		
		array<Object> toDelete = new array<Object>;
		
		for (int i = 0; i < near.Count(); i++)
		{
			Object o = near.Get(i);
			
			if (!o)
			{
				continue;
			}
			
			if (KOTHWavesTerritoryFlag.Cast(o))
			{
				toDelete.Insert(o);
				continue;
			}
			
			if (KOTHWavesSmokeProxy.Cast(o))
			{
				toDelete.Insert(o);
			}
		}
		
		int removed = 0;
		
		for (int j = 0; j < toDelete.Count(); j++)
		{
			Object x = toDelete.Get(j);
			
			if (x)
			{
				GetGame().ObjectDelete(x);
				removed++;
			}
		}
		
		return removed;
	}
	
	protected void CleanupAndNotifyManager()
	{
		RemoveMarker();
		CleanupRewardsAtSite(false);
		Cleanup();
		m_Manager.NotifyAllPlayers(KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Cleanup_Title, m_Koth.Name), KOTHWavesNotifFormat.Msg(m_Config.Settings.Notif_Cleanup_Body, m_Koth.Name), m_Config.Settings.Notif_Duration_Cleanup);
		m_Manager.OnEventCleanupFinished(this);
	}
	
	protected void Cleanup()
	{
		StopFlagSmoke();
		
		for (int i = m_TrackedObjects.Count() - 1; i >= 0; i--)
		{
			Object obj = m_TrackedObjects.Get(i);
			
			if (obj)
			{
				GetGame().ObjectDelete(obj);
			}
		}
		
		m_TrackedObjects.Clear();
		m_Zombies.Clear();
		m_Flag = null;
		m_Fireworks = null;
	}
}
