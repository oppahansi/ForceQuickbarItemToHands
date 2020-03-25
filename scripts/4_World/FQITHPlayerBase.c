/**
 * Original implementation is done by the Bohemia Dayz developer team.
 * Modification have been done by oppahansi to fit the mods needs and keep the original functionality in tact as much as possible.
 * Modification are highlighted below.
 */

modded class PlayerBase
{
    override void OnQuickBarSingleUse(int slotClicked)
	{
		if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER )
			return;

		if( GetInventory().IsInventoryLocked() || IsEmotePlaying() )
			return;
		
		if( IsRaised() || GetCommand_Melee() || IsSwimming() || IsClimbingLadder() || IsClimbing() || IsRestrained() )
			return;
		
		if( GetDayZPlayerInventory().IsProcessing() )
			return;
		
		if( GetActionManager().GetRunningAction() != null )
			return;
		
		if (!ScriptInputUserData.CanStoreInputUserData())
			return;
			
		EntityAI quickBarEntity = GetQuickBarEntity(slotClicked - 1);
		
		if(!quickBarEntity)
			return;
		
		Magazine mag;
		Weapon_Base wpn;
		
		if( Class.CastTo(mag, quickBarEntity) && Class.CastTo(wpn, mag.GetHierarchyParent()) )
			return;

		EntityAI inHandEntity = GetHumanInventory().GetEntityInHands();
		
		if (!GetDayZPlayerInventory().IsIdle())
			return;

		InventoryLocation handInventoryLocation = new InventoryLocation;
		handInventoryLocation.SetHands(this,quickBarEntity);
		if( this.GetInventory().HasInventoryReservation(quickBarEntity, handInventoryLocation ) )
			return;
		
		if( inHandEntity == quickBarEntity )
		{
			if( GetHumanInventory().CanRemoveEntityInHands() )
			{
				syncDebugPrint("[QB] Stash - PredictiveMoveItemFromHandsToInventory HND=" + Object.GetDebugName(inHandEntity));
				PredictiveMoveItemFromHandsToInventory();
			}
		}
		else
		{
			InventoryLocation invLocQBItem = new InventoryLocation;
			quickBarEntity.GetInventory().GetCurrentInventoryLocation(invLocQBItem);
			if( GetInventory().HasInventoryReservation(quickBarEntity,invLocQBItem) )
				return;
				
			if (inHandEntity)
			{
				InventoryLocation Reserved_Item_il = new InventoryLocation;
				
				InventoryLocation inHandEntityFSwapDst = new InventoryLocation;
				inHandEntity.GetInventory().GetCurrentInventoryLocation(inHandEntityFSwapDst);
				
				int index = GetHumanInventory().FindUserReservedLocationIndex(inHandEntity);
				if( index >= 0 )
				{
					GetHumanInventory().GetUserReservedLocation( index, Reserved_Item_il);
				}
				
				if(Reserved_Item_il)
					inHandEntityFSwapDst.CopyLocationFrom(Reserved_Item_il, true);
				
				if(GameInventory.CanForceSwapEntities( quickBarEntity, null, inHandEntity, inHandEntityFSwapDst ))
				{
					syncDebugPrint("[QB] Swap - PredictiveForceSwapEntities HND=" + Object.GetDebugName(inHandEntity) +  " QB=" + Object.GetDebugName(quickBarEntity) + " fswap_dst=" + InventoryLocation.DumpToStringNullSafe(inHandEntityFSwapDst));
					PredictiveForceSwapEntities( quickBarEntity, inHandEntity, inHandEntityFSwapDst );
					/** oppa's modification **/
					return;
					/** oppa's modification end **/
				}
				else if(GameInventory.CanSwapEntities( quickBarEntity, inHandEntity ))
				{
					syncDebugPrint("[QB] PredictiveSwapEntities QB=" + Object.GetDebugName(quickBarEntity) + " HND=" + Object.GetDebugName(inHandEntity));
					PredictiveSwapEntities( quickBarEntity, inHandEntity );
				}
				
				/** oppa's modification **/
				DropItem(ItemBase.Cast(inHandEntity));
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TakeWeaponIntoHands, 250, false, quickBarEntity);
				/** oppa's modification end **/
			}
			else
			{
				if( GetInventory().HasInventoryReservation(quickBarEntity,handInventoryLocation) )
					return;
				
				if (GetInventory().CanAddEntityIntoHands(quickBarEntity) )
				{
					syncDebugPrint("[QB] Stash - PredictiveTakeEntityToHands QB=" + Object.GetDebugName(quickBarEntity));
					PredictiveTakeEntityToHands( quickBarEntity );
				}
			}
		}
	}
	
	/** oppa's modification **/
	void TakeWeaponIntoHands(EntityAI onQuickBar)
	{
		PredictiveTakeEntityToHands(onQuickBar);
	}
	/** oppa's modification end **/
}