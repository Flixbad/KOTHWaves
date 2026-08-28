#ifdef EXPANSIONMOD
modded class TerritoryFlag
{
	override void SetActions()
	{
		if (KOTHWavesTerritoryFlag.Cast(this))
		{
			return;
		}
		
		super.SetActions();
	}
}
#endif

#ifdef LBmaster_Groups
modded class TerritoryFlag
{
	override void EEInit()
	{
		super.EEInit();
		
		KOTHWavesTerritoryFlag kothFlag = KOTHWavesTerritoryFlag.Cast(this);
		
		if (kothFlag)
		{
			kothFlag.DisableKothTerritorySystems();
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		KOTHWavesTerritoryFlag kothFlag = KOTHWavesTerritoryFlag.Cast(this);
		
		if (kothFlag)
		{
			kothFlag.DisableKothTerritorySystems();
		}
	}
	
	override void SetRefresherActive(bool state)
	{
		if (KOTHWavesTerritoryFlag.Cast(this))
		{
			super.SetRefresherActive(false);
			return;
		}
		
		super.SetRefresherActive(state);
	}
}
#endif
