#include "Graphics/Wrappers/DebugLogger.h"

namespace Graphics::Wrappers {

	void DebugLogger::logMessage(Flags::DebugMessageSeverity::Bits messageSeverity,
		Flags::DebugMessageType messageType,
		const DebugUtils::CallbackData* pCallbackData) {
		std::unique_lock<std::shared_mutex> lock(m_mutex);

		m_messages.push_back({ messageSeverity, messageType,
			pCallbackData->getMessage().data(),
			pCallbackData->getMessageIdName().data(),
			std::chrono::steady_clock::now()
			});
		addMessageBySeverity(m_messages.back(), m_messages.size() - 1);
		addMessageByType(m_messages.back(), m_messages.size() - 1);
	}

	void DebugLogger::addMessageBySeverity(Message& message, size_t index)
	{
		m_messageIndicesBySeverity[messageSeverityToIndex(message.messageSeverity)].push_back(index);
	}

	void DebugLogger::addMessageByType(Message& message, size_t index)
	{
		if (message.messageType.hasFlag(Flags::DebugMessageType::Bits::General))
			m_messageIndicesByType[0].push_back(index);
		if (message.messageType.hasFlag(Flags::DebugMessageType::Bits::Validation))
			m_messageIndicesByType[1].push_back(index);
		if (message.messageType.hasFlag(Flags::DebugMessageType::Bits::Performance))
			m_messageIndicesByType[2].push_back(index);
		if (message.messageType.hasFlag(Flags::DebugMessageType::Bits::DeviceAddressBinding))
			m_messageIndicesByType[3].push_back(index);
	}

	const DebugLogger::Message& DebugLogger::getMessageBySeverity(Flags::DebugMessageSeverity::Bits severity, size_t index) const {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		return m_messages[m_messageIndicesBySeverity[messageSeverityToIndex(severity)][index]];
	};

	size_t DebugLogger::getMessageBySeveritySize(Flags::DebugMessageSeverity::Bits type) const {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		return m_messageIndicesBySeverity[messageSeverityToIndex(type)].size();
	}

	const DebugLogger::Message& DebugLogger::getMessageByType(Flags::DebugMessageType::Bits type, size_t index) const {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		return m_messages[m_messageIndicesByType[messageTypeToIndex(type)][index]];
	}

	size_t DebugLogger::getMessageByTypeSize(Flags::DebugMessageType::Bits type) const {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		return m_messageIndicesByType[messageTypeToIndex(type)].size();
	}

	size_t DebugLogger::messageTypeToIndex(Flags::DebugMessageType::Bits messageType) {
		switch (messageType) {
		case Flags::DebugMessageType::Bits::General:
			return 0;
		case Flags::DebugMessageType::Bits::Validation:
			return 1;
		case Flags::DebugMessageType::Bits::Performance:
			return 2;
		case Flags::DebugMessageType::Bits::DeviceAddressBinding:
			return 3;
		default:
			return 4;
		}
	}

	size_t DebugLogger::messageSeverityToIndex(Flags::DebugMessageSeverity::Bits messageSeverity) {
		switch (messageSeverity) {
		case Flags::DebugMessageSeverity::Bits::Verbose:
			return 0;
		case Flags::DebugMessageSeverity::Bits::Info:
			return 1;
		case Flags::DebugMessageSeverity::Bits::Warning:
			return 2;
		case Flags::DebugMessageSeverity::Bits::Error:
			return 3;
		default:
			return 4;
		}
	}
}