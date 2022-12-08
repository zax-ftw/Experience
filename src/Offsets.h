#pragma once

namespace RE
{
	namespace Offset
	{
		constexpr auto RealTime = RELOCATION_ID(523662, 410201);  // 02F6B950, 1430064D0
		constexpr auto FileExists = RELOCATION_ID(80087, 82411);

		namespace Actor
		{
			constexpr auto Kill = RELOCATION_ID(36872, 37896);       // 00603B30, 14062B1E0
			constexpr auto Resurrect = RELOCATION_ID(36331, 37321);  // 005D5290, 1405F99E0
		}

		namespace PlayerCharacter
		{
			constexpr auto Ctor = RELOCATION_ID(39340, 40411);             // 00699040, 1406C0A00
			constexpr auto InitActorValues = RELOCATION_ID(39415, 40490);  // 006A25A0, 1406CAC60
			constexpr auto sub_69ABF0 = RELOCATION_ID(39346, 40417);       // 0069ABF0, 1406C28E0
			constexpr auto DiscoveryCheck = RELOCATION_ID(39663, 40750);   // 006B8340, 1406E1900
		}

		namespace PlayerSkills
		{
			constexpr auto Ctor = RELOCATION_ID(39231, 40307);               // 00694710, 1406BC3A0
			constexpr auto InitSkills = RELOCATION_ID(40565, 41572);         // 006E69A0, 14070F8A0
			constexpr auto EvaluateSkillData = RELOCATION_ID(40566, 41573);  // 006E6AD0, 14070F9D0
			constexpr auto GetLevelData = RELOCATION_ID(40553, 41560);       // 006E6180, 14070EBC0
			constexpr auto GetSkillData = RELOCATION_ID(40552, 41559);       // 006E6130, 14070EB70
			constexpr auto ModSkillPoints = RELOCATION_ID(40554, 41561);     // 006E61D0, 14070EC10
			constexpr auto ModSkillLevels = RELOCATION_ID(40555, 41562);     // 006E64D0, 14070F000
			constexpr auto SetLevel = RELOCATION_ID(40556, 41563);           // 006E65C0, 14070F100
			constexpr auto CanLevelUp = RELOCATION_ID(40558, 41565);         // 006E6670, 14070F1B0
		}

		namespace BGSLocation
		{
			constexpr auto ClearedCheck = RELOCATION_ID(17965, 18369);  // 00243380, 140253860
			constexpr auto TryToClear = RELOCATION_ID(24526, 25055);    // 00375AF0, 14038C940
		}

		namespace Unknown
		{
			constexpr auto LocationMarkerRelated = RELOCATION_ID(24647, 25118);  // 0037F050, 140393810
		}

		namespace HUDNotifications
		{
			constexpr auto Update = RELOCATION_ID(50757, 51652);  // 008808D0, 1408B0B20
		}

		namespace TESQuest
		{
			constexpr auto CompleteQuest = RELOCATION_ID(24472, 24991);  // 00370290, 140387960
		}

		namespace BGSQuestObjective
		{
			constexpr auto FillQuestInstanceData = RELOCATION_ID(23429, 23897);  // 0033A820, 1403510B0
		}

		namespace TrainingMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51788, 52662);  // 008CE590, 1408FD610
			constexpr auto Train = RELOCATION_ID(51793, 52667);           // 008CE8E0, 1408FD960
			constexpr auto SetSkillXpPercent = RELOCATION_ID(51795, 52669);

			namespace TrainCallback
			{
				constexpr auto Accept = RELOCATION_ID(51790, 52664);  // 008CE6F0, 1408FD760
			}
		}

		namespace StatsMenu
		{
			constexpr auto ProcessMessage = RELOCATION_ID(51638, 52510);   // 008BF360, 1408EE960
			constexpr auto UpdateSkillList = RELOCATION_ID(51652, 52525);  // 008C20C0, 1408F16A0
		//	constexpr auto UpdateSelectedSkillInfo = RELOCATION_ID(0, 0);
		}

		namespace ConfirmLevelUpAttributeCallback
		{
			constexpr auto Run = RELOCATION_ID(51037, 51917);  // 00893F80, 1408C4700
		}

		namespace TweenMenu
		{
			constexpr auto sub_8D16A0 = RELOCATION_ID(51843, 52715);  // 008D11F0, 140900550
		}

		namespace Console
		{
			constexpr auto UpdateLevel = RELOCATION_ID(21887, 22369);  // 00300210, 140314C20
		}

		namespace Main
		{
			constexpr auto sub_5B5490 = RELOCATION_ID(35593, 36601);  // 005B5490, 1405DCD70
			constexpr auto sub_5B6DC0 = RELOCATION_ID(35620, 36630);  // 005B6DC0, 1405DEB80
		}

		namespace StoryEvents
		{
			constexpr auto LocationCleared = RELOCATION_ID(18046, 18435);
			constexpr auto ObjectiveState = RELOCATION_ID(23486, 23951);
			constexpr auto QuestStatus = RELOCATION_ID(24719, 25196);
		}

	}

}
