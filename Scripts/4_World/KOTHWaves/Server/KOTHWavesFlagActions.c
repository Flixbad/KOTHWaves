class KOTHWavesFlagActionBlock
{
	static bool IsBlockedKothFlag(Object targetObject)
	{
		return KOTHWavesTerritoryFlag.IsKothWavesFlagEntity(targetObject);
	}
	
	static bool IsBlockedKothFlagTarget(ActionTarget target)
	{
		if (!target)
		{
			return false;
		}
		
		if (IsBlockedKothFlag(target.GetObject()))
		{
			return true;
		}
		
		return IsBlockedKothFlag(target.GetParent());
	}
}

modded class ActionRaiseFlag
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (KOTHWavesFlagActionBlock.IsBlockedKothFlagTarget(target))
		{
			return false;
		}
		
		return super.ActionCondition(player, target, item);
	}
}

modded class ActionLowerFlag
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (KOTHWavesFlagActionBlock.IsBlockedKothFlagTarget(target))
		{
			return false;
		}
		
		return super.ActionCondition(player, target, item);
	}
}

modded class ActionFoldBaseBuildingObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (KOTHWavesFlagActionBlock.IsBlockedKothFlagTarget(target))
		{
			return false;
		}
		
		return super.ActionCondition(player, target, item);
	}
}

modded class ActionWorldFlagActionSwitch
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (KOTHWavesFlagActionBlock.IsBlockedKothFlagTarget(target))
		{
			return false;
		}
		
		return super.ActionCondition(player, target, item);
	}
}
