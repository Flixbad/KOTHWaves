class KOTHWavesSmokeProxy: WoodenStick
{
	protected int m_SmokeDisplayMode;
	protected ParticleSource m_SmokeParticle;
	protected bool m_ClientParticleInited;
	protected int m_ClientParticleLastMode;
	
	void KOTHWavesSmokeProxy()
	{
		RegisterNetSyncVariableInt("m_SmokeDisplayMode", 0, 1);
	}
	
	void SetDisplayModeServer(int mode)
	{
		if (!GetGame().IsServer())
		{
			return;
		}
		
		if (mode != 0 && mode != 1)
		{
			mode = 0;
		}
		
		if (m_SmokeDisplayMode == mode)
		{
			return;
		}
		
		m_SmokeDisplayMode = mode;
		SetSynchDirty();
	}
	
	protected int GetStartParticleId()
	{
		if (m_SmokeDisplayMode == 1)
		{
			return ParticleList.GRENADE_RDG2_WHITE_START;
		}
		
		return ParticleList.GRENADE_M18_YELLOW_START;
	}
	
	protected int GetLoopParticleId()
	{
		if (m_SmokeDisplayMode == 1)
		{
			return ParticleList.GRENADE_RDG2_WHITE_LOOP;
		}
		
		return ParticleList.GRENADE_M18_YELLOW_LOOP;
	}
	
	protected void ApplyKothSmokeIntensity(Object particleObj)
	{
		Particle p = Particle.Cast(particleObj);
		
		if (!p)
		{
			return;
		}
		
		p.ScaleParticleParamFromOriginal(EmitorParam.BIRTH_RATE, 0.34);
		p.ScaleParticleParamFromOriginal(EmitorParam.BIRTH_RATE_RND, 0.34);
		p.ScaleParticleParamFromOriginal(EmitorParam.SIZE, 0.48);
	}
	
	protected void ClientRestartSmokeVisual()
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnSmokeRefreshTick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AttachSmokeLoopToSelf);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ApplyIntensityToLoopParticle);
		StopKothSmokeLoop();
		
		if (this)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredBeginSmokeGrenadeStyle, 80, false);
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		if (m_ClientParticleInited && m_ClientParticleLastMode == m_SmokeDisplayMode)
		{
			return;
		}
		
		m_ClientParticleInited = true;
		m_ClientParticleLastMode = m_SmokeDisplayMode;
		ClientRestartSmokeVisual();
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredBeginSmokeGrenadeStyle, 400, false);
	}
	
	protected void DeferredBeginSmokeGrenadeStyle()
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		if (!this)
		{
			return;
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnSmokeRefreshTick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AttachSmokeLoopToSelf);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ApplyIntensityToLoopParticle);
		PlaySmokeGrenadeStyle();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnSmokeRefreshTick, 32000, true);
	}
	
	protected void PlaySmokeGrenadeStyle()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AttachSmokeLoopToSelf);
		StopKothSmokeLoop();
		
		int startId = GetStartParticleId();
		int loopId = GetLoopParticleId();
		
		if (loopId == 0)
		{
			return;
		}
		
		ParticleManager pm = ParticleManager.GetInstance();
		
		if (!pm)
		{
			return;
		}
		
		vector worldPos = GetPosition();
		worldPos[1] = worldPos[1] + 0.12;
		
		if (startId != 0)
		{
			Particle startP = pm.PlayInWorld(startId, worldPos);
			
			if (startP)
			{
				ApplyKothSmokeIntensity(startP);
			}
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AttachSmokeLoopToSelf, 220, false);
	}
	
	protected void AttachSmokeLoopToSelf()
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		if (!this)
		{
			return;
		}
		
		int loopId = GetLoopParticleId();
		
		if (loopId == 0)
		{
			return;
		}
		
		ParticleManager pm = ParticleManager.GetInstance();
		
		if (!pm)
		{
			return;
		}
		
		if (m_SmokeParticle)
		{
			m_SmokeParticle.Stop();
			m_SmokeParticle = null;
		}
		
		m_SmokeParticle = pm.PlayOnObject(loopId, this, Vector(0, 0.18, 0), Vector(0, 0, 0), false);
		
		if (m_SmokeParticle)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ApplyIntensityToLoopParticle, 40, false);
		}
	}
	
	protected void ApplyIntensityToLoopParticle()
	{
		if (!m_SmokeParticle)
		{
			return;
		}
		
		ApplyKothSmokeIntensity(m_SmokeParticle);
	}
	
	protected void OnSmokeRefreshTick()
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}
		
		if (!this)
		{
			return;
		}
		
		PlaySmokeGrenadeStyle();
	}
	
	protected void StopKothSmokeLoop()
	{
		if (m_SmokeParticle)
		{
			m_SmokeParticle.Stop();
			m_SmokeParticle = null;
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnSmokeRefreshTick);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AttachSmokeLoopToSelf);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ApplyIntensityToLoopParticle);
		StopKothSmokeLoop();
		super.EEDelete(parent);
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
}
