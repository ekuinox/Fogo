#include <iostream>
#include "./StoreImpl.h"
#include "./ComponentImpl.h"

#include <variant>
#include <optional>

/*

C++‚Ì‹@”\‚Æ‚©‘‚¢‚Ä‚İ‚½‚¢‚â‚Â‚ª‚©‚¯‚é‚©‚È‚Ç‚¨‚ê‚ª‚·êŠ@‚»‚Ì‚P

*/

template <typename E, typename V>
struct Result final {
	std::variant<E, V> r;
	bool success;
	std::function<void(E)> onFailure = [](E) {};
	std::function<void(V)> onSucces = [](V) {};

	Result(const E & e) : r(e), success(false) {}
	Result(const V & v) : r(v), success(true) {}
	~Result() {
		if (success) onSucces(std::get<V>(r));
		else onFailure(std::get<E>(r));
	}

	Result & done(const std::function<void(V)> func) {
		onSucces = func;
		return * this;
	}

	Result & fail(const std::function<void(E)> func) {
		onFailure = func;
		return * this;
	}

	bool operator==(const E & e) const {
		if (success) return false;
		return e == std::get<E>(r);
	}

	std::optional<V> get() const {
		if (success) return std::move(std::get<V>(r));
		return std::nullopt;
	}

	V operator*() const {
		if (success) return std::move(std::get<V>(r));
	}

	static Result right(const V & v) {
		return Result(v);
	}
	static Result left(const E & e) {
		return Result(e);
	}
};

enum class ErrorType {
	Error
};

struct Cm : Component {
	~Cm() {
		std::cout << "destruct" << std::endl;
	}
};

Result<ErrorType, Cm *> create(int n) {
	if (n < 0) return ErrorType::Error;
	return new Cm;
}

auto main() -> int {

	create(-1).fail([](ErrorType error) {
		std::cout << "error!" << std::endl;
	}).done([](Cm * component) {
		std::cout << "success" << std::endl;
		std::cout << component->uuid << std::endl;
	});

	std::cout << "-----" << std::endl;

	const auto component = *create(10).get();

	std::cout << component->uuid << std::endl;

	delete component;

	/*
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

	*/

	getchar();

	return 0;
}
