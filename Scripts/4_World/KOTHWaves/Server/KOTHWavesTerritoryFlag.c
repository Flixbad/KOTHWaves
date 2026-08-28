class KOTHWavesTerritoryFlag: TerritoryFlag
{
	protected bool m_KothWavesFlagReady;
	
	void KOTHWavesTerritoryFlag()
	{
		m_KothWavesFlagReady = false;
	}
	
	bool IsKothWavesEventFlag()
	{
		return true;
	}
	
	void PrepareForKOTH()
	{
		PrepareForKOTH("");
	}
	
	void PrepareForKOTH(string flagAttachmentClassname)
	{
		BuildAllParts();
		EnsureFlagAttachment(flagAttachmentClassname);
		AnimateFlag(1.0);
		SetSynchDirty();
		m_KothWavesFlagReady = true;
		DisableKothTerritorySystems();
		
		if (GetGame().IsServer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DisableKothTerritorySystems, 250, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DisableKothTerritorySystems, 1500, false);
		}
	}
	
	void DisableKothTerritorySystems()
	{
		m_RefresherTimeRemaining = 0;
		m_RefreshTimeCounter = 0;
		SetRefresherActive(false);
		RemoveRefresherPosition();
		SetSynchDirty();
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		if (GetGame().IsServer())
		{
			DisableKothTerritorySystems();
		}
	}
	
	override void SetActions()
	{
		// No raise / lower / fold / claim — KOTH progress is handled by the event script only.
	}
	
	override bool CanUseConstruction()
	{
		return false;
	}
	
	override bool CanUseConstructionBuild()
	{
		return false;
	}
	
	override bool CanDisplayAttachmentCategory(string category_name)
	{
		return false;
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!GetGame().IsServer() || !m_KothWavesFlagReady)
		{
			return false;
		}
		
		string slotName = InventorySlots.GetSlotName(slotId);
		
		if (slotName == "Material_FPole_Flag")
		{
			return true;
		}
		
		return false;
	}
	
	override void OnPartBuiltServer(notnull Man player, string part_name, int action_id)
	{
	}
	
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
	}
	
	override void OnPartDestroyedServer(Man player, string part_name, int action_id, bool destroyed_by_connected_part = false)
	{
	}
	
	override void OnCEUpdate()
	{
		if (m_RefresherTimeRemaining != 0 || m_RefresherActive)
		{
			DisableKothTerritorySystems();
		}
	}
	
	override void SetRefresherActive(bool state)
	{
		super.SetRefresherActive(false);
	}
	
	override void InsertRefresherPosition()
	{
	}
	
	override void HandleRefreshers()
	{
	}
	
	override void AddRefresherTime01(float fraction)
	{
	}
	
	override void SetRefreshTimer01(float fraction)
	{
	}
	
	override bool IsPlayerInside(PlayerBase player, string selection)
	{
		return false;
	}
	
	override bool IsFacingPlayer(PlayerBase player, string selection)
	{
		return false;
	}
	
	override bool HasProperDistance(string selection, PlayerBase player)
	{
		return false;
	}
	
	override string GetConstructionKitType()
	{
		return "";
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	static bool IsKothWavesFlagEntity(Object object)
	{
		return KOTHWavesTerritoryFlag.Cast(object) != null;
	}
	
	protected void BuildAllParts()
	{
		Construction construction = GetConstruction();
		
		if (!construction)
		{
			return;
		}
		
		SetBaseState(true);
		
		map<string, ref ConstructionPart> parts = construction.GetConstructionParts();
		
		for (int i = 0; i < parts.Count(); i++)
		{
			ConstructionPart part = parts.GetElement(i);
			
			if (!part)
			{
				continue;
			}
			
			construction.AddToConstructedParts(parts.GetKey(i));
			RegisterPartForSync(part.GetId());
			SetPartFromSyncData(part);
		}
		
		SynchronizeBaseState();
		UpdateVisuals();
		UpdatePhysics();
	}
	
	protected void EnsureFlagAttachment(string flagAttachmentClassname)
	{
		EntityAI att;
		
		while (true)
		{
			att = FindAttachmentBySlotName("Material_FPole_Flag");
			
			if (!att)
			{
				break;
			}
			
			GetGame().ObjectDelete(att);
		}
		
		string cls = flagAttachmentClassname;
		
		if (cls == "")
		{
			cls = "Flag_DayZ";
		}
		
		GetInventory().CreateAttachment(cls);
	}
}
