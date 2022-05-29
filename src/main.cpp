#include <qpl/qpl.hpp>

namespace config {
	std::wstring default_command;
	std::wstring default_duration;
	std::wstring default_reason;
}
namespace input {
	std::wstring reason;
	std::wstring command;
	std::wstring duration;
	std::wstring ids;
}

void cmd_gen(bool first) {
	qpl::print("command  > ");

	auto input = qpl::get_input_wstring();


	if (input.empty()) {
		if (first) {
			input::command = config::default_command;
		}
	}
	else {
		input::command = input;
	}
	bool is_kick = input::command == L"kick";

	qpl::print("reason   > ");
	input = qpl::get_input_wstring();

	if (input.empty()) {
		if (first) {
			input::reason = config::default_reason;
		}
	}
	else {
		input::reason = input;
	}


	if (!is_kick) {

		qpl::print("duration > ");

		input = qpl::get_input_wstring();

		if (input.empty()) {
			if (first) {
				input::duration = config::default_duration;
			}
		}
		else {
			input::duration = input;
		}
	}




	qpl::print("ids      > ");
	input = qpl::get_input_wstring();

	if (input.empty()) {
		if (first) {
			qpl::println("invalid input, no ids.\n");
			cmd_gen(first);
		}
	}
	else {
		input::ids = input;
	}
	auto ids = qpl::split_numbers<qpl::u32>(input::ids);

	std::vector<std::wstring> commands;
	for (auto& id : ids) {

		std::wstring cmd;
		if (input::reason.empty()) {
			if (is_kick) {
				cmd = qpl::to_wstring(input::command, " ", id);
			}
			else {
				cmd = qpl::to_wstring(input::command, " ", id, " ", input::duration);
			}
		}
		else {
			if (is_kick) {
				cmd = qpl::to_wstring(input::command, " ", id, " ", input::reason);
			}
			else {
				cmd = qpl::to_wstring(input::command, " ", id, " ", input::duration, " ", input::reason);
			}
		}
		commands.push_back(cmd + L";");
	}

	std::wstring line;

	std::vector<std::wstring> lines;
	bool first_cmd = true;
	for (auto& cmd : commands) {

		auto current_line = line;

		std::wstring next_command;
		if (!first_cmd) {
			next_command += L" ";
		}
		first_cmd = false;

		next_command += cmd;

		if (line.length() + next_command.length() > 255) {
			lines.push_back(line);
			line = L"";
			if (next_command.length()) {
				next_command = next_command.substr(1u);
			}
		}
		line += next_command;
	}
	lines.push_back(line);

	qpl::println("\nOUTPUT: ");
	if (lines.size() != 1u) {
		qpl::println("(there are ", lines.size(), " lines because of the 255 character limit in f2)\n");
	}
	for (qpl::size i = 0u; i < lines.size(); ++i) {
		qpl::println(lines[i]);
		qpl::copy_to_clipboard(lines[i]);
		if (lines.size() == 1u) {
			qpl::println("( copied to clipboard )");
		}
		else {
			qpl::print("( copied ", i + 1, " / ", lines.size(), " lines to clipboard )");
			if (i != lines.size() - 1) {
				qpl::print(". [ENTER] for next line > ");
				qpl::get_input_wstring();
			}
			qpl::println();
		}
	}
}

int main() {
	qpl::winsys::enable_utf();

	qpl::println("(C)ReD (ReD#7561, https://github.com/DanielRabl)");
	qpl::println("creates command chains and copies them into your clipboard (CTRL + V)\n");

	qpl::config config;
	config.wload("default.cfg");
	config::default_command = config.wget(0u);
	config::default_duration = config.wget(1u);
	config::default_reason = config.wget(2u);

	qpl::println("default values: ");
	qpl::println(" - if command  is empty: \"", config::default_command, "\" is selected");
	qpl::println(" - if duration is empty: \"", config::default_duration, "\" is selected");
	qpl::println(" - if reason   is empty: \"", config::default_reason, "\" is selected");
	qpl::println("(for any runs beyond the first your previous inputs will be the default values)");
	qpl::println("\nexample:\n");
	qpl::println("command  > ban");
	qpl::println("reason   > blocking");
	qpl::println("duration > 10");
	qpl::println("ids      > 1, 5, 21");
	qpl::println("");
	qpl::println("OUTPUT:");
	qpl::println("ban 1 10 blocking; ban 5 10 blocking; ban 21 10 blocking;");
	qpl::println("( copied to clipboard )");

	bool first = true;
	while (true) {
		qpl::println("\n");
		qpl::println_repeat("- ", 50);
		qpl::println("\n");

		cmd_gen(first);
		first = false;
	}
}