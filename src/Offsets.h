#pragma once

namespace RE
{
	namespace Offset
	{
		namespace Actor
		{
			constexpr auto KillImpl = RELOCATION_ID(36872, 37896);
			constexpr auto Resurrect = RELOCATION_ID(36331, 37321);
			constexpr auto DoDamage = RELOCATION_ID(36345, 37335);
		}

		namespace PlayerCharacter
		{
			constexpr auto Ctor = RELOCATION_ID(39340, 40411);
			constexpr auto InitValues = RELOCATION_ID(39415, 40490);
			constexpr auto WakeUp = RELOCATION_ID(39346, 40417);
			constexpr auto UpdateNearbyMapMarkers = RELOCATION_ID(39663, 40750);
			constexpr auto UseSkill = RELOCATION_ID(39413, 40488);
		}

		namespace PlayerSkills
		{
			constexpr auto Ctor = RELOCATION_ID(39231, 40307);
			constexpr auto InitializeData = RELOCATION_ID(40565, 41572);
			constexpr auto EvaluateSkillData = RELOCATION_ID(40566, 41573);
			constexpr auto GetLevelInfo = RELOCATION_ID(40553, 41560);
			constexpr auto GetSkillInfo = RELOCATION_ID(40552, 41559);
			constexpr auto UseSkill = RELOCATION_ID(40554, 41561);
			constexpr auto IncrementSkill = RELOCATION_ID(40555, 41562);
			constexpr auto ForceLevel = RELOCATION_ID(40556, 41563);
			constexpr auto IsReadyToLevelUp = RELOCATION_ID(40558, 41565);
		}

		namespace BGSLocation
		{
			constexpr auto CheckLocationCleared = RELOCATION_ID(17965, 18369);
			constexpr auto CanLocBeAlias = RELOCATION_ID(24526, 25055);
		}

		namespace GetAllGoodLocationsFunctor
		{
			constexpr auto Run = RELOCATION_ID(24647, 25118);
		}

		namespace ActorStateManager
		{
			constexpr auto SetStates = RELOCATION_ID(38048, 39004);
		}

		namespace HUDNotifications
		{
			constexpr auto Update = RELOCATION_ID(50757, 51652);
		}

		namespace BGSQuestInstanceText
		{
			constexpr auto ParseString = RELOCATION_ID(23429, 23897);
		}

		namespace TrainingMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51788, 52662);
			constexpr auto Train = RELOCATION_ID(51790, 52664);
			constexpr auto TrainSkill = RELOCATION_ID(51793, 52667);
			constexpr auto UpdateSkillMeter = RELOCATION_ID(51795, 52669);
		}

		namespace StatsMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51638, 52510);
			constexpr auto InitSkills = RELOCATION_ID(51652, 52525);
			constexpr auto UpdateDescriptionCard = RELOCATION_ID(51654, 52527);
		}

		namespace ConfirmLevelUpAttributeCallback
		{
			constexpr auto Run = RELOCATION_ID(51037, 51917);
		}

		namespace TweenMenu
		{
			constexpr auto StartMenuOpenAnim = RELOCATION_ID(51843, 52715);
		}

		namespace Console
		{
			constexpr auto UpdateLevel = RELOCATION_ID(21887, 22369);
		}

		namespace Main
		{
			constexpr auto PerformGameReset = RELOCATION_ID(35593, 36601);
			constexpr auto UpdatePlayer = RELOCATION_ID(35620, 36630);
		}

	}

}
