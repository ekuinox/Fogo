#include <iostream>
#include "Component.h"
#include "Result.h"

/*

C++‚Ì‹@”\‚Æ‚©‘‚¢‚Ä‚İ‚½‚¢‚â‚Â‚ª‚©‚¯‚é‚©‚È‚Ç‚¨‚ê‚ª‚·êŠ@‚»‚Ì‚P

*/

enum class ErrorType {
	Error
};

struct Cm : Component {
	void foo() {
		std::cout << "Cm#foo" << std::endl;
	}

	~Cm() {
		std::cout << "destruct" << std::endl;
	}
};

Result<ErrorType, Cm *> create(int n) {
	if (n < 0) return ErrorType::Error;
	return new Cm;
}

Result<ErrorType, int> pow(int n) {
	if (n < 0) return ErrorType::Error;
	return n * n;
}

auto main() -> int {

	if (const auto & n = pow(20)) {
		std::cout << *n << std::endl;
	}

	if (const auto & result = create(11)) {
		std::cout << result->uuid << std::endl;
		result->foo();
	}

	const auto & result = create(-1);
	if (result) {
		std::cout << result->uuid << std::endl;
		result->foo();
	} else if (result == ErrorType::Error) {
		std::cout << "ErrorType::Error" << std::endl;
	}
	
	std::cout << "---" << std::endl;

	class Data : public Component {
	public:
		const char * name = "";
		Data(const char * name) : Component(), name(name) {}
		Data() : Data("hello") {}
		~Data() {
			std::cout << "Data::~Data()" << std::endl;
		}
	};

	class Foo : public Component, public Updatable {
	public:
		~Foo() {
			std::cout << "Foo::~Foo()" << std::endl;
		}

		void update() override {
			std::cout << "Foo::update()" << std::endl;
		}
		void echoParentName() const {
			const auto & parent = Store::GetParent<Component>(uuid);
			if (parent->uuid != uuid) {
				std::cout << parent.element->uuid << std::endl;
			}
		}
	};

	class Bar : public Component, public Renderable {
	public:
		~Bar() {
			std::cout << "Bar::~Bar()" << std::endl;
		}

		void render() const override {
			std::cout << "Bar::render()" << std::endl;
		}
	};

	class Scene : public Component, public Updatable, public Renderable {
		class X : public Component {
		public:
			~X() {
				std::cout << "X::~X()" << std::endl;
			}
		};
	public:

		enum class DataIndex {
			First, Second, Third
		};

		Scene() : Component() {
			create<Foo>();
			create<Bar>();
			create<X>();
			create<Data>().makeIndex(DataIndex::First);
		}

		~Scene() {
			std::cout << "Scene::~Scene()" << std::endl;
		}

		void update() override {
			execute<Updatable>([](Updatable & object) {
				object.update();
			});
		}

		void render() const override {
			execute<Renderable>([](Renderable & object) {
				object.render();
			});
		}
	};

	auto scene = Store::Create<Scene>();

	std::cout << scene.element->uuid << std::endl;
	
	scene->execute<Foo>([](Foo & foo) {
		foo.echoParentName();
	});

	std::cout << Store::Get<Data>(Scene::DataIndex::First).name << std::endl;

	std::cout << Store::GetSize() << std::endl;

	Store::Free();

	std::cout << Store::GetSize() << std::endl;

	

	getchar();

	return 0;
}
