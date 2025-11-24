//
// Created by magofrays on 10/26/25.
//

#include "BodyData.h"

std::vector<BodyState> BodyData::get_body_states() const
{
    return bodyStates;
}

void BodyData::addBodyState(const BodyState& body_state)
{
    bodyStates.push_back(body_state);
}
