modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		KOTHWavesManager.GetInstance().Start();
	}
	
	override void OnMissionFinish()
	{
		KOTHWavesManager.GetInstance().Stop();
		super.OnMissionFinish();
	}
}
