#pragma once

namespace RE
{
	namespace Offset
	{
		namespace Actor
		{
			constexpr auto Kill = RELOCATION_ID(36872, 37896);
			constexpr auto Resurrect = RELOCATION_ID(36331, 37321);
			constexpr auto DamageHealth = RELOCATION_ID(36345, 37335);
			constexpr auto IsCombatTarget = RELOCATION_ID(37618, 38571);
		}

		namespace PlayerCharacter
		{
			constexpr auto Ctor = RELOCATION_ID(39340, 40411);
			constexpr auto InitActorValues = RELOCATION_ID(39415, 40490);
			constexpr auto StopWaitSleep = RELOCATION_ID(39346, 40417);
			constexpr auto DiscoveryCheck = RELOCATION_ID(39663, 40750);
		}

		namespace PlayerSkills
		{
			constexpr auto Ctor = RELOCATION_ID(39231, 40307);
			constexpr auto InitSkills = RELOCATION_ID(40565, 41572);
			constexpr auto EvaluateSkillData = RELOCATION_ID(40566, 41573);
			constexpr auto GetLevelData = RELOCATION_ID(40553, 41560);
			constexpr auto GetSkillData = RELOCATION_ID(40552, 41559);
			constexpr auto ModSkillPoints = RELOCATION_ID(40554, 41561);
			constexpr auto ModSkillLevels = RELOCATION_ID(40555, 41562);
			constexpr auto SetLevel = RELOCATION_ID(40556, 41563);
			constexpr auto CanLevelUp = RELOCATION_ID(40558, 41565);
		}

		namespace BGSLocation
		{
			constexpr auto ClearedCheck = RELOCATION_ID(17965, 18369);
			constexpr auto TryToClear = RELOCATION_ID(24526, 25055);
		}

		namespace Unknown
		{
			constexpr auto LocationMarkerRelated = RELOCATION_ID(24647, 25118);
			constexpr auto HandleAction = RELOCATION_ID(38048, 39004);
		}

		namespace HUDNotifications
		{
			constexpr auto Update = RELOCATION_ID(50757, 51652);
		}

		namespace BGSQuestObjective
		{
			constexpr auto FillQuestInstanceData = RELOCATION_ID(23429, 23897);
		}

		namespace TrainingMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51788, 52662);
			constexpr auto Train = RELOCATION_ID(51793, 52667);
			constexpr auto SetSkillXpPercent = RELOCATION_ID(51795, 52669);

			namespace TrainCallback
			{
				constexpr auto Accept = RELOCATION_ID(51790, 52664);
			}
		}

		namespace StatsMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51638, 52510);
			constexpr auto UpdateSkillList = RELOCATION_ID(51652, 52525);
		}

		namespace ConfirmLevelUpAttributeCallback
		{
			constexpr auto Run = RELOCATION_ID(51037, 51917);
		}

		namespace TweenMenu
		{
			constexpr auto sub_8D16A0 = RELOCATION_ID(51843, 52715);
		}

		namespace Console
		{
			constexpr auto UpdateLevel = RELOCATION_ID(21887, 22369);
		}

		namespace Main
		{
			constexpr auto sub_5B5490 = RELOCATION_ID(35593, 36601);
			constexpr auto sub_5B6DC0 = RELOCATION_ID(35620, 36630);
		}

	}

}
