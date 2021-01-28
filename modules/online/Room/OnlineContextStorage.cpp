#include "OnlineContextStorage.h"

OnlineContextStorage::OnlineContextStorage(const Config *config) : config_(config)
{
    nbest_value = config_->default_nbest_value;
}

