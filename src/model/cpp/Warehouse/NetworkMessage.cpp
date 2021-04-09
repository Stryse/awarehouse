#include "NetworkMessage.h"

MoveAgentMessage::MoveAgentMessage(const DirectionVector<> &moveDirection)
    : moveDirection(moveDirection)
{
}

MoveAgentMessage::~MoveAgentMessage() {}

const DirectionVector<> &MoveAgentMessage::getMoveDirection() const { return moveDirection; }