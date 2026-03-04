/**********************************************************/
//
// File: main_entry.hpp
//
// Purpose: main classes for running dt
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_WINDOW_INCLUDE_PATH
#include CORE_UI_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_TOUT_INCLUDE_PATH
#include CORE_SYSINIT_INCLUDE_PATH
#include CORE_DRAWING_INCLUDE_PATH
#include CORE_FRONTEND_INCLUDE_PATH
#include CORE_BACKEND_INCLUDE_PATH
#include CORE_MESSAGES_INCLUDE_PATH



namespace core {
	namespace main {
#if !TERMINAL_BUILD
		class Cmain : public core::main::messages
		{
		public:
			Cmain(core::main::build type) 
				:messages(type) {}

			virtual void go() {
				std::jthread backend_begin_t([this] { m_backend->begin(); });
				std::jthread process_message_t([this] { process(); });
				m_frontend->message_pump();
				exit();
			}
		protected:
			virtual void exit() {
				DestroyWindow(core::logger::glb_sl->get_window_handle());
				m_backend->m_run_watch.store(false);
				m_run_messages.store(false);
				PostQueuedCompletionStatus(m_backend->m_hCompletionPort, 0, m_backend->m_completionKey, m_backend->m_pOverlapped);
			}
		};
#else
		class Cmain : public core::main::messages {
		public:
			Cmain(core::main::build type,int argc, char* argv[])
				:messages(type,argc,argv) {
			}

			virtual void go() = 0;
		protected:
			virtual void exit() = 0;
		};


		class Cterminal : public Cmain {
		public:
			Cterminal(int argc, char* argv[])
				:Cmain(build::terminal,argc,argv){}

			void go() override {
				std::jthread backend_begin_t([this] { m_backend->begin(); });
				process();
				exit();
			}
		protected:
			void exit() override {
				m_backend->m_run_watch.store(false);
				m_run_messages.store(false);
				PostQueuedCompletionStatus(m_backend->m_hCompletionPort, 0, m_backend->m_completionKey, m_backend->m_pOverlapped);
			}
		};
	}
#endif
}