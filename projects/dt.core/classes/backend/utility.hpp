#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#if! UNDER_CONSTRUCTION

/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_TOUT_INCLUDE_PATH


namespace core {
	class utility : public backend_message_queue{
	public:
		utility() = default;

		/*
			outputs an error message with location data
		*/
		void output_em(const code_pkg cp, const std::string location = get_location());

		/*
			returns a count of regular file's in a given directory
		*/
		std::uintmax_t file_numbers(const std::filesystem::path& p);
	protected:

	};
}

#endif