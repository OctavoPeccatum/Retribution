#pragma once

class AnimUtils
{
public:

	template<typename T>
	static T* FindNotifyByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}

		return nullptr;
	}

	template<typename T>
	static T* FindNotifyStateByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.NotifyStateClass);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}

		return nullptr;
	}
};