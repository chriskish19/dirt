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
#include CORE_FRONTEND_INCLUDE_PATH
#include CORE_BACKEND_INCLUDE_PATH


namespace core{
	namespace main {
		enum class build {
			gui,
			gui_with_terminal,
			terminal
		};


		class messages {
		public:
			messages(build type);
			messages(build type, int argc, char* argv[]);

			void process();
			std::vector<core::arg_entry> get_entry_v() { return m_entry_v; }
		protected:
			std::atomic<bool> m_run_messages = true;
			std::unique_ptr<core::backend::Cbackend> m_backend = nullptr;
			std::unique_ptr<core::frontend::Cfrontend> m_frontend = nullptr;
			std::vector<core::arg_entry> m_entry_v;
		};
	}
}