class KOTHWavesRewardCrate: WoodenCrate
{
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
}
