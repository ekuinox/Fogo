#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <type_traits>

namespace Fogo::Utility {
	template <typename EventType, typename EventDetail>
	class PubSub
	{
	public:
		using Subscriber = std::function<void(EventDetail)>;
	private:
		struct Subscribers : public std::unordered_map<EventType, std::vector<Subscriber>>
		{
			static Subscribers & GetInstance()
			{
				static Subscribers instance;
				return instance;
			}
		};
	public:
		static void RegisterSubscriber(EventType type, Subscriber subscriber)
		{
			Subscribers::GetInstance()[type].emplace_back(subscriber);
		}
		static void Publish(EventType type, const EventDetail & detail)
		{
			for (const auto & subscriber : Subscribers::GetInstance()[type]) subscriber(detail);
		}
		static size_t GetSubscriberCount(EventType type)
		{
			return Subscribers::GetInstance()[type].size();
		}
	};

	// ÉCÉxÉìÉgè⁄ç◊Ç»ÇµÇÃì¡éÍâª
	template <typename EventType>
	class PubSub<EventType, void>
	{
	public:
		using Subscriber = std::function<void(void)>;
	private:
		struct Subscribers : public std::unordered_map<EventType, std::vector<Subscriber>>
		{
			static Subscribers & GetInstance()
			{
				static Subscribers instance;
				return instance;
			}
		};
	public:
		static void RegisterSubscriber(EventType type, Subscriber subscriber)
		{
			Subscribers::GetInstance()[type].emplace_back(subscriber);
		}
		static void Publish(EventType type)
		{
			for (const auto & subscriber : Subscribers::GetInstance()[type]) subscriber();
		}
		static size_t GetSubscriberCount(EventType type)
		{
			return Subscribers::GetInstance()[type].size();
		}
	};
}