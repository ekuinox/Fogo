#pragma once
#include <Fogo.h>

inline void test1() {
	using namespace Fogo::Utility;
	using namespace Fogo::Graphics::DX12;
	using namespace Microsoft::WRL;
	using Fogo::Game::System;
	using Fogo::Game::Component;

	class FooComponent : public Component {
		const char * name = "";
		unsigned char age = 0;
	public:
		FooComponent(const char * name_, unsigned int age_) : name(name_), age(age_) {}
		void echoNameAndAge() const {
			printf("name=%s, age=%d\n", name, age);
		}
	};

	class BarComponent : public Component {
		enum class Key {
			First,
			Second,
			Third
		};
		int number;
	public:
		BarComponent(int number_) : number(number_) {
			create<FooComponent>("first", number).makeIndex(Key::First);
			create<FooComponent>("second", number).makeIndex(Key::Second);
			create<FooComponent>("third", number).makeIndex(Key::Third);
		}
		void test() const {
			for (const auto & key : { Key::First, Key::Second, Key::Third }) {
				if (const auto & component = get<FooComponent>(key)) {
					component->echoNameAndAge();
				}
			}
		}
	};

	class BazComponent : public Component {
		enum class Key {
			One,
			Two,
			Three
		};
	public:
		BazComponent() {
			create<BarComponent>(10).makeIndex(Key::One);
			create<BarComponent>(20).makeIndex(Key::Two);
			create<BarComponent>(30).makeIndex(Key::Three);
		}
		void test() const {
			for (const auto & key : { Key::One, Key::Two, Key::Three }) {
				if (const auto & component = get<BarComponent>(key)) {
					component->test();
				}
			}
		}
	};

	BazComponent component;

	component.test();
}
