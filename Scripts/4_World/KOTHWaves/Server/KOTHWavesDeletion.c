class KOTHWavesDeletion
{
	static void DeleteEntityCargoAndAttachmentsRecursive(EntityAI item)
	{
		if (!item || !item.GetInventory())
		{
			return;
		}
		
		GameInventory inv = item.GetInventory();
		CargoBase cargo = CargoBase.Cast(inv.GetCargo());
		
		if (cargo)
		{
			int guard = 0;
			
			while (cargo.GetItemCount() > 0 && guard < 500)
			{
				guard++;
				EntityAI inner = cargo.GetItem(0);
				
				if (!inner)
				{
					break;
				}
				
				DeleteEntityCargoAndAttachmentsRecursive(inner);
				GetGame().ObjectDelete(inner);
			}
		}
		
		int attCount = inv.AttachmentCount();
		
		for (int i = attCount - 1; i >= 0; i--)
		{
			EntityAI att = inv.GetAttachmentFromIndex(i);
			
			if (att)
			{
				DeleteEntityCargoAndAttachmentsRecursive(att);
				GetGame().ObjectDelete(att);
			}
		}
	}
	
	static void DeleteRewardContainerHard(EntityAI container)
	{
		if (!container)
		{
			return;
		}
		
		DeleteEntityCargoAndAttachmentsRecursive(container);
		
		ItemBase itemBase = ItemBase.Cast(container);
		
		if (itemBase)
		{
			itemBase.SetLifetime(0);
		}
		
		GetGame().ObjectDelete(container);
		
		if (container)
		{
			EntityAI entity = EntityAI.Cast(container);
			
			if (entity)
			{
				entity.Delete();
			}
		}
	}
	
	static bool IsConfiguredRewardContainerClass(string className, array<string> rewardClassNames)
	{
		if (className == "" || !rewardClassNames)
		{
			return false;
		}
		
		for (int i = 0; i < rewardClassNames.Count(); i++)
		{
			if (rewardClassNames.Get(i) == className)
			{
				return true;
			}
		}
		
		return false;
	}
	
	static int DeleteRewardCratesNear(vector center, float radius, array<string> rewardClassNames)
	{
		if (radius < 1.0)
		{
			radius = 1.0;
		}
		
		array<Object> near = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D(center, radius, near, proxyCargos);
		
		int removed = 0;
		
		for (int i = 0; i < near.Count(); i++)
		{
			Object o = near.Get(i);
			EntityAI ent = EntityAI.Cast(o);
			
			if (!ent)
			{
				continue;
			}
			
			if (!KOTHWavesRewardCrate.Cast(ent) && !IsConfiguredRewardContainerClass(ent.GetType(), rewardClassNames))
			{
				continue;
			}
			
			DeleteRewardContainerHard(ent);
			removed++;
		}
		
		return removed;
	}
}
