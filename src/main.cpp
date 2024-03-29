#include <qpl/qpl.hpp>
//#include "C:/dev/projects/VisualStudio2022/RSAVersionUpdater/QPL/src/version_control.hpp"
#include "C:/dev/projects/C++/VisualStudio2022\RSAVersionUpdater/QPL/src/version_control.hpp"
//#include "D:/projects/VisualStudio2022/VersionUpdater/QPL/src/version_control.hpp"

namespace config {
	std::wstring default_command;
	std::wstring default_duration;
	std::wstring default_reason;
	std::wstring default_ids;

	std::wstring loaded_command;
	std::wstring loaded_duration;
	std::wstring loaded_reason;
	std::wstring loaded_ids;
}
namespace input {
	std::wstring reason;
	std::wstring command;
	std::wstring duration;
	std::wstring ids;
}

bool all_skips(std::wstring input) {
	if (input.empty()) {
		return false;
	}
	for (auto& i : input) {
		bool skip = (i == L'*' || i == L'#');
		if (!skip) {
			return false;
		}
	}
	return true;
}

void cmd_gen(bool first) {
	qpl::size skip_next = 0u;

	qpl::print("command  > ");
	auto input = qpl::get_input_wstring();

	auto command_ids_right_away = qpl::string_split_numbers<qpl::u32>(input);
	if (command_ids_right_away.size()) {

		qpl::print("\r");
		qpl::println("command  > *");
		qpl::println("reason   > *");
		qpl::println("duration > *");
		qpl::println("id       > ", input);
		input::command = config::default_command;
		input::reason = config::default_reason;
		input::duration = config::default_duration;
		config::default_ids = input::ids = input;
	}
	else {
		input::command = input;

		if (all_skips(input)) {
			skip_next = input.length() - 1;
			input::command = config::default_command;
		}
		if (input == L"*") {
			input::command = config::default_command;
		}
		config::default_command = input::command;

		qpl::print("reason   > ");
		if (skip_next) {
			qpl::println("*");
			input::reason = config::default_reason;
			--skip_next;
		}
		else {
			input = qpl::get_input_wstring();
			input::reason = input;
			if (all_skips(input)) {
				skip_next = input.length() - 1;
				input::reason = config::default_reason;
			}
			if (input == L"*") {
				input::reason = config::default_reason;
			}
			config::default_reason = input::reason;
		}

		qpl::print("duration > ");
		if (skip_next) {
			qpl::println("*");
			input::duration = config::default_duration;
			--skip_next;
		}
		else {
			input = qpl::get_input_wstring();
			input::duration = input;
			if (all_skips(input)) {
				skip_next = input.length() - 1;
				input::duration = config::default_duration;
			}
			if (input == L"*") {
				input::duration = config::default_duration;
			}
			config::default_duration = input::duration;
		}

		qpl::print("ids      > ");
		if (skip_next) {
			qpl::println("*");
			input::ids = config::default_ids;
			--skip_next;
		}
		else {
			input = qpl::get_input_wstring();
			input::ids = input;
			if (all_skips(input)) {
				skip_next = input.length() - 1;
				input::ids = config::default_ids;
			}
			if (input == L"*") {
				input::ids = config::default_ids;
			}
			config::default_ids = input::ids;
		}
	}

	auto ids = qpl::string_split_numbers<qpl::u32>(input::ids);

	std::vector<std::wstring> commands;
	for (auto& id : ids) {

		std::vector<std::wstring> inputs;
		inputs.push_back(input::command);
		inputs.push_back(qpl::to_wstring(id));
		if (!input::duration.empty()) inputs.push_back(input::duration);
		if (!input::reason.empty()) inputs.push_back(input::reason);

		std::wstring cmd;
		bool first = true;
		for (auto& i : inputs) {
			if (!first) {
				cmd += L' ';
			}
			first = false;
			cmd += i;
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

int main(int argc, char** argv) try {
	qpl::println(qpl::foreground::bright_white, "(C)ReD (ReD#7561, https://github.com/DanielRabl)");
	qpl::println(qpl::foreground::bright_white, "creates command chains and copies them into your clipboard (CTRL + V)\n");

	if (argc <= 1) {
		auto result = version_control::auto_updater(argv[0]);
		if (result) {
			return 0;
		}
	}

	//qpl::println(qpl::yellow, "\nThis application is used soley to update the directory!\nRun the update to use the actual program.");
	//qpl::system_pause();
	//return 0;

	qpl::winsys::enable_utf();
	
	if (!qpl::filesys::exists("cmd.cfg")) {
		qpl::filesys::write_data_file("default command: ban\ndefault duration: 30\ndefault reason : block on playground", "cmd.cfg");
	}
	
	qpl::config config;
	config.wload("cmd.cfg");
	config::default_command = config.wget(0u);
	config::default_duration = config.wget(1u);
	config::default_reason = config.wget(2u);
	
	config::loaded_command = config::default_command;
	config::loaded_duration = config::default_duration;
	config::loaded_reason = config::default_reason;
	
	qpl::println("default values: ");
	qpl::println(" - if command  is '*': \"", config::loaded_command, "\" is selected");
	qpl::println(" - if duration is '*': \"", config::loaded_duration, "\" is selected");
	qpl::println(" - if reason   is '*': \"", config::loaded_reason, "\" is selected");
	qpl::println("any empty value will be ignored.");
	qpl::println("multiple '*' apply for multiple lines.");
	qpl::println("a '#' will use the default value from config file.");
	qpl::println("for any runs beyond the first your previous inputs will be the new default values.");
	qpl::println("\nexample:\n");
	qpl::println("command  > ban");
	qpl::println("reason   > blocking");
	qpl::println("duration > 10");
	qpl::println("ids      > 1, 5, 21");
	qpl::println("");
	qpl::println("OUTPUT:");
	qpl::println("ban 1 10 blocking; ban 5 10 blocking; ban 21 10 blocking;");
	qpl::println("( copied to clipboard )");
	
	
	if (argc > 1) {
		std::string output = "";
		for (qpl::size i = 1u; i < qpl::size_cast(argc); ++i) {
			if (i != 1u) {
				output += ' ';
			}
			output += argv[i];
		}
		qpl::println();
		qpl::println(" > ", qpl::foreground::aqua, output);
		qpl::println(" > ", qpl::foreground::aqua, qpl::to_string(" version = ", version_control::find_version(argv[0])));
		qpl::println();
	}
	
	bool first = true;
	while (true) {
		qpl::println("\n");
		qpl::println_repeat("- ", 50);
		qpl::println("\n");
	
		cmd_gen(first);
		first = false;
	}
}
catch (std::exception& any) {
	qpl::println(qpl::foreground::light_red, "caught exception: ", any.what());
	qpl::system_pause();
}