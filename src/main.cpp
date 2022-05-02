#include <qpl/qpl.hpp>

namespace config {
	std::string default_command;
	std::string default_duration;
	std::string default_reason;
}

void cmd_gen() {
	qpl::print("command  > ");

	auto input = qpl::get_input();


	std::string command;
	if (input.empty()) {
		command = config::default_command;
	}
	else {
		command = input;
	}

	qpl::print("reason   > ");

	input = qpl::get_input();

	std::string reason;
	if (input.empty()) {
		reason = config::default_reason;
	}
	else {
		reason = input;
	}

	qpl::print("duration > ");

	input = qpl::get_input();

	std::string duration;
	if (input.empty()) {
		duration = config::default_duration;
	}
	else {
		duration = input;
	}


	qpl::print("ids      > ");
	input = qpl::get_input();
	auto ids = qpl::split_numbers<qpl::u32>(input);

	if (ids.empty()) {
		qpl::println("invalid input, no ids.\n");
		cmd_gen();
	}

	qpl::println("\nOUTPUT: ");
	std::string cmd_chain;

	for (auto& id : ids) {
		std::string cmd;
		if (reason.empty()) {
			cmd = qpl::to_string(command, " ", id, " ", duration);
		}
		else {
			cmd = qpl::to_string(command, " ", id, " ", duration, " ", reason);
		}
		cmd_chain += cmd + "; ";
	}
	qpl::println(cmd_chain);
	qpl::copy_to_clipboard(cmd_chain);
	qpl::println("( copied to clipboard )");
}

int main() {

	qpl::println("(C)ReD (ReD#7561, https://github.com/DanielRabl)");
	qpl::println("creates command chains and copies them into your clipboard (CTRL + V)\n");

	qpl::config config;
	config.load("default.cfg");
	config::default_command = config.get(0u);
	config::default_duration = config.get(1u);
	config::default_reason = config.get(2u);

	qpl::println("default values: ");
	qpl::println(" - if command  is empty: \"", config::default_command, "\" is selected");
	qpl::println(" - if duration is empty: \"", config::default_duration, "\" is selected");
	qpl::println(" - if reason   is empty: \"", config::default_reason, "\" is selected");
	qpl::println("\nexample:\n");
	qpl::println("command  > ban");
	qpl::println("reason   > blocking");
	qpl::println("duration > 10");
	qpl::println("ids      > 1, 5, 21");
	qpl::println("");
	qpl::println("OUTPUT:");
	qpl::println("ban 1 10 blocking; ban 5 10 blocking; ban 21 10 blocking;");
	qpl::println("( copied to clipboard )");


	while (true) {
		qpl::println("\n");
		qpl::println_repeat("- ", 50);
		qpl::println("\n");

		cmd_gen();
	}
}