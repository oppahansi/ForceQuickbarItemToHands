class CfgPatches
{
	class ForceQuickbarItemToHands
	{
		requiredAddons[]=
		{
			"DZ_Data"
		};
		requiredVersion=0.1;
		units[]={};
		weapons[]={};
	};
};
class CfgMods
{
	class ForceQuickbarItemToHands_Scripts
	{
		dir="ForceQuickbarItemToHands";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Force Quickbar Item To Hands";
		credits="oppahansi";
		author="oppahansi";
		authorID="";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"ForceQuickbarItemToHands/scripts/4_world"
				};
			};
		};
	};
};
