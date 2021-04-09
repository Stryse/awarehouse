#include "NetworkMessage.h"

MoveAgentMessage::MoveAgentMessage(const DirectionVector<> &moveDirection, int senderAddress)
    : NetworkMessage(senderAddress), moveDirection(moveDirection)
{
}

MoveAgentMessage::~MoveAgentMessage() {}

const DirectionVector<> &MoveAgentMessage::getMoveDirection() const { return moveDirection; }