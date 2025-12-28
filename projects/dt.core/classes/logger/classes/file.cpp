/**********************************************************/
//
// File: file.cpp
//
// Purpose: file.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_FILELOG_INCLUDE_PATH

core::logger::file::file(const fs::path& folder, const std::string& file_name)
{
	m_out_file_path = folder / file_name;
}

core::codes core::logger::file::load()
{
	out_file.open(m_out_file_path, std::ios::out | std::ios::trunc);

	if (!out_file.is_open()) {
		return codes::file_open_fail;
	}

	return codes::success;
}

core::codes core::logger::file::write_buffer(std::vector<log>* buffer)
{
	for (auto& log : *buffer) {
#if WIDE
		std::string message;
		try {
			message = api::to_narrow_string(*log.message);
		}
		catch (const core::dtapierror& e) {
			api::output_dtapierror(e);
		}
		catch (...) {
			api::output_cp(unknown_exception_caught_pkg);
			return core::codes::exception_thrown_and_handled;
		}
#else
		auto& message = *log.message;
#endif
		try {
			out_file << message;
		}
		catch (const std::ios_base::failure& e) {
			return codes::write_to_file_fail;
		}
		catch (...) {
			api::output_cp(unknown_exception_caught_pkg);
			return core::codes::exception_thrown_and_handled;
		}
	}

	return codes::success;
}

core::codes core::logger::file::write_message(const std::string& message)
{
	try {
		out_file << message;
	}
	catch (const std::ios_base::failure& e) {
		return codes::write_to_file_fail;
	}
	catch (...) {
		api::output_cp(unknown_exception_caught_pkg);
		return core::codes::exception_thrown_and_handled;
	}
	return codes::success;
}
