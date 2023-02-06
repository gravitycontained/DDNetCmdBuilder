#include <qpl/qpl.hpp>

int main() try {
	qpl::println("hello world!");
}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}