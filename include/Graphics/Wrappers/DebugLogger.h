#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <shared_mutex>
#include <mutex>
#include "../HandleTypes/DebugUtilsMessenger.h"

namespace Graphics::Wrappers {
	class DebugLogger {
	public:
		struct Message {
			Flags::DebugMessageSeverity::Bits messageSeverity;
			Flags::DebugMessageType messageType;
			std::string message;
			std::string messageIDName;
			std::chrono::steady_clock::time_point timestamp;
		};

	private:
		std::vector<Message> m_messages;
		std::array<std::vector<size_t>, 4> m_messageIndicesBySeverity;
		std::array<std::vector<size_t>, 4> m_messageIndicesByType;
		
		mutable std::shared_mutex m_mutex;
	public:
		DebugLogger() = default;
		~DebugLogger() = default;

		DebugLogger(const DebugLogger&) = delete;
		DebugLogger& operator=(const DebugLogger&) = delete;

		DebugLogger(DebugLogger&& other) {
			std::unique_lock lock1(m_mutex);
			std::unique_lock lock2(other.m_mutex);

			m_messages = std::exchange(other.m_messages, std::vector<Message>());
			m_messageIndicesBySeverity = std::exchange(other.m_messageIndicesBySeverity, std::array<std::vector<size_t>, 4>());
			m_messageIndicesByType = std::exchange(other.m_messageIndicesByType, std::array<std::vector<size_t>, 4>());
		}
		DebugLogger& operator=(DebugLogger&& other) {
			if (this == &other) return *this;
			std::unique_lock lock1(m_mutex);
			std::unique_lock lock2(other.m_mutex);

			m_messages = std::exchange(other.m_messages, std::vector<Message>());
			m_messageIndicesBySeverity = std::exchange(other.m_messageIndicesBySeverity, std::array<std::vector<size_t>, 4>());
			m_messageIndicesByType = std::exchange(other.m_messageIndicesByType, std::array<std::vector<size_t>, 4>());
			return *this;
		}

		void logMessage(Flags::DebugMessageSeverity::Bits messageSeverity,
			Flags::DebugMessageType messageType,
			const DebugUtils::CallbackData* pCallbackData);

		template<Flags::DebugMessageSeverity::Bits severity>
		void logMessage(Flags::DebugMessageType messageType, const DebugUtils::CallbackData* pCallbackData) {
			std::unique_lock<std::shared_mutex> lock(m_mutex);

			m_messages.push_back({ severity, messageType,
				pCallbackData->getMessage().data(),
				pCallbackData->getMessageIdName().data(),
				std::chrono::steady_clock::now()
				});
			addMessageByType(m_messages.back(), m_messages.size() - 1);
			m_messageIndicesBySeverity[messageSeverityToIndex<severity>()].push_back(m_messages.size() - 1);
		}

		std::span<const Message> getMessages() const {
			return m_messages;
		};
		const Message& getMessageBySeverity(Flags::DebugMessageSeverity::Bits severity, size_t index) const;
		size_t getMessageBySeveritySize(Flags::DebugMessageSeverity::Bits type) const;
		const Message& getMessageByType(Flags::DebugMessageType::Bits type, size_t index) const;
		size_t getMessageByTypeSize(Flags::DebugMessageType::Bits type) const;

		template<Flags::DebugMessageSeverity::Bits severity>
		const Message& getMessageBySeverity(size_t index) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messages[m_messageIndicesBySeverity[messageSeverityToIndex<severity>()][index]];
		};

		template<Flags::DebugMessageSeverity::Bits severity>
		size_t getMessageBySeveritySize() const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messageIndicesBySeverity[messageSeverityToIndex<severity>()].size();
		}

		template<Flags::DebugMessageType::Bits type>
		const Message& getMessageByType(size_t index) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messages[m_messageIndicesByType[messageTypeToIndex<type>()][index]];
		}
		template<Flags::DebugMessageType::Bits type>
		size_t getMessageByTypeSize() const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return m_messageIndicesByType[messageTypeToIndex<type>()].size();
		}

		void dumpMessagesToStream(std::ostream& stream) const {
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			for (const auto& message : m_messages) {
				switch (message.messageSeverity) {
				case Flags::DebugMessageSeverity::Bits::Verbose:
					// Diagnostic messages
					stream << "VERBOSE: " << message.message << '\n';
					break;

				case Flags::DebugMessageSeverity::Bits::Info:
					// Informational messages like resource creation
					stream << "INFO: " << message.message << '\n';
					break;

				case Flags::DebugMessageSeverity::Bits::Warning:
					// Warning messages like use of deprecated functions
					stream << "WARNING: " << message.message << '\n';
					break;

				case Flags::DebugMessageSeverity::Bits::Error:
					// Error messages for invalid behavior
					stream << "ERROR: " << message.message << '\n';
					break;

				default:
					stream << "UNKNOWN SEVERITY: " << message.message << '\n';
					break;
				}
			}
		}

		void reset() {
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			m_messages.clear();
			m_messageIndicesBySeverity.fill({});
			m_messageIndicesByType.fill({});
		}

	private:
		void addMessageBySeverity(Message& message, size_t index);
		void addMessageByType(Message& message, size_t index);

		static size_t messageTypeToIndex(Flags::DebugMessageType::Bits messageType);
		static size_t messageSeverityToIndex(Flags::DebugMessageSeverity::Bits messageSeverity);

		template<Flags::DebugMessageType::Bits messageType>
		static size_t messageTypeToIndex() {
			if constexpr (messageType == Flags::DebugMessageType::Bits::General) {
				return 0;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::Validation) {
				return 1;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::Performance) {
				return 2;
			}
			else if constexpr (messageType == Flags::DebugMessageType::Bits::DeviceAddressBinding) {
				return 3;
			}
			else {
				static_assert(false, "Invalid message type");
			}
		}

		template<Flags::DebugMessageSeverity::Bits messageSeverity>
		static size_t messageSeverityToIndex() {
			if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Verbose) {
				return 0;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Info) {
				return 1;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Warning) {
				return 2;
			}
			else if constexpr (messageSeverity == Flags::DebugMessageSeverity::Bits::Error) {
				return 3;
			}
			else {
				static_assert(false, "Invalid message severity");
			}
		}
	};
}

